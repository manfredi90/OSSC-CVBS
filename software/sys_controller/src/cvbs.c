//
// CVBS firmware: composite / S-Video colour decoder control
//
// The decoder itself is in the FPGA (rtl/cvbs_decoder.v). The CPU:
//  - sets up the TVP7002 for composite video on the AV2 Y input (and the
//    S-Video chroma on Pb): no colour space conversion, gain with room for
//    the sync, burst and chroma below blank and above white, clamp after
//    the burst, no ALC
//  - computes the DDS increment from the measured line rate and the
//    subcarrier frequency of the colour system
//  - computes the chroma coefficient (saturation, hue, burst/chroma ratio of
//    the standard) and the luma gain
//  - detects the colour system (Auto): 3.58 or 4.43MHz from the colour
//    killer, PAL or NTSC from the burst phase alternation
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//

#include <stdio.h>
#include <string.h>
#include "system.h"
#include "sys/alt_timestamp.h"
#include "av_controller.h"
#include "avconfig.h"
#include "tvp7002.h"
#include "tvp7002_regs.h"
#include "sc_config_regs.h"
#include "lcd.h"
#include "cvbs.h"

extern avmode_t cm;
extern volatile sc_regs *sc;
extern char func_ret_status[];

uint8_t cv_mode;

// subcarrier frequencies x64 (Hz)
#define FSC_X64_PAL     283751600ULL    // 4433618.75
#define FSC_X64_NTSC    229090909ULL    // 3579545.45
#define FSC_X64_PALM    228839135ULL    // 3575611.49
#define FSC_X64_PALN    229251600ULL    // 3582056.25

// chroma coefficient for 100% saturation (burst / chroma ratio of the
// standard and the decoder scaling, see cvbs_decoder.v), x10
#define HC_K_PAL_X10    773
#define HC_K_NTSC_X10   1020

// TVP7002 clamp width (the start is a setting: pixels after the hsync trailing
// edge at ~27MHz, default 92: after the burst)
#define CV_CLAMP_WIDTH  12

// decoder status (lt_status register)
#define ST_BURST(s)     ((s) & 0xff)
#define ST_BLANK(s)     (((s) >> 8) & 0xff)
#define ST_KILL(s)      (((s) >> 24) & 1)
#define ST_ALT(s)       (((s) >> 25) & 1)
#define ST_CSAT(s)      (((s) >> 26) & 1)
#define ST_CORR(s)      ((int)(((s) >> 27) & 0x1f) - (int)((((s) >> 31) & 1) << 5))

// input mode
static uint16_t in_samples;
static uint32_t in_totlines, in_pcnt;
static uint8_t in_ilace;
static uint8_t notch_3l;    // 3-line luma notch in use (diagnostics)

// automatic detection state
static uint8_t auto_other;      // 0: usual subcarrier for the line count (625: 4.43MHz, 525: 3.58MHz), 1: the other one
static uint8_t auto_pal;        // PAL (525 lines)
static uint32_t auto_ts, auto_kill_since, auto_alt_since;
static uint8_t auto_alt_last;

static int lines625() {
    // 576i: 625 lines per frame; 288p: ~312 per field
    return in_ilace ? (in_totlines > 575) : (in_totlines > 287);
}

uint8_t cvbs_input_mode(avinput_t input) {
    if (input == AV2_RGsB)
        return 1;
    if (input == AV2_YPBPR)
        return 2;
    return 0;
}

int cvbs_group_ok(video_group group) {
    return (group == GROUP_240P) || (group == GROUP_288P) || (group == GROUP_480I) || (group == GROUP_576I);
}

int cvbs_line2x(avconfig_t *cc, video_group group) {
    return cv_mode && cc->cv_line2x && cvbs_group_ok(group);
}

void cvbs_reset_auto() {
    auto_other = 0;
    auto_pal = 0;
    auto_ts = alt_timestamp();
    auto_kill_since = 0;
    auto_alt_since = 0;
    auto_alt_last = 0;
}

void cvbs_tvp_setup(uint8_t h_synclen_px, uint16_t h_samplerate, uint32_t refclks_per_line) {
    avconfig_t *cc = &cm.cc;
    uint8_t cg = cc->cv_adcgain & 0xf;
    // same clamp reference compensation as tvp_source_setup(): the reference
    // is the end of the MVS window, not the hsync trailing edge
    int clamp_ref_offset = h_synclen_px - (((30*h_samplerate)/refclks_per_line)+5)/10;
    int clamp_pos = clamp_ref_offset + cc->cv_clamp;

    // sample clock: H-PLL (line-locked) or the 27MHz crystal on EXT_CLK (no
    // H-PLL jitter: the colour subcarrier phase stays steady along the line)
    tvp_set_ext_pclk(cv_mode && cc->cv_xtal);

    if (!cv_mode)
        return;

    if (clamp_pos < 0)
        clamp_pos = 0;
    else if (clamp_pos > 255-CV_CLAMP_WIDTH)
        clamp_pos = 255-CV_CLAMP_WIDTH;

    // gain: full scale 1/(0.5+cg/10) Vpp on all channels, no digital gain
    tvp_writereg(TVP_BG_CGAIN, (cg << 4) | cg);
    tvp_writereg(TVP_R_CGAIN, cg);
    tvp_writereg(TVP_R_FGAIN, 0);
    tvp_writereg(TVP_G_FGAIN, 0);
    tvp_writereg(TVP_B_FGAIN, 0);
    tvp_writereg(TVP_R_FOFFSET_MSB, 0x80);
    tvp_writereg(TVP_G_FOFFSET_MSB, 0x80);
    tvp_writereg(TVP_B_FOFFSET_MSB, 0x80);
    tvp_writereg(TVP_FOFFSET_LSB, 0x00);

    // clamps: all mid level. Y: blank at ADC code 128 + coarse offset (default
    // -31: ~97), room below blank for the sync, the burst and the chroma
    // (bottom level clamping put the blank too low: the burst clipped).
    // Pb (S-Video chroma): centered at 128.
    tvp_writereg(TVP_SOGTHOLD, (tvp_readreg(TVP_SOGTHOLD) & 0xF8) | 0x7);
    tvp_writereg(TVP_FCLAMPCTRL, tvp_readreg(TVP_FCLAMPCTRL) & 0x7F);    // no CM offset (bottom clamp only)
    {
        int ofs = (int)cc->cv_adcofs - SIGNED_NUMVAL_ZERO;   // 8-bit codes = 4 counts (coarse offset step)
        uint8_t reg = (ofs < 0) ? (0x20 | (uint8_t)(-ofs)) : (uint8_t)ofs;
        tvp_writereg(TVP_G_COFFSET, reg & 0x3f);
        tvp_writereg(TVP_B_COFFSET, 0x00);
        tvp_writereg(TVP_R_COFFSET, 0x00);
    }

    // clamp after the burst, no ALC (the decoder measures the blank level)
    tvp_writereg(TVP_CLAMPSTART, (alt_u8)clamp_pos);
    tvp_writereg(TVP_CLAMPWIDTH, CV_CLAMP_WIDTH);
    tvp_writereg(TVP_ALCEN, 0x00);

    printf("CVBS: %s, ADC gain %u, offset %d, clamp pos %d, %s clock\n", (cv_mode == 2) ? "S-Video" : "composite", (unsigned)cg, (int)cc->cv_adcofs - SIGNED_NUMVAL_ZERO, clamp_pos, cc->cv_xtal ? "27MHz xtal" : "H-PLL");
}

void cvbs_set_input(uint16_t samples_per_line, uint32_t totlines, uint8_t interlaced, uint32_t pcnt_field) {
    uint8_t was625 = lines625();
    in_samples = samples_per_line;
    in_totlines = totlines;
    in_ilace = interlaced;
    in_pcnt = pcnt_field;
    if (lines625() != was625)
        cvbs_reset_auto();
}

// colour system being decoded
static cv_std_t cur_std() {
    cv_std_t std = (cv_std_t)cm.cc.cv_std;

    if (std != CV_STD_AUTO)
        return std;
    if (lines625())
        return auto_other ? CV_STD_PALN : CV_STD_PAL;
    if (auto_other)
        return auto_pal ? CV_STD_PAL60 : CV_STD_NTSC443;
    return auto_pal ? CV_STD_PALM : CV_STD_NTSC;
}

const char* cvbs_std_name() {
    static const char* const names[] = { "Auto", "PAL", "NTSC", "PAL-60", "NTSC4.43", "PAL-M", "PAL-N" };
    return names[cur_std()];
}

// cos / sin of a small angle (degrees), Q16
static void cos_sin_q16(int deg, int32_t *c, int32_t *s) {
    int64_t x = (int64_t)deg * 1144;            // radians, Q16
    int64_t x2 = (x * x) >> 16;
    int64_t x3 = (x2 * x) >> 16;
    int64_t x4 = (x2 * x2) >> 16;
    *c = (int32_t)(65536 - x2/2 + x4/24);
    *s = (int32_t)(x - x3/6);
}

// diagnostic view (not kept): colour deviation from its average over the
// lines, x8 on grey
// test option (not kept, default both), 27MHz xtal clock output timing:
// 0 auto (resync at every input frame + output line length from the
// measured period, rounded up, never below the preset), 1 resync only, 2 off
// test option (not kept), 27MHz xtal clock: vsync from the TVP7002 (0) or
// from the composite sync (1, own)

void cvbs_get_regs(uint32_t *hc, uint32_t *flags, uint32_t *inc) {
    avconfig_t *cc = &cm.cc;
    cv_std_t std = cur_std();
    int pal = (std == CV_STD_PAL) || (std == CV_STD_PAL60) || (std == CV_STD_PALM) || (std == CV_STD_PALN);
    uint64_t fsc_x64, fs_x64;
    int32_t c, s, k, re, im, lg, white_x10, hue, bright, rf, setup;
    int sat = cc->cv_sat * 5;

    switch (std) {
    case CV_STD_PALM:
        fsc_x64 = FSC_X64_PALM;
        break;
    case CV_STD_PALN:
        fsc_x64 = FSC_X64_PALN;
        break;
    case CV_STD_NTSC:
        fsc_x64 = FSC_X64_NTSC;
        break;
    default:
        fsc_x64 = FSC_X64_PAL;
        break;
    }

    // sample rate x64: the crystal (27MHz xtal sample clock), else from the
    // measured field period (27MHz clocks)
    if (cc->cv_xtal) {
        fs_x64 = (uint64_t)TVP_EXTCLK_HZ * 64;
        *inc = (uint32_t)((fsc_x64 << 32) / fs_x64);
    } else if (in_pcnt && in_totlines && in_samples) {
        fs_x64 = ((uint64_t)TVP_EXTCLK_HZ * 64 * in_samples * in_totlines) / ((uint64_t)in_pcnt * (in_ilace ? 2 : 1));
        *inc = (uint32_t)((fsc_x64 << 32) / fs_x64);
    } else {
        *inc = 0;
    }

    // chroma coefficient: K * saturation * e^(-j hue)
    hue = (int)cc->cv_hue - SIGNED_NUMVAL_ZERO;
    cos_sin_q16(-hue, &c, &s);
    k = pal ? HC_K_PAL_X10 : HC_K_NTSC_X10;
    re = (int32_t)(((int64_t)k * sat * c + (5*65536LL*100)) / (10*100*65536LL));
    im = (int32_t)(((int64_t)k * sat * s) / (10*100*65536LL));

    // luma gain: white (0.7V) to 255, times contrast; x32, 32-95
    white_x10 = (1785 * (5 + (cc->cv_adcgain & 0xf))) / 10;     // ADC codes of 0.7V, x10
    lg = (81600 * (cc->cv_contrast * 2) + white_x10*50) / (white_x10 * 100);
    // NTSC 7.5 IRE setup (NTSC-M, PAL-M): black at 7.5 IRE above the blank
    // level, white at 100: black -> 0 and white -> 255 again (gain x100/92.5,
    // offset in the brightness term, 2 codes per step). Auto: NTSC / NTSC
    // 4.43 / PAL-M (NTSC-J has no setup: Off).
    setup = (cc->cv_setup == 2) ||
            ((cc->cv_setup == 0) && ((std == CV_STD_NTSC) || (std == CV_STD_NTSC443) || (std == CV_STD_PALM)));
    if (setup)
        lg = (lg * 200 + 92) / 185;
    if (lg < 32)
        lg = 32;
    else if (lg > 95)
        lg = 95;

    // reference low-pass over lines: with the crystal clock the burst phase
    // is the same on every line (frequency loop); line-locked it moves
    if (cc->cv_reffilt == 0)
        rf = cc->cv_xtal ? 1 : 0;
    else
        rf = (cc->cv_reffilt == 2);
    *hc = ((uint32_t)re & 0x3ff) | (((uint32_t)im & 0x3ff) << 16) | ((uint32_t)(lg - 32) << 26) |
          ((uint32_t)(rf & 1) << 10);
    *hc |= ((uint32_t)(cc->cv_sharp & 7) << 11);  // FPGA: hc[13:11] = sharpness
    // (VHS firmware: plain luma notch only; hc[14]: diagnostics select)
    // hc[15]: PAL delay line (the FPGA applies it to the PAL standards only)
    *hc |= ((uint32_t)(cc->cv_paldl_off == 0) << 15);

    bright = (int)cc->cv_bright - SIGNED_NUMVAL_ZERO;
    if (setup) {
        // 7.5 IRE (53.6mV) in ADC codes x10, times the luma gain, in 2-code steps
        int32_t s_x10 = (white_x10 * 766 + 5000) / 10000;
        bright -= (s_x10 * lg + 320) / 640;
    }
    if (bright < -32)
        bright = -32;
    else if (bright > 31)
        bright = 31;
    *flags = cv_mode ? (1 |
                        ((cv_mode == 2) << 1) |
                        (pal << 2) |
                        ((fsc_x64 == FSC_X64_PAL) << 3) |
                        ((cc->cv_notch_off == 1) << 4) |
                        ((cc->cv_xtal != 0) << 5) |
                        ((uint32_t)(cc->cv_xtal != 0) << 7) |
                        (((cc->cv_xtal != 0) && (cc->cv_retime_off == 0)) << 6) |
                        ((uint32_t)cc->cv_bgate << 8) |
                        (((uint32_t)bright & 0x3f) << 16)) : 0;
    // flags[23:22] (sl_config2[31:30]): VHS diagnostics select (ossc.v), 0 here
}

int cvbs_update() {
    uint32_t st = sc->lt_status.data;
    uint32_t now = alt_timestamp();
    uint32_t tps = alt_timestamp_freq();
    int changed = 0;

    if (!cv_mode || (cm.cc.cv_std != CV_STD_AUTO))
        return 0;

    // settle after a change (the frequency loop and the burst filter)
    if (now - auto_ts < tps/2)
        return 0;

    // no colour for 1s: try the other subcarrier frequency (a B&W source
    // just keeps toggling, which is harmless)
    if (ST_KILL(st)) {
        if (!auto_kill_since)
            auto_kill_since = now;
        else if (now - auto_kill_since > tps) {
            auto_other = !auto_other;
            auto_pal = 0;
            auto_kill_since = 0;
            auto_alt_since = 0;
            auto_ts = now;
            changed = 1;
            printf("CVBS auto: trying %s\n", cvbs_std_name());
        }
        return changed;
    }
    auto_kill_since = 0;

    // 525 lines: PAL (PAL-60 / PAL-M) or NTSC from the burst alternation,
    // stable for 0.5s
    if (!lines625()) {
        if (ST_ALT(st) != auto_alt_last) {
            auto_alt_last = ST_ALT(st);
            auto_alt_since = now;
        } else if (auto_alt_since && (now - auto_alt_since > tps/2) && (auto_pal != auto_alt_last)) {
            auto_pal = auto_alt_last;
            auto_ts = now;
            changed = 1;
            printf("CVBS auto: %s\n", cvbs_std_name());
        }
    }
    return changed;
}

void cvbs_diag_str(char *s, int len) {
    uint32_t st = sc->lt_status.data;
    sniprintf(s, len, "B%u L%u %c", (unsigned)ST_BURST(st), (unsigned)ST_BLANK(st),
              ST_KILL(st) ? 'K' : (ST_ALT(st) ? 'P' : 'N'));
    if (notch_3l && (strlen(s) + 2 <= (unsigned)len))
        strcat(s, "3");
}

// cvbs_sync_diag() / cvbs_xtal_diag(): in av_controller.c (output timing state)

int cvbs_diagnostics() {
    if (!cv_mode || (cm.id < 0)) {
        strncpy(func_ret_status, "No CVBS input", LCD_ROW_LEN+1);
        return 1;
    }
    cvbs_diag_str(func_ret_status, LCD_ROW_LEN+1);
    return 1;
}

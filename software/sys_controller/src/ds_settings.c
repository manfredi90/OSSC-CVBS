//
// Downscaler firmware settings storage.
//
// This firmware runs from the secondary slot and must never touch anything
// that belongs to the stock (primary) firmware: its image, its profiles in the
// userdata area (0x100000-0x1FFFFF) or its SD card profiles. All settings of
// this firmware live in a single flash sector at the very end of the secondary
// slot, which the secondary image is not allowed to reach (see
// tools/create_fw_img.c, FW_MAX_DATA_LEN).
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include "system.h"
#include "ds_settings.h"
#include "flash.h"
#include "avconfig.h"
#include "av_controller.h"
#include "controls.h"
#include "utils.h"
#include "cvbs.h"

// VHS firmware: own magic (the CVBS / downscaler / HD firmware settings in
// the same sector have another format and are not loaded)
#define DS_SETTINGS_MAGIC   0x4F535648  // "OSVH"
#define DS_SETTINGS_VERSION 3     // 1: VHS 0.1, 2: 0.5 (PAL delay line), 3: 0.12 (line sync source)
#define V5_TC_LESS          1     // bytes added to avconfig_t in v6
#define V3_TC_LESS          (2 + V5_TC_LESS)    // bytes added to avconfig_t in v4 and v6

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t len;           // total length of this struct
    avconfig_t tc;
    settings_t ts;
    uint16_t rc_keymap[REMOTE_MAX_KEYS];
    uint32_t crc;           // crc32 of everything above
} __attribute__((packed, aligned(4))) ds_settings_blob_t;

extern flash_ctrl_dev flashctrl_dev;
extern avconfig_t tc;
extern settings_t ts;
extern alt_u16 rc_keymap[REMOTE_MAX_KEYS];

static ds_settings_blob_t blob;

static uint32_t blob_crc(const ds_settings_blob_t *b) {
    return crc32((unsigned char*)b, offsetof(ds_settings_blob_t, crc), 1);
}

void set_default_settings();

int ds_settings_load() {
    const ds_settings_blob_t *f = (const ds_settings_blob_t*)(INTEL_GENERIC_SERIAL_FLASH_INTERFACE_TOP_0_AVL_MEM_BASE + DS_SETTINGS_FLASH_ADDR);

    extern const avconfig_t tc_default;

    memcpy(&blob, f, sizeof(blob));

    if (((blob.magic != DS_SETTINGS_MAGIC) ||
         (blob.version != DS_SETTINGS_VERSION) ||
         (blob.len != sizeof(ds_settings_blob_t)) ||
         (blob.crc != blob_crc(&blob))))
    {
        printf("DS settings: none stored (using defaults)\n");
        return -1;
    }

    memcpy(&tc, &blob.tc, sizeof(avconfig_t));
    // settings saved before v0.12 have 0 (= unset) in the position fields
    if (tc.ds_hpos == 0)
        tc.ds_hpos = SIGNED_NUMVAL_ZERO;
    if (tc.ds_vpos == 0)
        tc.ds_vpos = SIGNED_NUMVAL_ZERO;
    if (tc.ds_width == 0)
        tc.ds_width = SIGNED_NUMVAL_ZERO;
    // options no longer in the menu (v1.0): full resolution sampling,
    // 260-264 lines, no phase hotkey (fixed sampling phase)
    tc.ds_native = 1;
    tc.ds_fixlines = 0;
    if (tc.ds_width < DS_WIDTH_MIN)
        tc.ds_width = DS_WIDTH_MIN;
    if (tc.ds_width > DS_WIDTH_MAX)
        tc.ds_width = DS_WIDTH_MAX;
    if (tc.ds_i2p > 1)
        tc.ds_i2p = 0;
    if (tc.ds_flicker > 4)
        tc.ds_flicker = 0;
    // test pattern design: not kept, Gray at start
    tc.ds_tp_sel = 0;
    // VHS output: Archive (480i / 576i), View (480p / 576p)
    if (tc.cv_line2x > 1)
        tc.cv_line2x = 1;
    if (tc.cv_lockstep > 3)
        tc.cv_lockstep = 3;
    if (tc.cv_paldl_off > 1)
        tc.cv_paldl_off = 0;
    tc.cv_rsv1 = 0;
    if (tc.ds_out_range > 2)
        tc.ds_out_range = 0;
    // decoder settings out of range: defaults
    if ((tc.cv_std > CV_STD_PALN) || (tc.cv_sat > CV_SAT_MAX) ||
        (tc.cv_hue < SIGNED_NUMVAL_ZERO-CV_HUE_RANGE) || (tc.cv_hue > SIGNED_NUMVAL_ZERO+CV_HUE_RANGE) ||
        (tc.cv_contrast < CV_CONTRAST_MIN) || (tc.cv_contrast > CV_CONTRAST_MAX) ||
        (tc.cv_bright < SIGNED_NUMVAL_ZERO-CV_BRIGHT_RANGE) || (tc.cv_bright > SIGNED_NUMVAL_ZERO+CV_BRIGHT_RANGE) ||
        (tc.cv_adcgain > 15) || (tc.cv_bgate < CV_BGATE_MIN) || (tc.cv_bgate > CV_BGATE_MAX) ||
        (tc.cv_adcofs < SIGNED_NUMVAL_ZERO-CV_ADCOFS_RANGE) || (tc.cv_adcofs > SIGNED_NUMVAL_ZERO+CV_ADCOFS_RANGE) ||
        (tc.cv_edge > 3) || (tc.cv_clamp > CV_CLAMP_MAX)) {
        memcpy(&tc.cv_std, &tc_default.cv_std, offsetof(avconfig_t, cv_clamp) + 1 - offsetof(avconfig_t, cv_std));
    }
    // VHS firmware: HDMI only (separate syncs), no edge suppression
    tc.ds_csync = 0;
    tc.cv_edge = 0;
    if (tc.cv_sharp > CV_SHARP_MAX)
        tc.cv_sharp = 0;
    if (tc.cv_reffilt > 2)
        tc.cv_reffilt = 0;
    if (tc.cv_setup > 2)
        tc.cv_setup = 0;
    // luma notch: 0 on (plain), 1 off; always the 27MHz crystal clock
    if (tc.cv_notch_off > 1)
        tc.cv_notch_off = 0;
    tc.cv_xtal = 1;
    if (tc.s480p_mode > 2)
        tc.s480p_mode = 0;
    if (tc.ds_720p_ar == 3)       // 1.2.2-1.2.3 "Wide (216p)": now "Wide"
        tc.ds_720p_ar = 2;
    else if (tc.ds_720p_ar > 3)
        tc.ds_720p_ar = 0;
    memcpy(&ts, &blob.ts, sizeof(settings_t));
    if (ts.phase_hotkey_enable > 1)
        ts.phase_hotkey_enable = 0;
    // CVBS firmware: AV2 composite / S-Video only, no input autodetection
    // (Initial input AV2_RGsB+1 = last used, was AV_LAST)
    if (ts.def_input == AV_LAST)
        ts.def_input = AV2_RGsB+1;
    else if ((ts.def_input < AV2_YPBPR) || (ts.def_input > AV2_RGsB+1))
        ts.def_input = AV2_RGsB;
    ts.auto_input = AUTO_OFF;
    memcpy(rc_keymap, blob.rc_keymap, sizeof(blob.rc_keymap));
    printf("DS settings loaded\n");

    return 0;
}

int ds_settings_save() {
    uint32_t *src, *dst;
    unsigned i;

    memset(&blob, 0, sizeof(blob));
    blob.magic = DS_SETTINGS_MAGIC;
    blob.version = DS_SETTINGS_VERSION;
    blob.len = sizeof(ds_settings_blob_t);
    memcpy(&blob.tc, &tc, sizeof(avconfig_t));
    memcpy(&blob.ts, &ts, sizeof(settings_t));
    memcpy(blob.rc_keymap, rc_keymap, sizeof(blob.rc_keymap));
    blob.crc = blob_crc(&blob);

    // The only flash write performed by this firmware
    flash_write_protect(&flashctrl_dev, 0);
    flash_sector_erase(&flashctrl_dev, DS_SETTINGS_FLASH_ADDR);

    src = (uint32_t*)&blob;
    dst = (uint32_t*)(INTEL_GENERIC_SERIAL_FLASH_INTERFACE_TOP_0_AVL_MEM_BASE + DS_SETTINGS_FLASH_ADDR);
    for (i=0; i<(sizeof(blob)+3)/4; i++)
        dst[i] = src[i];

    flash_write_protect(&flashctrl_dev, 1);

    // verify
    if (memcmp((void*)(INTEL_GENERIC_SERIAL_FLASH_INTERFACE_TOP_0_AVL_MEM_BASE + DS_SETTINGS_FLASH_ADDR), &blob, sizeof(blob)) != 0) {
        printf("DS settings: verify failed\n");
        return -1;
    }

    printf("DS settings saved\n");
    return 0;
}

// Restore defaults (applied immediately, stored only with "Save settings")
int ds_settings_reset() {
    set_default_profile(0);
    set_default_settings();
    return 0;
}

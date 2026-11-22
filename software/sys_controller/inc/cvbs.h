//
// CVBS firmware: composite / S-Video colour decoder control
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//

#ifndef CVBS_H_
#define CVBS_H_

#include <stdint.h>
#include "avconfig.h"
#include "video_modes.h"

typedef enum {
    CV_STD_AUTO     = 0,
    CV_STD_PAL      = 1,
    CV_STD_NTSC     = 2,
    CV_STD_PAL60    = 3,
    CV_STD_NTSC443  = 4,
    CV_STD_PALM     = 5,
    CV_STD_PALN     = 6,
} cv_std_t;

#define CV_SAT_MAX          40      // 200%
#define CV_HUE_RANGE        30      // +-30 degrees
#define CV_CONTRAST_MIN     25      // 50%
#define CV_CONTRAST_MAX     80      // 160%
#define CV_BRIGHT_RANGE     31
#define CV_ADCOFS_RANGE     31
#define CV_ADCOFS_DEFAULT   (SIGNED_NUMVAL_ZERO-31)     // blank at ~97
#define CV_CLAMP_MIN        0
#define CV_CLAMP_MAX        120
#define CV_SHARP_MAX        7
#define CV_REFFILT_MAX      3
#define CV_BGATE_MIN        120
#define CV_BGATE_MAX        220

// 0 = not a decoder input, 1 = composite (AV2 Y), 2 = S-Video (AV2 Y + Pb)
extern uint8_t cv_mode;

// decoder input for an AV input (0 if none)
uint8_t cvbs_input_mode(avinput_t input);
// 15kHz SD groups that can be decoded
int cvbs_group_ok(video_group group);
// TVP7002 setup for the decoder inputs (after tvp_source_setup)
void cvbs_tvp_setup(uint8_t h_synclen_px, uint16_t h_samplerate, uint32_t refclks_per_line);
// input mode parameters: samples per line, lines, field period (27MHz clocks)
void cvbs_set_input(uint16_t samples_per_line, uint32_t totlines, uint8_t interlaced, uint32_t pcnt_field);
// decoder registers: hc (sl_config), flags (sl_config2[31:8]), increment (sl_config3)
void cvbs_get_regs(uint32_t *hc, uint32_t *flags, uint32_t *inc);
// Line2x test output selected and possible (decoder input, 15kHz SD mode)
int cvbs_line2x(avconfig_t *cc, video_group group);
// periodic: colour system detection. Returns 1 if the registers must be updated.
int cvbs_update();
// name of the colour system being decoded
const char* cvbs_std_name();
// diagnostics (16 characters)
void cvbs_diag_str(char *s, int len);
int cvbs_diagnostics();
// reset the automatic detection (new input or mode)
void cvbs_reset_auto();

#endif

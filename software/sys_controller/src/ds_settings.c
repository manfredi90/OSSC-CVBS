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

// CVBS firmware: own magic (the downscaler / HD firmware settings in the same
// sector have another format and are not loaded)
#define DS_SETTINGS_MAGIC   0x4F534356  // "OSCV"
#define DS_SETTINGS_VERSION 2     // 2: 0.2 (ADC offset instead of CM offset, settings of 0.1 not loaded)

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

    memcpy(&blob, f, sizeof(blob));

    if ((blob.magic != DS_SETTINGS_MAGIC) ||
        (blob.version != DS_SETTINGS_VERSION) ||
        (blob.len != sizeof(ds_settings_blob_t)) ||
        (blob.crc != blob_crc(&blob)))
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
    if (tc.ds_out_range > 2)
        tc.ds_out_range = 0;
    // decoder settings out of range: defaults
    if ((tc.cv_std > CV_STD_PALN) || (tc.cv_sat > CV_SAT_MAX) ||
        (tc.cv_hue < SIGNED_NUMVAL_ZERO-CV_HUE_RANGE) || (tc.cv_hue > SIGNED_NUMVAL_ZERO+CV_HUE_RANGE) ||
        (tc.cv_contrast < CV_CONTRAST_MIN) || (tc.cv_contrast > CV_CONTRAST_MAX) ||
        (tc.cv_bright < SIGNED_NUMVAL_ZERO-CV_BRIGHT_RANGE) || (tc.cv_bright > SIGNED_NUMVAL_ZERO+CV_BRIGHT_RANGE) ||
        (tc.cv_adcgain > 15) || (tc.cv_bgate < CV_BGATE_MIN) || (tc.cv_bgate > CV_BGATE_MAX) ||
        (tc.cv_adcofs < SIGNED_NUMVAL_ZERO-CV_ADCOFS_RANGE) || (tc.cv_adcofs > SIGNED_NUMVAL_ZERO+CV_ADCOFS_RANGE)) {
        extern const avconfig_t tc_default;
        memcpy(&tc.cv_std, &tc_default.cv_std, offsetof(avconfig_t, cv_adcofs) + 1 - offsetof(avconfig_t, cv_std));
    }
    if (tc.s480p_mode > 2)
        tc.s480p_mode = 0;
    if (tc.ds_720p_ar == 3)       // 1.2.2-1.2.3 "Wide (216p)": now "Wide"
        tc.ds_720p_ar = 2;
    else if (tc.ds_720p_ar > 3)
        tc.ds_720p_ar = 0;
    memcpy(&ts, &blob.ts, sizeof(settings_t));
    ts.phase_hotkey_enable = 0;
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

//
// Copyright (C) 2015-2023  Markus Hiienkari <mhiienka@niksula.hut.fi>
//
// This file is part of Open Source Scan Converter project.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef FIRMWARE_H_
#define FIRMWARE_H_

#include <stdint.h>
#include "sysconfig.h"

#define FW_VER_MAJOR            1
#define FW_VER_MINOR            22

// VHS firmware version (shown at boot). DS_FW_TAG names the image
// (ossc_vhs_vX.Y.bin)
#define DS_VERSION "0.15"
#define DS_FW_TAG               "vhs"

#ifdef OSDLANG_JP
#define FW_SUFFIX              "j"
#else
#define FW_SUFFIX              "-2"   // secondary-slot build
#endif

typedef struct {
    char fw_key[4];
    uint8_t version_major;
    uint8_t version_minor;
    char version_suffix[8];
    uint32_t hdr_len;
    uint32_t data_len;
    uint32_t data_crc;
    char padding[482];
    uint32_t hdr_crc;
} __attribute__((packed)) fw_hdr;


int fw_update(char *dirname, char *filename);

#endif

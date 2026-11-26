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

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "system.h"
#include "firmware.h"
#include "sdcard.h"
#include "flash.h"
#include "controls.h"
#include "tvp7002.h"
#include "av_controller.h"
#include "lcd.h"
#include "utils.h"
#include "menu.h"
#include "ff.h"
#include "file.h"
#include "altera_avalon_pio_regs.h"

extern char menu_row1[LCD_ROW_LEN+1], menu_row2[LCD_ROW_LEN+1];
extern alt_u16 rc_keymap[REMOTE_MAX_KEYS];
extern char tmpbuf[SD_BLK_SIZE];
extern SD_DEV sdcard_dev;
extern alt_u32 sys_ctrl;
extern flash_ctrl_dev flashctrl_dev;

int fw_update(char *dirname, char *filename) {
    // Updates are done from the official (primary) firmware only: this
    // firmware never writes any firmware image (1.0.1: the FPGA remote update
    // block, used to reboot into a new image, removed).
    return -1;
}


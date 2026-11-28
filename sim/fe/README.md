Frontend measurement simulation
===============================

`tb_fe.v`: `rtl/tvp7002_frontend.v` (measurement side, CLK_MEAS_i) with a
composite sync (SOG, active low: hsync, equalizing and broad pulses) and the
TVP7002 vsync output; per field: line count (vtotal), interlace flag, field
period, line period, sync width.

    verilator --binary --timing -Wno-fatal -Wno-lint -Wno-style -Wno-PROCASSWIRE --top-module tb -I../../rtl tb_fe.v ../../rtl/tvp7002_frontend.v -o tbfe
    ./obj_dir/tbfe +PAL=1 +MV=1 +VA=150 +VHS=150 +VJ=8 +NF=24

Plusargs: PAL (1: 625 lines, 0: 525), MV (Macrovision pseudo syncs in lines
7-21 of each field, every 8us), VA / VP (slow drift, clocks / period in
lines), VHS (head switch jump, clocks, last lines before the vsync), VJ
(random jitter per half line), NF (fields).

Field parity (output side)
--------------------------

`tb_fid.v`: the same composite sync plus the regenerated line start (HS
flywheel) on the output side (PCLK_i domain, sync separated mode); the
TVP7002 vsync edge `VSD` clocks after the first broad pulse edge with a random
jitter of +-`VSJ` clocks, `EPE` / `EPO` per mille of even / odd fields with
the vsync edge half a line early (on the last equalizing pulse), an optional
dropped field (`SKIP`: field number, a tape splice), a progressive source
(`PROG=1`, 312 / 262 lines: VCR pause) and `VMISS` / `VEXTRA` per mille of
the fields with the vsync missing / a spurious vsync in the field (vertical
flywheel, VHS 0.11; summary: also missed and extra field starts). Per field start: the parity (FID) and the field start
line against the true ones; summary: alternation errors, placement errors
(wrong parity or start line) and fields read as progressive. `runfid.sh`
runs PAL / NTSC with the vsync edge racing the sync edge (`VSD=0`), well after
it, with the VHS model and with a splice.

    ./runfid.sh

VHS 0.7 and before, vsync edge racing the sync edge (`+VSD=0 +VSJ=4`): 5
fields placed one line off, 19 read as progressive out of 50 (VHS 0.8: 0 / 0).
Vsync edge half a line early in 5% of the even fields (`+EPE=50`): VHS 0.8
2 placed off and 4 progressive reads out of 50, VHS 0.9 0 / 0. A splice: one
field placed off and one progressive read (unavoidable without a frame
memory); a progressive source stays progressive (no even field).

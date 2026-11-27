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
dropped field (`SKIP`: field number, a tape splice) and a progressive source
(`PROG=1`, 312 / 262 lines: VCR pause). Per field start: the parity (FID) and the field start
line against the true ones; summary: alternation errors, placement errors
(wrong parity or start line) and fields read as progressive. `runfid.sh`
runs PAL / NTSC with the vsync edge racing the sync edge (`VSD=0`), well after
it, with the VHS model and with a splice.

    ./runfid.sh

VHS 0.8 / 0.9 changed the frontend parity reading (decision 1/16 line after
the vsync edge; parity following the alternation): 0 errors in every case
here, but no change on the hardware (the field one line off in View is not
the frontend parity reading); reverted in 0.10 for FPGA space. Frontend as in
VHS 0.7, vsync edge racing the sync edge (`+VSD=0 +VSJ=4`): 5 fields placed
one line off, 19 read as progressive out of 50.

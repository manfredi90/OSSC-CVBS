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

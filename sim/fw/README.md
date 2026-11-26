Flywheel simulation
===================

`tb_fw.v`: the hsync flywheel block of `rtl/ossc.v` (copied by hand into the
testbench) with tape-like line timing: slow drift (VA / VP), random line to
line jitter (VJ), missing syncs (MISS: one every n lines), Macrovision pseudo
syncs (MV). Prints accepted / generated edges.

    verilator --binary --timing -Wno-fatal -Wno-lint -Wno-style --top-module tb tb_fw.v -o tbfw
    ./obj_dir/tbfw +VA=150 +VP=200 +VJ=8

Flywheel simulation
===================

`tb_fw.v`: the hsync flywheel block of `rtl/ossc.v` (copied by hand into the
testbench) with tape-like line timing: slow drift (VA / VP), random line to
line jitter (VJ), missing syncs (MISS: one every n lines), Macrovision pseudo
syncs (MV), a vertical interval with half line pulses where the flywheel
starts (HLV). Prints the locked period, accepted / generated edges.

VHS 0.15: `+HLV=1` locks to the line (1727); 0.13 / 0.14 locked to half a line
(863, an edge generated every line): the picture garbled after a setup made
during the vertical interval (VCR pause / play).

    verilator --binary --timing -Wno-fatal -Wno-lint -Wno-style --top-module tb tb_fw.v -o tbfw
    ./obj_dir/tbfw +VA=150 +VP=200 +VJ=8

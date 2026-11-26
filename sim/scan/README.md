Scanconverter timing simulation
===============================

Output sync timing of `rtl/scanconverter.v` with `resync_always` (CVBS 27MHz
crystal clock): input frame starts from a source line period and line count
(optional jitter), output HSYNC / VSYNC / DE edges, then checks of the field
parity (vsync at the line start or half a line), of the lengths of the lines
around each vsync and of the picture lines, and of the resyncs.

    python3 mkcopy.py      # copy of the RTL for the simulators (SV array initializers removed)
    verilator --binary -Wno-fatal -Wno-lint -Wno-style --top-module tb -I../../rtl tb_sc.v sc_sim.v stubs.v -o tbsc
    ./obj_dir/tbsc +NF=10 +LP=439296 +HTOT=1717      # 480i, 1716.0 clocks per line (x256)
    python3 analyze.py 1717

Plusargs: LP (line period x256), LINES (per input frame), NF (frames), JIT
(input frame start jitter, clocks), HTOT / HACT / HSL / HBP, VTOT / VACT / VSL
/ VBP, ILACE, RESYNC. The line buffer, the test pattern and the multipliers
are stubs (no picture data).

Line buffer simulation
----------------------

`tb_lb.v` with a behavioral line buffer (`stubs.v`: 2 lines, 4 for input
lines up to 1024 samples): input lines tagged with their field / line
number (frontend timing: frame change at the start of input line 1, field B
263 lines later), output picture line reads logged to `lb_out.txt`;
`anlb.py` checks that every output line shows one whole input line, in
order (bob: the first line may be the previous field's last one), and the
data age. `runall.sh` runs 15kHz (480i, 240p, 576i, 288p) and Line2x / 3x /
4x cases with real and worst-case line periods.

    python3 mkcopy.py
    verilator --binary --timing -Wno-fatal -Wno-lint -Wno-style --top-module tb -I../../rtl tb_lb.v sc_sim.v stubs.v -o tblb
    NF=24 ./runall.sh

VHS firmware
------------

`runvhs.sh`: exact CEA output line lengths, input with the VHS model of
`tb_lb.v` (plusargs VA / VP: slow drift amplitude in clocks and period in
lines, VHS / VHSL: head switch jump in clocks over the last lines of each
field, VJ: random line start jitter, JIT: frame start jitter, LS: lock step),
480i / 576i in Archive (Line1x) and View (Line2x), 0, +-100 and 300ppm.

    python3 mkcopy.py
    verilator --binary --timing -Wno-fatal -Wno-lint -Wno-style --top-module tb -I../../rtl tb_lb.v sc_sim.v stubs.v -o tblb
    NF=24 ./runvhs.sh

Tape splice: `+SPF=n +SPC=c` (from input frame n the timing jumps by c clocks,
e.g. -270000 = a PAL field dropped).

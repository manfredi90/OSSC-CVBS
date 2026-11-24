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

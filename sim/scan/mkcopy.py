# copy of the scanconverter for iverilog: the unpacked array initializers
# (SystemVerilog '{...} on wires) are replaced with zero values
import re, sys
s = open('../../rtl/scanconverter.v').read()
s = re.sub(r"wire \[3:0\] SL_L_STR\[5:0\] = '\{[^}]*\};", "wire [3:0] SL_L_STR[5:0];", s)
s = re.sub(r"wire \[3:0\] SL_C_STR\[9:0\] = '\{[^}]*\};", "wire [3:0] SL_C_STR[9:0];", s)
s = s.replace("wire bfi_frame;", "reg bfi_frame;")
open("sc_sim.v", "w").write(s)

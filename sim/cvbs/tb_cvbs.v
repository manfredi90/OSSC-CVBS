`timescale 1ns/1ps
module tb;
reg clk = 0;
always #18.5 clk = ~clk;
reg [7:0] G = 0, B = 0;
reg HS = 1, VS = 1;
reg [23:0] cfg = 0;
reg [31:0] hc = 0, inc = 0;
wire [7:0] Ro, Go, Bo;
wire [1:0] side_o;
wire [31:0] status;
cvbs_decoder #(.SIDE_W(2)) dut (
    .clk(clk), .R_i(8'h80), .G_i(G), .B_i(B), .HS_i(HS), .VS_i(VS),
    .side_i({HS, VS}), .cfg(cfg), .hc(hc), .inc(inc),
    .R_o(Ro), .G_o(Go), .B_o(Bo), .side_o(side_o), .status(status));
integer fin, fout, fdbg, r, n;
`ifdef K_DBG
always @(posedge clk) if (dut.st_line == 9'd100) $fwrite(fdbg, "%0d %0d %0d %0d %0d\n", dut.h, dut.yA, dut.yE, dut.yB, dut.R_o);
`endif
`ifdef Z_DBG
always @(posedge clk) if (dut.st_line == 9'd100) $fwrite(fdbg, "%0d %0d %0d %0d %0d\n", dut.h, dut.zI0, dut.zQ0, dut.zIo, dut.zQo);
`endif
`ifdef SE_DBG
always @(posedge clk) if (dut.h == 12'd301) $fwrite(fdbg, "%0d %0d %0d %0d %0d\n", dut.se_k, dut.se_q, dut.se_e, dut.se_d, dut.se_found);
`endif
reg [31:0] gv, bv, hv, vv;
initial begin
    fin = $fopen("in.txt", "r");
    fout = $fopen("out.txt", "w");
    fdbg = $fopen("dbg.txt", "w");
    r = $fscanf(fin, "%h %h %h\n", cfg, hc, inc);
    n = 0;
    while (!$feof(fin)) begin
        r = $fscanf(fin, "%h %h %h %h\n", gv, bv, hv, vv);
        @(negedge clk);
        G = gv; B = bv; HS = hv; VS = vv;
        @(posedge clk); #1;
        $fwrite(fout, "%02x %02x %02x %1x %08x\n", Ro, Go, Bo, side_o, status);
        n = n + 1;
    end
    $fclose(fout);
    $finish;
end
endmodule

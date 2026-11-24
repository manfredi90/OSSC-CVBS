// Scanconverter + behavioral 2-line buffer with resync_always: input lines
// (from the source line period, frontend timing: frame change at the start of
// input line 1, interlaced field B 263 lines later) tagged with field / line
// numbers; the tags read by the output picture lines are logged to lb_out.txt
// (first / middle / last pixel, age of the data in input clocks).
// Input clock period 24, output 24/OM (OM = 1, 1.5, 2: Line3x / Line4x PLL).
`timescale 1ns/1ps
module tb;
integer OMX2;     // output clock x2 / input clock
reg clki = 0, clko = 0;
integer HI, HO;
initial begin
    if (!$value$plusargs("OMX2=%d", OMX2)) OMX2 = 2;
    HI = 12; HO = 24 / OMX2;
end
always #(HI) clki = ~clki;
always #(HO) clko = ~clko;

integer JIT, LP_X256, NFRAMES, HTOT, HACT, HSL, HBP, VTOT, VACT, VSL, VBP, ILACE, YRPT, IHSL, IHBP, IHACT, IHSKIP, IVSL, IVBP, IVACT, IPROG;
wire [31:0] hv_out_config  = {HSL[7:0], HACT[11:0], HTOT[11:0]};
wire [31:0] hv_out_config2 = {ILACE[0], VACT[10:0], VTOT[10:0], HBP[8:0]};
integer VSTART;
wire [31:0] hv_out_config3 = {8'h0, VSTART[10:0], VBP[8:0], VSL[3:0]};
wire [31:0] hv_ext_config  = {26'h0, HBP[9], 2'b00, HSL[8], HACT[12], HTOT[12]};
wire [31:0] xy_out_config  = {9'h0, VACT[10:0], HACT[11:0]};
wire [31:0] xy_out_config2 = {YRPT[3:0], 4'h0, 24'h0};
wire HS, VS, CS, DE;
wire [7:0] rcnt;
reg fc = 0;
reg [7:0] Ri = 0, Gi = 0, Bi = 0;
reg DEi = 0;
reg [10:0] xi = 0, yi = 0;

scanconverter dut (
    .PCLK_CAP_i(clki), .PCLK_OUT_i(clko), .reset_n(1'b1),
    .R_i(Ri), .G_i(Gi), .B_i(Bi), .HSYNC_i(1'b1), .VSYNC_i(1'b1), .DE_i(DEi), .FID_i(1'b0), .datavalid_i(1'b1),
    .interlaced_in_i(~IPROG[0]), .frame_change_i(fc), .xpos_i(xi), .ypos_i(yi), .h_in_active(IHACT[11:0]),
    .hv_out_config(hv_out_config), .hv_out_config2(hv_out_config2), .hv_out_config3(hv_out_config3),
    .xy_out_config(xy_out_config), .xy_out_config2(xy_out_config2), .hv_ext_config(hv_ext_config), .misc_config(32'h0),
    .sl_config(32'h0), .sl_config2(32'h0), .sl_config3(32'h0), .testpattern_enable(1'b0), .lb_enable(1'b1),
    .ds_vdec(3'h0), .lb_layout(2'h0), .ds_hfilt(2'h0), .ds_field_cfg(10'h0), .ds_ilace(1'b0), .ds_fswap(1'b0), .ds_i2p(1'b0),
    .ds_vflt(1'b0), .ds_vflt_bofs(2'h0), .ds_vflt_a(5'h0), .ds_w31(1'b0), .lb_stride(10'h0), .lb_slots(6'h0), .lb_rgb(1'b0),
    .ds_hofs(13'h0), .resync_always(1'b1), .ext_sync_mode(1'b0), .ext_frame_change_i(1'b0),
    .ext_R_i(8'h0), .ext_G_i(8'h0), .ext_B_i(8'h0),
    .HSYNC_o(HS), .VSYNC_o(VS), .CSYNC_o(CS), .DE_o(DE), .shmask_data(12'h0), .ds_resync_cnt(rcnt),
    .lt_active(1'b0), .lt_mode(2'h0), .emif_br_clk(1'b0), .emif_br_reset(1'b0),
    .emif_rd_rdata(256'h0), .emif_rd_waitrequest(1'b0), .emif_rd_readdatavalid(1'b0), .emif_wr_waitrequest(1'b0)
);

integer f, g, ti = 0, to = 0, frame = 0, fld = 0;
reg [63:0] acc, lstart;      // x256: next frame start, current line start
integer v, lines_a, lines_f, k;
initial begin
    if (!$value$plusargs("LP=%d", LP_X256)) LP_X256 = 1716*256;
    if (!$value$plusargs("NF=%d", NFRAMES)) NFRAMES = 12;
    if (!$value$plusargs("VSTART=%d", VSTART)) VSTART = 0;
    if (!$value$plusargs("JIT=%d", JIT)) JIT = 0;
    if (!$value$plusargs("HTOT=%d", HTOT)) HTOT = 1716;
    if (!$value$plusargs("HACT=%d", HACT)) HACT = 1440;
    if (!$value$plusargs("HSL=%d", HSL)) HSL = 124;
    if (!$value$plusargs("HBP=%d", HBP)) HBP = 114;
    if (!$value$plusargs("VTOT=%d", VTOT)) VTOT = 525;
    if (!$value$plusargs("VACT=%d", VACT)) VACT = 240;
    if (!$value$plusargs("VSL=%d", VSL)) VSL = 3;
    if (!$value$plusargs("VBP=%d", VBP)) VBP = 15;
    if (!$value$plusargs("ILACE=%d", ILACE)) ILACE = 1;
    if (!$value$plusargs("YRPT=%d", YRPT)) YRPT = 0;
    if (!$value$plusargs("IHSL=%d", IHSL)) IHSL = 124;     // input timings (samples)
    if (!$value$plusargs("IHBP=%d", IHBP)) IHBP = 114;
    if (!$value$plusargs("IHACT=%d", IHACT)) IHACT = 1440;
    if (!$value$plusargs("IHSKIP=%d", IHSKIP)) IHSKIP = 0;
    if (!$value$plusargs("IVSL=%d", IVSL)) IVSL = 3;
    if (!$value$plusargs("IVBP=%d", IVBP)) IVBP = 15;
    if (!$value$plusargs("IVACT=%d", IVACT)) IVACT = 240;
    if (!$value$plusargs("IPROG=%d", IPROG)) IPROG = 0;
    f = $fopen("lb_out.txt", "w");
    g = $fopen("sc_out.txt", "w");
    dut.h_cnt = 0; dut.v_cnt = 0; dut.dst_fid = 0; dut.src_fid = 0; dut.fl_drop = 0; dut.fl_ins = 0;
    dut.frame_change_sync1_reg = 0; dut.frame_change_sync2_reg = 0; dut.frame_change_prev = 0; dut.frame_change_resync = 0;
    dut.in_field = 0; dut.ds_resync_cnt = 0; dut.fl_cap = 0; dut.resync_strobe = 0;
    lines_f = IPROG ? 262 : 525;
    if (!$value$plusargs("LINES=%d", lines_f)) ;
    lines_a = IPROG ? lines_f : (lines_f + 1) / 2;    // field A lines (frame change at its line 1)
    acc = 64'd1000 << 8; lstart = acc; v = 0;
end
// input side: line v (1..) from the frame start, field A / B
integer h, vv, fb, hs;
always @(posedge clki) begin
    ti <= ti + 1;
    if ((ti << 8) >= acc) begin
        fc <= 1'b1;
        lstart <= acc; v <= 1; fld <= fld + (IPROG ? 1 : 2);
        acc <= acc + lines_f * LP_X256 + ((JIT > 0) ? (($urandom % (2*JIT+1)) - JIT) << 8 : 0);
        frame <= frame + 1;
        $fwrite(g, "F %0d\n", ti);
        if (frame == NFRAMES) begin $fclose(f); $fclose(g); $finish; end
    end else begin
        if ((ti << 8) >= lstart + 16*256) fc <= 1'b0;
        if ((ti << 8) >= lstart + LP_X256) begin lstart <= lstart + LP_X256; v <= v + 1; end
    end
    // position in the line (frontend: DE / x / y 4 clocks after the line start)
    h = ti - 4 - (lstart >> 8);
    fb = (!IPROG && (v > lines_a)) ? 1 : 0;
    vv = fb ? v - lines_a : v;
    hs = h / (IHSKIP + 1) - IHSL - IHBP;
    yi <= vv - IVSL - IVBP;
    if (h >= 0 && hs >= 0 && hs < IHACT && vv >= IVSL + IVBP && vv < IVSL + IVBP + IVACT) begin
        DEi <= 1; xi <= hs;
        {Ri, Gi, Bi} <= (((fld - 2 + fb + (IPROG ? 1 : 0)) & 8191) << 11) | (vv - IVSL - IVBP);
    end else DEi <= 0;
end
// output side: picture line reads (first / middle / last pixel)
integer HLB, a;
reg [11:0] xp = 0;
reg HSp = 1, VSp = 1, DEp = 0;
reg [7:0] rp = 0;
always @(posedge clko) begin
    to <= to + 1;
    HLB = HACT / 1;
    xp <= dut.xpos_lb;
    if ((dut.xpos_lb != xp) && ((xp == 0) || (xp == HLB/2) || (xp == HLB-1)) && (dut.v_cnt >= VSL+VBP) && (dut.v_cnt < VSL+VBP+VACT)) begin
        a = (IHACT <= 1024) ? {dut.ypos_lb[1:0], xp[9:0]} : {dut.ypos_lb[0], xp[10:0]};
        $fwrite(f, "%0d %0d %0d %0d %0d %0d %0d\n", to, dut.dst_fid, dut.v_cnt, xp, dut.linebuf_top_inst.mem[a] >> 11, dut.linebuf_top_inst.mem[a] & 2047,
                dut.linebuf_top_inst.tcap - dut.linebuf_top_inst.wt[a]);
    end
    HSp <= HS; VSp <= VS; DEp <= DE; rp <= rcnt;
    if (HSp & ~HS) $fwrite(g, "H %0d\n", to);
    if (VSp & ~VS) $fwrite(g, "V %0d\n", to);
    if (~DEp & DE) $fwrite(g, "D %0d\n", to);
    if (rp != rcnt) $fwrite(g, "R %0d\n", to);
end
endmodule

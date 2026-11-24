// Scanconverter output timing with resync_always (CVBS 27MHz crystal clock):
// input frame starts from the source line period (x256) and line count,
// output H / V / C sync and DE edges written to sc_out.txt
`timescale 1ns/1ps
module tb;
reg clk = 0;
always #18.5 clk = ~clk;
reg fc = 0;

// parameters (plusargs)
integer JIT, LP_X256, LINES, NFRAMES, HTOT, HACT, HSL, HBP, VTOT, VACT, VSL, VBP, ILACE, RESYNC;
wire [31:0] hv_out_config  = {HSL[7:0], HACT[11:0], HTOT[11:0]};
wire [31:0] hv_out_config2 = {ILACE[0], VACT[10:0], VTOT[10:0], HBP[8:0]};
wire [31:0] hv_out_config3 = {8'h0, 11'd0, VBP[8:0], VSL[3:0]};
wire [31:0] hv_ext_config  = {26'h0, HBP[9], 2'b00, HSL[8], HACT[12], HTOT[12]};
wire HS, VS, CS, DE;
wire [7:0] rcnt;

scanconverter dut (
    .PCLK_CAP_i(clk), .PCLK_OUT_i(clk), .reset_n(1'b1),
    .R_i(8'h0), .G_i(8'h0), .B_i(8'h0), .HSYNC_i(1'b1), .VSYNC_i(1'b1), .DE_i(1'b0), .FID_i(1'b0), .datavalid_i(1'b0),
    .interlaced_in_i(ILACE[0]), .frame_change_i(fc), .xpos_i(11'h0), .ypos_i(11'h0), .h_in_active(12'd1440),
    .hv_out_config(hv_out_config), .hv_out_config2(hv_out_config2), .hv_out_config3(hv_out_config3),
    .xy_out_config(32'h0), .xy_out_config2(32'h0), .hv_ext_config(hv_ext_config), .misc_config(32'h0),
    .sl_config(32'h0), .sl_config2(32'h0), .sl_config3(32'h0), .testpattern_enable(1'b0), .lb_enable(1'b1),
    .ds_vdec(3'h0), .lb_layout(2'h0), .ds_hfilt(2'h0), .ds_field_cfg(10'h0), .ds_ilace(1'b0), .ds_fswap(1'b0), .ds_i2p(1'b0),
    .ds_vflt(1'b0), .ds_vflt_bofs(2'h0), .ds_vflt_a(5'h0), .ds_w31(1'b0), .lb_stride(10'h0), .lb_slots(6'h0), .lb_rgb(1'b0),
    .ds_hofs(13'h0), .resync_always(RESYNC[0]), .ext_sync_mode(1'b0), .ext_frame_change_i(1'b0),
    .ext_R_i(8'h0), .ext_G_i(8'h0), .ext_B_i(8'h0),
    .HSYNC_o(HS), .VSYNC_o(VS), .CSYNC_o(CS), .DE_o(DE), .shmask_data(12'h0), .ds_resync_cnt(rcnt),
    .lt_active(1'b0), .lt_mode(2'h0), .emif_br_clk(1'b0), .emif_br_reset(1'b0),
    .emif_rd_rdata(256'h0), .emif_rd_waitrequest(1'b0), .emif_rd_readdatavalid(1'b0), .emif_wr_waitrequest(1'b0)
);

integer f, t = 0, frame = 0;
reg [63:0] acc;         // input time x256
reg HSp = 1, VSp = 1, CSp = 1, DEp = 0;
reg [7:0] rp = 0;
initial begin
    if (!$value$plusargs("LP=%d", LP_X256)) LP_X256 = 1716*256;
    if (!$value$plusargs("LINES=%d", LINES)) LINES = 525;
    if (!$value$plusargs("NF=%d", NFRAMES)) NFRAMES = 12;
    if (!$value$plusargs("JIT=%d", JIT)) JIT = 0;
    if (!$value$plusargs("HTOT=%d", HTOT)) HTOT = 1717;
    if (!$value$plusargs("HACT=%d", HACT)) HACT = 1440;
    if (!$value$plusargs("HSL=%d", HSL)) HSL = 124;
    if (!$value$plusargs("HBP=%d", HBP)) HBP = 114;
    if (!$value$plusargs("VTOT=%d", VTOT)) VTOT = 525;
    if (!$value$plusargs("VACT=%d", VACT)) VACT = 240;
    if (!$value$plusargs("VSL=%d", VSL)) VSL = 3;
    if (!$value$plusargs("VBP=%d", VBP)) VBP = 15;
    if (!$value$plusargs("ILACE=%d", ILACE)) ILACE = 1;
    if (!$value$plusargs("RESYNC=%d", RESYNC)) RESYNC = 1;
    f = $fopen("sc_out.txt", "w");
    // power-up state (registers start at 0 in the FPGA)
    dut.h_cnt = 0; dut.v_cnt = 0; dut.dst_fid = 0; dut.src_fid = 0; dut.sl_lag = 0; dut.sl_lag2 = 0; dut.sl_cut = 0;
    dut.frame_change_sync1_reg = 0; dut.frame_change_sync2_reg = 0; dut.frame_change_prev = 0; dut.frame_change_resync = 0;
    dut.in_field = 0; dut.ds_resync_cnt = 0; dut.sl_cap = 0; dut.resync_strobe = 0;
    acc = 64'd1000 << 8;
end
always @(posedge clk) begin
    t <= t + 1;
    // input frame start every LINES * LP clocks (fractional), pulse 16 clocks
    if ((t << 8) >= acc) begin
        fc <= 1'b1;
        acc <= acc + LINES * LP_X256 + ((JIT > 0) ? (($urandom % (2*JIT+1)) - JIT) << 8 : 0);
        frame <= frame + 1;
        $fwrite(f, "F %0d\n", t);
        if (frame == NFRAMES) begin $fclose(f); $finish; end
    end else if ((t << 8) >= acc - (LINES * LP_X256) + (16 << 8))
        fc <= 1'b0;
    HSp <= HS; VSp <= VS; CSp <= CS; DEp <= DE; rp <= rcnt;
    if (HSp & ~HS) $fwrite(f, "H %0d\n", t);
    if (VSp & ~VS) $fwrite(f, "V %0d\n", t);
    if (~DEp & DE) $fwrite(f, "D %0d\n", t);
    if (rp != rcnt) $fwrite(f, "R %0d\n", t);
end
endmodule

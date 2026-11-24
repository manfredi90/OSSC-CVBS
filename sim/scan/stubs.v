// stubs for the scanconverter timing simulation (no picture data)
module linebuf_top #(parameter EMIF_ENABLE=0, parameter NUM_LINE_BUFFERS=2) (
    input PCLK_CAP_i, input PCLK_OUT_i, input [7:0] R_i, input [7:0] G_i, input [7:0] B_i, input DE_i, input datavalid_i,
    input [11:0] h_in_active, input [10:0] xpos_i, input [10:0] ypos_i, input [11:0] xpos_lb, input [10:0] ypos_lb, input [10:0] ypos_lb_next,
    input [2:0] line_id, input lb_enable, input [2:0] ds_vdec, input [10:0] ds_vstart, input [1:0] lb_layout, input [1:0] ds_hfilt,
    input [9:0] ds_field_cfg, input ds_ilace, input vflt, input [1:0] vflt_bofs, input [4:0] vflt_a, input w31, input in_field,
    input [9:0] lb_stride, input [5:0] lb_slots, input lb_rgb, input [10:0] lb_rd_base,
    output [7:0] R_linebuf, output [7:0] G_linebuf, output [7:0] B_linebuf,
    input emif_br_clk, input emif_br_reset, output [27:0] emif_rd_addr, output emif_rd_read, input [255:0] emif_rd_rdata,
    input emif_rd_waitrequest, input emif_rd_readdatavalid, output [5:0] emif_rd_burstcount,
    output [27:0] emif_wr_addr, output emif_wr_write, output [255:0] emif_wr_wdata, input emif_wr_waitrequest, output [5:0] emif_wr_burstcount);
// behavioral 2-line buffer (layout 0): the tag of the input line in R/G/B,
// write time per pixel (write pipeline: WDLY clocks)
localparam WDLY = 8;
reg [23:0] mem [0:4095];
integer wt [0:4095];
reg [23:0] q;
reg [10:0] yprev = 0;
reg [1:0] slot = 0;
wire lb4 = (h_in_active <= 12'd1024);
integer tcap = 0;
reg [23:0] d_tag [0:WDLY]; reg [11:0] d_adr [0:WDLY]; reg d_we [0:WDLY];
integer i;
always @(posedge PCLK_CAP_i) begin
    tcap <= tcap + 1;
    if (ypos_i != yprev) slot <= (ypos_i == 0) ? 2'd0 : slot + 1'b1;
    yprev <= ypos_i;
    d_tag[0] <= {R_i, G_i, B_i}; begin : wa reg [1:0] sn; sn = (ypos_i == 0) ? 2'd0 : ((ypos_i != yprev) ? slot + 1'b1 : slot); d_adr[0] <= lb4 ? {sn, xpos_i[9:0]} : {sn[0], xpos_i}; end d_we[0] <= DE_i & datavalid_i;
    for (i = 1; i <= WDLY; i = i + 1) begin d_tag[i] <= d_tag[i-1]; d_adr[i] <= d_adr[i-1]; d_we[i] <= d_we[i-1]; end
    if (d_we[WDLY]) begin mem[d_adr[WDLY]] <= d_tag[WDLY]; wt[d_adr[WDLY]] <= tcap; end
end
always @(posedge PCLK_OUT_i) q <= mem[lb4 ? {ypos_lb[1:0], xpos_lb[9:0]} : {ypos_lb[0], xpos_lb[10:0]}];
assign {R_linebuf, G_linebuf, B_linebuf} = q;
assign emif_rd_addr = 0; assign emif_rd_read = 0; assign emif_rd_burstcount = 0;
assign emif_wr_addr = 0; assign emif_wr_write = 0; assign emif_wr_wdata = 0; assign emif_wr_burstcount = 0;
endmodule
module videogen (input pclk, input [10:0] xpos, input [10:0] ypos, input [1:0] tp_sel, output [7:0] R_out, output [7:0] G_out, output [7:0] B_out);
assign R_out = 0; assign G_out = 0; assign B_out = 0;
endmodule
module lpm_mult_8x5_9 (input clock, input [7:0] dataa, input [4:0] datab, output [8:0] result);
assign result = 0;
endmodule
module lpm_mult_sl (input clock, input [7:0] dataa, input [7:0] datab, output [7:0] result);
assign result = 0;
endmodule

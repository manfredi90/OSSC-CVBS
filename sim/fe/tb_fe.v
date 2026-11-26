// Frontend measurement (tvp7002_frontend.v, CLK_MEAS_i domain): line count,
// interlace flag, field period, line period from a composite sync (SOG,
// active low) and the TVP7002 vsync output, PAL (625) or NTSC (525) with
// equalizing / broad pulses, optional Macrovision pseudo-sync pulses in the
// vertical blanking lines and the VHS model (slow drift, head switch, line
// jitter). Per field: vtotal, interlace_flag, pcnt_field, pcnt_line,
// hsync_width.
`timescale 1ns/1ps
module tb;
reg clk = 0;
always #18.5 clk = ~clk;     // 27MHz

integer PAL, MV, NF, VA, VP, VHS, VJ;
integer HL, FHL, NEQ, LINE;          // half line (clocks), half lines per field, equalizing / broad half lines
initial begin
    if (!$value$plusargs("PAL=%d", PAL)) PAL = 1;
    if (!$value$plusargs("MV=%d", MV)) MV = 0;            // Macrovision pseudo syncs
    if (!$value$plusargs("NF=%d", NF)) NF = 16;
    if (!$value$plusargs("VA=%d", VA)) VA = 0;
    if (!$value$plusargs("VP=%d", VP)) VP = 200;
    if (!$value$plusargs("VHS=%d", VHS)) VHS = 0;
    if (!$value$plusargs("VJ=%d", VJ)) VJ = 0;
    LINE = PAL ? 1728 : 1716;
    HL = LINE / 2;
    FHL = PAL ? 625 : 525;
    NEQ = PAL ? 5 : 6;
end

// composite sync generator: half line k (absolute), field = k / FHL,
// field relative half line f; line starts at even k
reg sog = 1, vs = 1;
integer k = 0, t0, tn, f, fld = 0, ln, off, w, pmv, i;
real ph;
integer pos [0:15]; integer npos;
integer tc = 0;
always @(posedge clk) tc <= tc + 1;

task automatic pulse(input integer at, input integer width);
    begin
        // (scheduled below)
    end
endtask

// event list per half line: list of (start, width) low pulses
initial begin : gen
    integer start, base, j, p;
    integer hs_w, eq_w, br_w;
    hs_w = 127; eq_w = 63; br_w = HL - 127;
    wait (tc == 100);
    base = 100;
    for (k = 0; k < NF * FHL; k = k + 1) begin
        f = k % FHL;
        fld = k / FHL;
        ln = (k % (2*FHL)) / 2;
        ph = VA * $sin(6.283185307 * (k/2) / VP);
        off = $rtoi(ph);
        if (VJ > 0) off = off + ($urandom % (2*VJ+1)) - VJ;
        if ((VHS != 0) && (f >= FHL - NEQ - 12) && (f < FHL - NEQ)) off = off + VHS;
        start = base + k * HL + off;
        // pulses of this half line
        npos = 0;
        if (f < NEQ) begin
            pos[0] = 0; w = br_w; npos = 1;                     // broad
        end else if ((f < 2*NEQ) || (f >= FHL - NEQ)) begin
            pos[0] = 0; w = eq_w; npos = 1;                     // equalizing
        end else if ((k % 2) == 0) begin
            pos[0] = 0; w = hs_w; npos = 1;                     // hsync (line start)
        end
        // TVP7002 vsync: low from 20 clocks into the first broad pulse, 3 lines
        if (f == 0) fork begin : vsg #((start - tc + 20) * 37); vs = 0; #(6 * HL * 37); vs = 1; end join_none
        // wait for the half line start and draw the pulse
        while (tc < start) @(posedge clk);
        if (npos) begin sog = 0; repeat (w) @(posedge clk); sog = 1; end
        // Macrovision: pseudo syncs (2.3us) every 8us from 10us in lines 7-21 of each field
        if (MV && ((k % 2) == 0) && (f >= 2*NEQ + 2) && (f < 2*NEQ + 2 + 30)) begin
            for (p = 270; p < HL - 100; p = p + 216) begin
                while (tc < start + p) @(posedge clk);
                sog = 0; repeat (62) @(posedge clk); sog = 1;
            end
        end else if (MV && ((k % 2) == 1) && (f >= 2*NEQ + 2) && (f < 2*NEQ + 2 + 30)) begin
            for (p = 20; p < HL - 100; p = p + 216) begin
                while (tc < start + p) @(posedge clk);
                sog = 0; repeat (62) @(posedge clk); sog = 1;
            end
        end
    end
    $finish;
end

wire interlace_flag, frame_change, sof_scaler, sync_active;
wire [10:0] vtotal;
wire [19:0] pcnt_field;
wire [7:0] hsync_width;
reg [2:0] hs_s = 3'b111, vs_s = 3'b111;
always @(posedge clk) begin hs_s <= {hs_s[1:0], sog}; vs_s <= {vs_s[1:0], vs}; end

tvp7002_frontend dut (
    .PCLK_i(clk), .CLK_MEAS_i(clk), .reset_n(1'b1), .R_i(8'h0), .G_i(8'h0), .B_i(8'h0),
    .HS_i(1'b1), .VS_i(1'b1), .HSYNC_i(hs_s[2]), .VSYNC_i(vs_s[2]), .DE_i(1'b0), .FID_i(1'b0),
    .sogref_update_i(1'b0), .vsync_i_type(1'b0),
    .hv_in_config({8'd127, 12'd1440, LINE[11:0]}), .hv_in_config2(32'h0), .hv_in_config3(32'h0),
    .misc_config(32'h0), .lumacode_data(32'h0),
    .interlace_flag(interlace_flag), .vtotal(vtotal), .frame_change(frame_change), .sof_scaler(sof_scaler),
    .pcnt_field(pcnt_field), .hsync_width(hsync_width), .sync_active(sync_active)
);

// report at every TVP vsync (after the field)
reg vsp = 1;
always @(posedge clk) begin
    vsp <= vs_s[2];
    if (vsp & ~vs_s[2])
        $display("F vtotal %0d ilace %0d pcnt_field %0d pcnt_line %0d hsw %0d", vtotal, interlace_flag, pcnt_field, dut.pcnt_line, hsync_width);
end
endmodule

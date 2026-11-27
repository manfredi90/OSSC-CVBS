// Frontend field parity (tvp7002_frontend.v, PCLK_i side, sync separated
// mode as with the CVBS decoder): composite sync (SOG) with equalizing /
// broad pulses, the regenerated line start (HS flywheel: line starts only),
// the TVP7002 vsync edge VSD clocks after the first broad pulse edge with a
// random jitter of +-VSJ clocks (VSD near 0: the vsync edge races the SOG
// edge; EPE / EPO: per mille of even / odd fields with the vsync edge half
// a line early, on the last equalizing pulse), VHS model (drift, head switch jump, jitter) and an optional dropped
// field (SKIP: field number, a tape splice). Per field start: FID, field
// start line vs the expected one, alternation errors counted.
`timescale 1ns/1ps
module tb;
reg clk = 0;
always #18.5 clk = ~clk;     // 27MHz

integer PAL, NF, VA, VP, VHS, VJ, VSD, VSJ, SKIP, V, EPE, EPO, PROG;
integer HL, FHL, NEQ, LINE;
initial begin
    if (!$value$plusargs("PAL=%d", PAL)) PAL = 1;
    if (!$value$plusargs("NF=%d", NF)) NF = 40;
    if (!$value$plusargs("VA=%d", VA)) VA = 0;
    if (!$value$plusargs("VP=%d", VP)) VP = 200;
    if (!$value$plusargs("VHS=%d", VHS)) VHS = 0;
    if (!$value$plusargs("VJ=%d", VJ)) VJ = 0;
    if (!$value$plusargs("VSD=%d", VSD)) VSD = 20;
    if (!$value$plusargs("VSJ=%d", VSJ)) VSJ = 0;
    if (!$value$plusargs("SKIP=%d", SKIP)) SKIP = -1;
    if (!$value$plusargs("V=%d", V)) V = 0;
    if (!$value$plusargs("EPE=%d", EPE)) EPE = 0;   // per mille: vsync half a line early, even fields
    if (!$value$plusargs("EPO=%d", EPO)) EPO = 0;   // odd fields
    if (!$value$plusargs("PROG=%d", PROG)) PROG = 0; // progressive source (312 / 262 lines, VCR pause)
    LINE = PAL ? 1728 : 1716;
    HL = LINE / 2;
    FHL = PAL ? 625 : 525;
    if (PROG) FHL = FHL - 1;
    NEQ = PAL ? 5 : 6;
end

reg sog = 1, vs = 1, hs = 1;
integer vs_at = -100000, hs_at = -100000;
always @(posedge clk) begin
    vs <= ~((tc >= vs_at) && (tc < vs_at + 6*HL));
    hs <= ~((tc >= hs_at) && (tc < hs_at + 32));
end
integer tc = 0;
always @(posedge clk) tc <= tc + 1;

// true field starts (time of the first broad pulse) and the field parity:
// odd = the broad pulses start at a line start
integer fstart [0:255]; integer fpar [0:255]; integer nfld = 0;

initial begin : gen
    integer k, kk, f, start, base, j, w, off, d, dropped;
    real ph;
    integer hs_w, eq_w, br_w;
    hs_w = 127; eq_w = 63; br_w = HL - 127;
    wait (tc == 100);
    base = 100; dropped = 0;
    for (kk = 0; kk < NF * FHL; kk = kk + 1) begin
        // dropped field: the source jumps one field ahead, time goes on
        if ((SKIP >= 0) && (kk == SKIP * FHL) && !dropped) begin dropped = 1; base = base - FHL * HL; end
        k = kk + (dropped ? FHL : 0);
        f = k % FHL;
        ph = VA * $sin(6.283185307 * (k/2) / VP);
        off = $rtoi(ph);
        if (VJ > 0) off = off + ($urandom % (2*VJ+1)) - VJ;
        if ((VHS != 0) && (f >= FHL - NEQ - 12) && (f < FHL - NEQ)) off = off + VHS;
        start = base + kk * HL + off;
        w = 0;
        if (f < NEQ) w = br_w;
        else if ((f < 2*NEQ) || (f >= FHL - NEQ)) w = eq_w;
        else if ((k % 2) == 0) w = hs_w;
        if (f == 0) begin
            fstart[nfld] = start; fpar[nfld] = ((k % 2) == 0); nfld = nfld + 1;
            d = VSD + ((VSJ > 0) ? (($urandom % (2*VSJ+1)) - VSJ) : 0);
            if (((k % 2) ? EPE : EPO) > ($urandom % 1000)) d = d - HL;
            vs_at = start + d;
        end
        while (tc < start) @(posedge clk);
        // regenerated line start (flywheel): every line start, 32 clocks low
        if ((k % 2) == 0) hs_at = start;
        if (w) begin sog = 0; repeat (w) @(posedge clk); sog = 1; end
    end
    #1000;
    $display("SUMMARY fields %0d alternation_errors %0d placement_errors %0d progressive_reads %0d even_fids %0d", nf_seen, n_alt, n_pos, n_prog, n_even);
    $finish;
end

reg [2:0] sog_s = 3'b111, vs_s = 3'b111;
reg sog_prev = 1;
reg hs_r = 1;
always @(posedge clk) begin
    sog_s <= {sog_s[1:0], sog}; vs_s <= {vs_s[1:0], vs};
    sog_prev <= sog_s[2];
    hs_r <= hs;
end

wire interlace_flag, frame_change, sof_scaler, sync_active, FID_o;
wire [10:0] vtotal;
wire [19:0] pcnt_field;
wire [7:0] hsync_width;
tvp7002_frontend dut (
    .PCLK_i(clk), .CLK_MEAS_i(clk), .reset_n(1'b1), .R_i(8'h0), .G_i(8'h0), .B_i(8'h0),
    .HS_i(hs_r), .VS_i(vs_s[2]), .HSYNC_i(sog_s[2]), .VSYNC_i(vs_s[2]), .DE_i(1'b0), .FID_i(1'b0),
    .sogref_update_i(sog_prev & ~sog_s[2]), .vsync_i_type(1'b0),
    .hv_in_config({8'd127, 12'd1440, LINE[11:0]}), .hv_in_config2(32'h0), .hv_in_config3(32'h4),
    .misc_config(32'h0), .lumacode_data(32'h0), .FID_o(FID_o),
    .interlace_flag(interlace_flag), .vtotal(vtotal), .frame_change(frame_change), .sof_scaler(sof_scaler),
    .pcnt_field(pcnt_field), .hsync_width(hsync_width), .sync_active(sync_active)
);

// field start in the frontend: HS edge with fid_next_ctr == 1 (v_cnt <= 1)
integer n_prog = 0, n_even = 0;
integer nf_seen = 0, n_alt = 0, n_pos = 0, last_fid = -1, fi, dl, exp_dl, last_true = -1;
always @(posedge clk) begin
    if (dut.HS_i_prev & ~dut.HS_i & (dut.fid_next_ctr == 2'h1)) begin
        // the true field this start belongs to: the last one begun
        fi = nfld - 1;
        dl = (tc - fstart[fi]);
        // expected: odd (broad at line start) starts 1 line after, even 1.5
        exp_dl = fpar[fi] ? LINE : LINE + HL;
        if (nf_seen >= 6 && !interlace_flag) n_prog = n_prog + 1;
        if (nf_seen >= 6 && (dut.fid_next == 0)) n_even = n_even + 1;
        if (nf_seen >= 6 && interlace_flag) begin
            if ((last_fid >= 0) && (dut.fid_next == last_fid) && (fpar[fi] != last_true)) n_alt = n_alt + 1;
            if ((dut.fid_next != fpar[fi]) || (dl < exp_dl - HL/2) || (dl > exp_dl + HL/2)) n_pos = n_pos + 1;
        end
        if (V || (nf_seen >= 6 && interlace_flag && ((dut.fid_next != fpar[fi]) || (dl < exp_dl - HL/2) || (dl > exp_dl + HL/2))))
            $display("field %0d true_par %0d fid %0d start %0d (exp %0d) ilace %0d", fi, fpar[fi], dut.fid_next, dl, exp_dl, interlace_flag);
        last_fid = dut.fid_next; last_true = fpar[fi];
        nf_seen = nf_seen + 1;
    end
end
endmodule

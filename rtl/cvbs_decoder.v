//
// Composite / S-Video colour decoder for the OSSC (PAL and NTSC)
//
// This file is part of the OSSC CVBS firmware, a modified version of the
// Open Source Scan Converter project. GPLv3 or later (see LICENSE).
//
// The TVP7002 samples the input line-locked (one sample per clock, ~27MHz:
// 1716 samples per 525-line line, 1728 per 625-line line). Composite (or the
// luma of S-Video) comes on the G channel, the S-Video chroma on the B channel.
//
// Chroma: band-pass, quadrature demodulation with a free running DDS
// (increment from the CPU, from the measured line rate and the nominal
// subcarrier frequency), low-pass (cascaded box filters 6/7/8), then per line:
//  - burst vector Zb (sum of the demodulated chroma in the burst gate)
//  - PAL switch from the sign of Im(Zb(n) * conj(Zb(n-1))) (consecutive PAL
//    bursts are 90 degrees apart)
//  - reference r of the line from its own burst, rotated onto the -U axis
//    (PAL: +-45 degrees): the line-locked sampling clock jitters from line to
//    line, so the phase of the previous lines is not good enough
//  - w = hc * conj(r) / |r|^2 (hc from the CPU: saturation, hue and the
//    standard's burst/chroma ratio), so that U + jV comes from Z * w:
//    burst-locked phase and automatic chroma gain
//  - a frequency loop trims the DDS increment so that r does not rotate from
//    line to line (source subcarrier / OSSC crystal tolerance)
// Luma: composite minus the remodulated chroma (an adaptive notch at the
// subcarrier, removed when the colour killer is active).
//
// The active video is demodulated one line later (line memories), with the
// reference interpolated between the bursts of its line and of the next one.
//

// all multipliers (constants included) on the embedded multipliers: logic
// elements are the scarce resource
(* multstyle = "dsp" *)
module cvbs_decoder #(
    parameter SIDE_W = 40
) (
    input clk,
    input [7:0] R_i,
    input [7:0] G_i,
    input [7:0] B_i,
    input HS_i,                 // active low, aligned with the data
    input VS_i,                 // active low
    input [SIDE_W-1:0] side_i,  // other signals delayed with the video
    // [0] enable, [1] S-Video (chroma on B), [2] PAL (V switch), [3] band-pass
    // for 4.43MHz (else 3.58MHz), [4] no luma notch, [5] 27MHz crystal
    // clock (HS flywheel, in ossc.v),
    // [6] luma re-timed on the sync edge (27MHz crystal clock), [7] output
    // resync at every frame (scanconverter, test),
    // [15:8] burst gate start (samples from the hsync leading edge),
    // [21:16] brightness (signed, x2), [23:22] chroma suppression at luma
    // edges (0 off, 1-3)
    input [23:0] cfg,
    // [9:0] re, [25:16] im (signed), [31:26] luma gain - 32, [10]
    // reference low-pass over lines (1/8)
    input [31:0] hc,
    input [31:0] inc,           // DDS increment (2^32 = sample rate)
    output reg [7:0] R_o,
    output reg [7:0] G_o,
    output reg [7:0] B_o,
    output [SIDE_W-1:0] side_o,
    // [7:0] burst level, [15:8] blank level, [23:16] peak code (last field),
    // [24] colour killer, [25] PAL alternation seen, [26] frequency loop at
    // (0), [31:27] (0)
    output reg [31:0] status
);

// The active video is delayed by one line (line memories after the Z stage):
// when a line is demodulated, the bursts of that line and of the next one are
// both known, and the colour reference is interpolated between them along the
// line. The line-locked sampling clock (TVP7002 H-PLL) changes its frequency
// slightly at every hsync, so the subcarrier phase drifts along the line by a
// different amount on each line; the reference follows that drift. The
// picture comes out one line lower (the syncs are not delayed).
localparam LAT = 41;            // inputs -> outputs (clocks)
localparam GATE_LEN = 32;       // burst gate length (samples)
localparam ZD = 22;             // h counter -> Z stage delay

wire en       = cfg[0];
wire svideo   = cfg[1];
wire pal      = cfg[2];
wire bp443    = cfg[3];
wire nonotch  = cfg[4];
wire line_align = cfg[6];      // luma re-timed on the sync edge (27MHz crystal clock)
wire rflt = hc[10];             // reference low-pass over lines (1/8)
wire [7:0] bstart = cfg[15:8];
wire [1:0] edge_sup = cfg[23:22];   // chroma suppression at luma edges: 0 off, 1-3
wire signed [5:0] bright = cfg[21:16];
wire signed [9:0] hc_re = hc[9:0];
wire signed [9:0] hc_im = hc[25:16];
wire [6:0] luma_gain = {1'b0, hc[31:26]} + 7'd32;   // x1/32 (32-95)

// ---------------------------------------------------------------------------
// side signals and bypass path
reg [SIDE_W-1:0] side_d [0:LAT-1];
reg [23:0] rgb_d [0:LAT-2];
integer i;
always @(posedge clk) begin
    side_d[0] <= side_i;
    rgb_d[0] <= {R_i, G_i, B_i};
    for (i=1; i<LAT; i=i+1)
        side_d[i] <= side_d[i-1];
    for (i=1; i<LAT-1; i=i+1)
        rgb_d[i] <= rgb_d[i-1];
end
assign side_o = side_d[LAT-1];

// ---------------------------------------------------------------------------
// s0: input registers, h counter (h is the index of x0 one clock earlier)
reg [7:0] x0, c0;
reg hs0, hs0_prev, vs0, vs0_prev;
reg [11:0] h;
always @(posedge clk) begin
    x0 <= G_i;
    c0 <= svideo ? B_i : G_i;
    hs0 <= HS_i;
    vs0 <= VS_i;
    hs0_prev <= hs0;
    vs0_prev <= vs0;
    if (hs0_prev & ~hs0)
        h <= 12'h0;
    else if (h != 12'hfff)
        h <= h + 1'b1;
end

// x0 delayed (x0d[k] = x0 k clocks earlier)
reg [7:0] x0d [1:26];
always @(posedge clk) begin
    x0d[1] <= x0;
    for (i=2; i<=26; i=i+1)
        x0d[i] <= x0d[i-1];
end

// ---------------------------------------------------------------------------
// sync trailing edge (27MHz crystal: the line start h = 0 is quantised to a
// sample, so each line is shifted by up to +-0.5 sample). The edge is found
// at the crossing of the level half way between sync tip and blank, then
// timed by the area over 16 samples around it: with a band-limited edge the
// number of samples at blank level L = sum(x - tip) / (blank - tip) does not
// depend on the edge shape. E = k - L; d = E - average E (1/16 sample),
// applied to the luma read from the line memory (rounded for the chroma)
reg [12:0] se_tip;
reg [8:0] se_thr2;          // tip + blank (2x threshold)
reg [12:0] se_ms;           // sum of the last 16 samples
reg se_arm, se_found;
reg [7:0] se_k;
reg [3:0] se_wait;
reg [16:0] se_n;            // dividend, shifted out MSB first
reg [8:0] se_rem;
reg [7:0] se_den;
reg [9:0] se_q;             // L x 16
reg [4:0] se_cnt;
reg [17:0] se_acc;          // average edge x 2^6
reg se_init;
reg signed [12:0] se_d;     // edge - average (1/16 sample)
reg signed [5:0] d_line;    // for the line read from the memories
wire [8:0] x0_2 = {x0, 1'b0};
wire [8:0] x0d_2 = {x0d[1], 1'b0};
wire [7:0] se_tp = se_tip[12:5];
wire signed [13:0] se_num = $signed({1'b0, se_ms}) - $signed({2'b0, se_tp, 4'h0});
wire [7:0] se_dn0 = blank - se_tp;
wire [11:0] se_e = {se_k, 4'h0} + 12'd512 - {2'b0, se_q};
wire [11:0] se_avg = se_acc[17:6];
wire signed [12:0] se_dn = $signed({1'b0, se_e}) - $signed({1'b0, se_avg});
wire [9:0] se_rem2 = {se_rem, se_n[16]};
always @(posedge clk) begin
    se_ms <= se_ms + x0 - x0d[16];
    if (h == 12'd32)
        se_tip <= {5'h0, x0};
    else if (h < 12'd64)
        se_tip <= se_tip + x0;
    if (h == 12'd64) begin
        se_thr2 <= se_tp + blank;
        se_arm <= 1'b1;
        se_found <= 1'b0;
    end else if (se_arm && (h < 12'd255) && (x0d_2 < se_thr2) && (x0_2 >= se_thr2)) begin
        // x0d[1] is the sample before x0: wait for 8 more samples
        se_arm <= 1'b0;
        se_k <= h[7:0];
        se_wait <= 4'd8;
    end else if (h == 12'd255)
        se_arm <= 1'b0;
    if (se_wait != 4'd0) begin
        se_wait <= se_wait - 1'b1;
        if (se_wait == 4'd1) begin
            se_n <= {(se_num[13] ? 13'h0 : se_num[12:0]), 4'h0};
            se_den <= se_dn0;
            se_rem <= 9'h0;
            se_cnt <= (se_dn0 >= 8'd8 && se_dn0 < 8'd200) ? 5'd17 : 5'd0;
        end
    end
    if (se_cnt != 5'd0) begin
        // restoring division, one quotient bit per clock
        se_cnt <= se_cnt - 1'b1;
        se_n <= {se_n[15:0], 1'b0};
        if (se_rem2 >= {2'b0, se_den}) begin
            se_rem <= se_rem2 - {2'b0, se_den};
            se_q <= {se_q[8:0], 1'b1};
        end else begin
            se_rem <= se_rem2[8:0];
            se_q <= {se_q[8:0], 1'b0};
        end
        if (se_cnt == 5'd1)
            se_found <= 1'b1;
    end
    if (se_found && (h == 12'd300)) begin
        if (se_init || se_dn > 13'sd128 || se_dn < -13'sd128) begin
            // first line, or edge moved (other mode): restart the average
            se_acc <= {se_e, 6'h0};
            se_init <= 1'b0;
            se_d <= 13'sd0;
        end else begin
            se_acc <= se_acc + {{5{se_dn[12]}}, se_dn};
            se_d <= se_dn;
        end
    end else if (h == 12'd300)
        se_d <= 13'sd0;
    if (hs0_prev & ~hs0)
        d_line <= ~line_align ? 6'sd0 :
                  (se_d > 13'sd31) ? 6'sd31 :
                  (se_d < -13'sd32) ? 6'h20 : se_d[5:0];
end
// read positions: h + 2 + d (luma), h + 2 + round(d) (chroma)
wire signed [6:0] d_r7 = d_line + 7'sd8;
wire signed [2:0] d_rnd = d_r7 >>> 4;
wire [11:0] h_cr = h + 12'd2 + {{9{d_rnd[2]}}, d_rnd};
// (luma 8 samples ahead of the chroma: the edge detection of stage E looks
// ahead, no chroma delay line)
wire [11:0] h_yr = h + 12'd11 + {{10{d_line[5]}}, d_line[5:4]};

// ---------------------------------------------------------------------------
// blank level: x0 is first filtered by a 6-sample box (about one subcarrier
// cycle), then averaged over 30 samples of the burst gate: the burst residual
// is < 1% of its amplitude, so the level no longer beats with the burst phase
// from line to line. Low-pass over lines (4 fractional bits). The integer
// level used by the luma path is updated only at the start of a field, and
// only when the filtered value is more than 0.75 code away (hysteresis): a
// level toggling between two codes inside the field showed as flickering
// horizontal bands.
wire gate_h = (h >= {4'h0, bstart}) && (h < {4'h0, bstart} + GATE_LEN);   // burst gate
wire gate_bl = (h >= {4'h0, bstart} + 12'd3) && (h < {4'h0, bstart} + 12'd33);
reg gate_bl_prev;
reg [9:0] bx_a, bx_b;
reg [10:0] bx6;
reg [15:0] bl_acc;
reg bl_upd;
reg [11:0] blank_f = 12'd960;   // 60.0
reg [11:0] bl_line;     // mean of the gate, 4 fractional bits
reg [7:0] blank = 8'd60;
wire signed [12:0] bl_diff = $signed({1'b0, bl_line}) - $signed({1'b0, blank_f});
wire [11:0] bl_step = {{3{bl_diff[12]}}, bl_diff[11:3]};
wire signed [12:0] bl_err = $signed({1'b0, blank_f}) - $signed({1'b0, blank, 4'h0});
wire [22:0] bl_scaled = bl_acc * 7'd91;     // x16/180
always @(posedge clk) begin
    bx_a <= x0d[1] + x0d[2] + x0d[3];
    bx_b <= x0d[4] + x0d[5] + x0d[6];
    bx6 <= bx_a + bx_b;
    gate_bl_prev <= gate_bl;
    bl_upd <= 1'b0;
    if (gate_bl) begin
        bl_acc <= bl_acc + bx6;
    end else if (gate_bl_prev) begin
        bl_line <= bl_scaled[21:10];
        bl_acc <= 16'h0;
        bl_upd <= 1'b1;
    end
    if (bl_upd)     // blank_f += (line*16 - blank_f) / 8
        blank_f <= blank_f + bl_step;
end

// ---------------------------------------------------------------------------
// band-pass: c_bp = 2*c(n-4) - c(n-4-k) - c(n-4+k), k = 3 (4.43) or 4 (3.58).
// Gain ~4 at the subcarrier, 0 at DC. Two stages.
reg [7:0] cw [1:8];
always @(posedge clk) begin
    cw[1] <= c0;
    for (i=2; i<=8; i=i+1)
        cw[i] <= cw[i-1];
end
reg signed [9:0] c_bp;
always @(posedge clk) begin
    if (bp443)
        c_bp <= $signed({2'b0, cw[4], 1'b0}) - $signed({2'b0, cw[1]}) - $signed({2'b0, cw[7]});
    else
        c_bp <= $signed({2'b0, cw[4], 1'b0}) - $signed({2'b0, c0}) - $signed({2'b0, cw[8]});
end

// second identical stage (gain 1 at the subcarrier): narrower band, less
// luma (edges) demodulated as chroma
reg signed [9:0] cb [1:8];
always @(posedge clk) begin
    cb[1] <= c_bp;
    for (i=2; i<=8; i=i+1)
        cb[i] <= cb[i-1];
end
wire signed [11:0] c_bp2_k3 = $signed({cb[4], 1'b0}) - cb[1] - cb[7];
wire signed [11:0] c_bp2_k4 = $signed({cb[4], 1'b0}) - c_bp - cb[8];
reg signed [9:0] c_bp2;
always @(posedge clk)
    c_bp2 <= bp443 ? (c_bp2_k3 >>> 2) : (c_bp2_k4 >>> 2);

// ---------------------------------------------------------------------------
// DDS and sin/cos ROM (10-bit signed, +-511)
reg [31:0] ph;
// frequency loop: the DDS follows the subcarrier of the source (the burst
// phase stays still from line to line, so the reference can be low-passed
// over lines). Low gain: the burst noise does not modulate the DDS.
reg signed [21:0] corr;
wire [31:0] inc_eff = inc + {{10{corr[21]}}, corr};
reg [19:0] sincos_rom [0:1023];
initial $readmemh("rtl/cvbs_sincos.hex", sincos_rom);
reg [19:0] sc_q;
always @(posedge clk) begin
    ph <= ph + inc_eff;
    sc_q <= sincos_rom[ph[31:22]];
end
wire signed [9:0] sin_q = sc_q[19:10];
wire signed [9:0] cos_q = sc_q[9:0];

// ---------------------------------------------------------------------------
// demodulation: p = c_bp * e^(-j phase) (scaled 1/256)
wire signed [19:0] pI_full = c_bp2 * cos_q;
wire signed [19:0] pQ_full = c_bp2 * sin_q;
reg signed [10:0] pI, pQ;
// The box filters are running sums: an error (e.g. a clock glitch while the
// TVP7002 PLL is reprogrammed) would stay forever. They are flushed in the
// hsync of every line: zero input from h = 20, then the sums are forced to 0
// once their delay lines hold zeros (h = 30 / 40 / 52, until h = 60).
wire flush_p = (h >= 12'd20) && (h < 12'd60);
wire flush_b6 = (h >= 12'd30) && (h < 12'd60);
wire flush_b7 = (h >= 12'd40) && (h < 12'd60);
wire flush_b8 = (h >= 12'd52) && (h < 12'd60);
always @(posedge clk) begin
    pI <= flush_p ? 11'sd0 : pI_full[18:8];
    pQ <= flush_p ? 11'sd0 : -pQ_full[18:8];
end

// sin/cos delayed to the Z stage (13 clocks after sc_q)
reg [19:0] scd [1:13];
always @(posedge clk) begin
    scd[1] <= sc_q;
    for (i=2; i<=13; i=i+1)
        scd[i] <= scd[i-1];
end
wire signed [9:0] sin_z0 = scd[13][19:10];
wire signed [9:0] cos_z0 = scd[13][9:0];

// low-pass: box filters 6, 7, 8 (gain 336), running sums
reg signed [10:0] pI_d [1:6];
reg signed [10:0] pQ_d [1:6];
reg signed [13:0] b6I, b6Q;
reg signed [13:0] b6I_d [1:7];
reg signed [13:0] b6Q_d [1:7];
reg signed [16:0] b7I, b7Q;
reg signed [16:0] b7I_d [1:8];
reg signed [16:0] b7Q_d [1:8];
reg signed [19:0] b8I, b8Q;
always @(posedge clk) begin
    pI_d[1] <= pI;
    pQ_d[1] <= pQ;
    b6I_d[1] <= b6I;
    b6Q_d[1] <= b6Q;
    b7I_d[1] <= b7I;
    b7Q_d[1] <= b7Q;
    for (i=2; i<=6; i=i+1) begin
        pI_d[i] <= pI_d[i-1];
        pQ_d[i] <= pQ_d[i-1];
    end
    for (i=2; i<=7; i=i+1) begin
        b6I_d[i] <= b6I_d[i-1];
        b6Q_d[i] <= b6Q_d[i-1];
    end
    for (i=2; i<=8; i=i+1) begin
        b7I_d[i] <= b7I_d[i-1];
        b7Q_d[i] <= b7Q_d[i-1];
    end
    b6I <= flush_b6 ? 14'sd0 : b6I + pI - pI_d[6];
    b6Q <= flush_b6 ? 14'sd0 : b6Q + pQ - pQ_d[6];
    b7I <= flush_b7 ? 17'sd0 : b7I + b6I - b6I_d[7];
    b7Q <= flush_b7 ? 17'sd0 : b7Q + b6Q - b6Q_d[7];
    b8I <= flush_b8 ? 20'sd0 : b8I + b7I - b7I_d[8];
    b8Q <= flush_b8 ? 20'sd0 : b8Q + b7Q - b7Q_d[8];
end
wire signed [15:0] zI0 = b8I[19:4];  // Z stage (burst measurement)
wire signed [15:0] zQ0 = b8Q[19:4];

// Z stage for the luma notch (remodulation) of the line being received
wire signed [15:0] zI = zI0;
wire signed [15:0] zQ = zQ0;
wire signed [9:0] sin_z = sin_z0;
wire signed [9:0] cos_z = cos_z0;

// line memory of the chroma (Z, one sample in 4, 512 x 32): written at the Z
// stage, read one line later at the same h, linearly interpolated
// (lo, lo, (lo+hi)/2, (lo+hi)/2 within each group of 4)
(* ramstyle = "M9K" *) reg [31:0] cl_mem [0:511];
reg [31:0] cl_q;
reg signed [15:0] cl_loI, cl_loQ, cl_hiI, cl_hiQ;
reg signed [15:0] zIo, zQo;         // Z of the previous line
wire [8:0] cl_ra = h_cr[10:2] + 9'd1;
wire signed [16:0] cl_mI = cl_loI + cl_hiI;
wire signed [16:0] cl_mQ = cl_loQ + cl_hiQ;
reg [1:0] h_f;
always @(posedge clk) begin
    cl_q <= cl_mem[cl_ra];
    if (h[1:0] == 2'd0)
        cl_mem[h[10:2]] <= {zI0, zQ0};
    h_f <= h_cr[1:0];
    if (h_f == 2'd0) begin
        cl_loI <= cl_hiI;
        cl_loQ <= cl_hiQ;
        cl_hiI <= cl_q[31:16];
        cl_hiQ <= cl_q[15:0];
    end
    zIo <= (h_f == 2'd0) ? cl_hiI : (h_f[1] ? cl_mI[16:1] : cl_loI);
    zQo <= (h_f == 2'd0) ? cl_hiQ : (h_f[1] ? cl_mQ[16:1] : cl_loQ);
end

// ---------------------------------------------------------------------------
// burst gate at the Z stage, burst accumulation
reg [ZD:1] gate_zd;
always @(posedge clk)
    gate_zd <= {gate_zd[ZD-1:1], gate_h};
wire gate_z = gate_zd[ZD];
reg gate_z_prev;
reg signed [21:0] accI, accQ;
reg fsm_start;
always @(posedge clk) begin
    gate_z_prev <= gate_z;
    fsm_start <= 1'b0;
    if (gate_z) begin
        accI <= accI + zI0;
        accQ <= accQ + zQ0;
    end else if (gate_z_prev) begin
        fsm_start <= 1'b1;
    end
    if (fsm_start) begin
        accI <= 22'h0;
        accQ <= 22'h0;
    end
end

// ---------------------------------------------------------------------------
// per-line processing (one shared multiplier)
reg [6:0] st;
reg signed [23:0] ma, mb;
reg signed [47:0] mres;
always @(posedge clk)
    mres <= ma * mb;

reg signed [17:0] Zr, Zi, Pr, Pi;   // current / previous valid burst
reg signed [18:0] Rr, Ri;           // reference of the line
reg pv;                             // P valid
reg vb;                             // current burst valid
reg signed [47:0] t1;
reg [31:0] m;
reg [41:0] rem;
reg [19:0] rec;
reg [5:0] div_i;
reg signed [23:0] ar, ai;
reg signed [17:0] w_re, w_im;
reg s_prev;                         // PAL switch of the previous line
reg signed [18:0] RPr, RPi;         // r of the previous line
reg rpv;
reg rot_p, rot_z;                   // sign of Im(r * conj(rp)): r rotates +, no rotation
reg signed [18:0] Sr, Si;           // reference used: r low-passed over lines
wire signed [19:0] sdr = Rr - Sr;
wire signed [19:0] sdi = Ri - Si;
wire signed [19:0] sdr_s = sdr >>> 3;
wire signed [19:0] sdi_s = sdi >>> 3;
// sign-only (bang-bang) loop, step 128 (0.02 degrees per line), 1024 after
// 8 lines rotating the same way (acquisition: 100ppm off in ~100 lines)
reg [6:0] rot_h;
wire rot_fast = (rot_h == {7{rot_p}});
wire signed [22:0] corr_step = rot_fast ? 23'sd1024 : 23'sd128;
wire signed [22:0] corr_next = rot_z ? $signed({corr[21], corr}) : (rot_p ? corr + corr_step : corr - corr_step);
reg kill;
reg s_cur, s_meas, s_meas_prev, s_meas_pv;
reg [9:0] alt_cnt, bad_cnt;
reg alt_ok;

wire [17:0] absZr = Zr[17] ? -Zr : Zr;
wire [17:0] absZi = Zi[17] ? -Zi : Zi;
localparam [18:0] VB_THOLD = 19'd1000;
localparam [31:0] KILL_THOLD = 32'd2097152;   // |r| < ~1448

// saturate a 48-bit value to 24 bits
function signed [23:0] sat24;
    input signed [47:0] v;
    begin
        if (v > 48'sd8388607)
            sat24 = 24'sd8388607;
        else if (v < -48'sd8388608)
            sat24 = 24'sh800000;
        else
            sat24 = v[23:0];
    end
endfunction

reg [3:0] nob_cnt;      // consecutive lines without a valid burst
reg Rr2;                // NTSC: reference = 2Z (second step)
wire meas_w = ((t1 - mres) < 0);    // step 4: Im(Z * conj(P)) < 0
wire s_new = pal ? ((vb & pv) ? meas_w : ~s_cur) : 1'b1;

// vsync leading edge (field counters)
wire vs_fall = vs0_prev & ~vs0;

always @(posedge clk)
    if (vs_fall && (bl_err > 13'sd12 || bl_err < -13'sd12))
        blank <= blank_f[11:4] + blank_f[3];

always @(posedge clk) begin
    if (vs_fall) begin
        alt_ok <= (alt_cnt > 10'd64) && (bad_cnt < (alt_cnt >> 3));
        alt_cnt <= 10'h0;
        bad_cnt <= 10'h0;
    end
    if (fsm_start) begin
        Zr <= accI >>> 5;
        Zi <= accQ >>> 5;
        st <= 7'd1;
    end else if (st != 0) begin
        st <= st + 1'b1;
        case (st)
            1: begin
                vb <= ({1'b0, absZr} + {1'b0, absZi} >= VB_THOLD);
                ma <= Zi; mb <= Pr;
            end
            2: begin ma <= Zr; mb <= Pi; end
            3: t1 <= mres;          // Zi*Pr
            4: begin
                // Im(Z * conj(P)) = Zi*Pr - Zr*Pi; < 0: +V line (PAL)
                s_meas <= meas_w;
                if (vb & pv) begin
                    if (s_meas_pv) begin
                        if (meas_w != s_meas_prev)
                            alt_cnt <= (alt_cnt != 10'h3ff) ? alt_cnt + 1'b1 : alt_cnt;
                        else
                            bad_cnt <= (bad_cnt != 10'h3ff) ? bad_cnt + 1'b1 : bad_cnt;
                    end
                    s_meas_prev <= meas_w;
                    s_meas_pv <= 1'b1;
                end else begin
                    s_meas_pv <= 1'b0;
                end
                s_prev <= s_cur;
                s_cur <= s_new;
                if (vb) begin
                    // reference of this line from its own burst, rotated
                    // onto the -U axis (PAL: +-45 degrees); same magnitude
                    // as Z(n) + Z(n-1) (NTSC: 2Z, PAL: sqrt(2)Z)
                    if (!pal) begin
                        Rr <= {Zr[17], Zr};
                        Ri <= {Zi[17], Zi};
                        Rr2 <= 1'b1;
                    end else if (s_new) begin
                        Rr <= Zr - Zi;      // Z * (1 + j)
                        Ri <= Zr + Zi;
                        Rr2 <= 1'b0;
                    end else begin
                        Rr <= Zr + Zi;      // Z * (1 - j)
                        Ri <= Zi - Zr;
                        Rr2 <= 1'b0;
                    end
                    Pr <= Zr;
                    Pi <= Zi;
                    pv <= 1'b1;
                    nob_cnt <= 4'h0;
                end else begin
                    // no burst: keep the current reference; colour off
                    // after 8 lines without a burst (not for the few
                    // lines of the vertical blanking)
                    if (nob_cnt != 4'hf)
                        nob_cnt <= nob_cnt + 1'b1;
                    if (nob_cnt >= 4'd7)
                        kill <= 1'b1;
                    pv <= 1'b0;
                    st <= 7'd0;
                end
            end
            5: begin
                if (Rr2) begin      // NTSC: 2Z
                    Rr <= Rr <<< 1;
                    Ri <= Ri <<< 1;
                end
            end
            6: begin
                ma <= Ri; mb <= RPr;
                // reference low-pass over lines (none: this line's burst)
                if (~rflt) begin
                    Sr <= Rr;
                    Si <= Ri;
                end else begin
                    Sr <= Sr + sdr_s[18:0];
                    Si <= Si + sdi_s[18:0];
                end
            end
            7: begin ma <= Rr; mb <= RPi; end
            8: begin
                t1 <= mres;         // Ri*RPr
            end
            9: begin
                rot_p <= (t1 > mres);   // Im(r * conj(rp)) = Ri*RPr - Rr*RPi
                rot_z <= (t1 == mres);
                ma <= Sr; mb <= Sr;
            end
            10: begin ma <= Si; mb <= Si; end
            11: t1 <= mres;
            12: begin
                m <= (t1 + mres) > 48'sh7fffffff ? 32'h7fffffff : t1[31:0] + mres[31:0];
                rem <= 42'h0;
                rec <= 20'h0;
                div_i <= 6'd40;
            end
            13: begin
                if (m < KILL_THOLD) begin
                    kill <= 1'b1;
                    w_re <= 18'h0;
                    w_im <= 18'h0;
                    RPr <= Rr;
                    RPi <= Ri;
                    rpv <= 1'b1;
                    st <= 7'd0;
                end
            end
            14: begin
                // rec = 2^40 / m, restoring division (one bit per clock)
                if ({rem[40:0], (div_i == 6'd40)} >= {10'h0, m}) begin
                    rem <= {rem[40:0], (div_i == 6'd40)} - {10'h0, m};
                    rec <= {rec[18:0], 1'b1};
                end else begin
                    rem <= {rem[40:0], (div_i == 6'd40)};
                    rec <= {rec[18:0], 1'b0};
                end
                if (div_i != 0) begin
                    div_i <= div_i - 1'b1;
                    st <= st;
                end
            end
            15: ;
            16: begin ma <= Sr; mb <= {4'h0, rec}; end
            17: begin
                ma <= Si; mb <= {4'h0, rec};
            end
            18: begin
                ar <= sat24(mres >>> 12);       // conj(r) * rec
                // frequency loop (the line to line rotation of r)
                if (rpv) begin
                    if (~rot_z)
                        rot_h <= {rot_h[5:0], rot_p};
                    if (corr_next[22] != corr_next[21])
                        corr <= corr_next[22] ? 22'h200000 : 22'h1fffff;
                    else
                        corr <= corr_next[21:0];
                end
            end
            19: begin
                ai <= sat24(-(mres >>> 12));
            end
            // w = a * hc (x1/256)
            20: begin ma <= ar; mb <= hc_re; end
            21: begin ma <= ai; mb <= hc_im; end
            22: begin t1 <= mres; ma <= ar; mb <= hc_im; end
            23: begin w_re <= sat18((t1 - mres) >>> 8); ma <= ai; mb <= hc_re; end
            24: t1 <= mres;
            25: begin
                w_im <= sat18((t1 + mres) >>> 8);
                kill <= 1'b0;
                RPr <= Rr;
                RPi <= Ri;
                rpv <= 1'b1;
                st <= 7'd0;
            end
            default: st <= 7'd0;
        endcase
    end

end

function signed [17:0] sat18;
    input signed [47:0] v;
    begin
        if (v > 48'sd131071)
            sat18 = 18'sd131071;
        else if (v < -48'sd131072)
            sat18 = 18'sh20000;
        else
            sat18 = v[17:0];
    end
endfunction

// ---------------------------------------------------------------------------
// Z stage + 1..3: remodulated chroma and U/V
reg signed [25:0] rm1, rm2;
reg signed [26:0] rm_s;
wire signed [33:0] rm_p = rm_s * 7'sd49;
reg signed [9:0] rmod;
reg signed [33:0] q1, q2, q3, q4;
reg signed [34:0] zre, zim;
reg signed [10:0] U3, V3, UA, VA;
reg s_d;

function signed [10:0] sat11;
    input signed [34:0] v;
    begin
        if (v > 35'sd1023)
            sat11 = 11'sd1023;
        else if (v < -35'sd1024)
            sat11 = 11'sh400;
        else
            sat11 = v[10:0];
    end
endfunction

always @(posedge clk) begin
    // remodulation: c_bp = (I cos - Q sin) / (20.96*511); luma chroma = c_bp/4
    rm1 <= zI * cos_z;
    rm2 <= zQ * sin_z;
    rm_s <= rm1 - rm2;
    rmod <= rm_p >>> 21;        // x49 / 2^21 = 1/42798

    // U + jV of the previous line from its Z and the interpolated reference
    // (1/2^20); U = -Re, V = -s*Im
    q1 <= zIo * w_re;
    q2 <= zQo * w_im;
    q3 <= zIo * w_im;
    q4 <= zQo * w_re;
    zre <= q1 - q2;
    zim <= q3 + q4;
    s_d <= s_prev;
    U3 <= sat11(-(zre >>> 20));
    V3 <= s_d ? sat11(-(zim >>> 20)) : sat11(zim >>> 20);
end

// ---------------------------------------------------------------------------
// stage A: luma of the line being received (notch: minus the remodulated
// chroma, not when killed), stored in the luma line memory (2048 x 8, offset
// +32, -32..223); out: luma of the previous line, with its chroma
reg signed [10:0] yAn;
(* ramstyle = "M9K" *) reg [7:0] yl_mem [0:2047];
reg [7:0] yl_q;
reg signed [10:0] yAo;
wire signed [11:0] yAn_o = yAn + 12'sd32;
wire [7:0] yAn8 = yAn_o[11] ? 8'd0 : ((yAn_o > 12'sd255) ? 8'd255 : yAn_o[7:0]);
reg [10:0] h_d1;
reg [7:0] yl_qp;
reg [3:0] d_f;
wire signed [8:0] yl_dd = $signed({1'b0, yl_q}) - $signed({1'b0, yl_qp});
wire signed [13:0] yl_ip = yl_dd * $signed({1'b0, d_f});
always @(posedge clk) begin
    if (svideo | nonotch | kill)
        yAn <= $signed({3'b0, x0d[26]}) - $signed({3'b0, blank});
    else
        yAn <= $signed({3'b0, x0d[26]}) - $signed({3'b0, blank}) - rmod;
    h_d1 <= h[10:0];
    yl_q <= yl_mem[h_yr[10:0]];
    yl_mem[h_d1] <= yAn8;
    yl_qp <= yl_q;
    d_f <= d_line[3:0];
    // between the two samples read last (fraction of d)
    yAo <= $signed({3'b0, yl_qp}) + (yl_ip >>> 4) - 11'sd32;
end
reg signed [10:0] yA;
always @(posedge clk) begin
    yA <= yAo;
    UA <= U3;
    VA <= V3;
end

// stage S: luma delayed by 4 (alignment with the chroma of stage E)
reg signed [10:0] yAd [1:6];
reg signed [10:0] yS;
always @(posedge clk) begin
    yAd[1] <= yA;
    for (i=2; i<=6; i=i+1)
        yAd[i] <= yAd[i-1];
    yS <= yAd[3];
end

// stage E: chroma suppression at luma edges. A sharp luma edge has energy at
// the subcarrier, which is demodulated as colour fringes (cross-colour, e.g.
// blue / purple at black / white edges). The chroma is attenuated around the
// edges: edge strength e = |Y(n) - Y(n-4)|, envelope decaying by 6 per
// sample, chroma delayed by 8 (luma: 4 in stage S, then 5 + 1) so that the
// attenuation also covers the fringe before the edge. Off with S-Video (no
// cross-colour; dark bands at edges between colours of different luma).
reg signed [10:0] yE_d [1:5];
reg [10:0] e_env;
reg [8:0] e_att;
reg signed [10:0] yE, UE, VE;
wire signed [11:0] e_diff = yA - yAd[4];
wire [10:0] e_abs = e_diff[11] ? -e_diff[10:0] : e_diff[10:0];
wire [10:0] e_dec = (e_env > 11'd6) ? e_env - 11'd6 : 11'd0;
wire [13:0] e_scaled = {3'b0, e_env} << edge_sup;   // x2 / x4 / x8
wire signed [9:0] e_gain = 10'sd256 - $signed({1'b0, e_att});
wire signed [20:0] uE_p = UA * e_gain;
wire signed [20:0] vE_p = VA * e_gain;
always @(posedge clk) begin
    yE_d[1] <= yS;
    for (i=2; i<=5; i=i+1)
        yE_d[i] <= yE_d[i-1];
    e_env <= (e_abs > e_dec) ? e_abs : e_dec;
    e_att <= ((edge_sup == 2'h0) | svideo) ? 9'h0 : ((e_scaled > 14'd256) ? 9'd256 : e_scaled[8:0]);
    yE <= yE_d[5];
    UE <= uE_p >>> 8;
    VE <= vE_p >>> 8;
end

// stage B: luma gain (white 160 -> 255), brightness, YUV -> RGB products
wire signed [18:0] yB_p = yE * $signed({1'b0, luma_gain});
reg signed [11:0] yB;
reg signed [21:0] pRV, pGU, pGV, pBU;
always @(posedge clk) begin
    yB <= (yB_p >>> 5) + {{5{bright[5]}}, bright, 1'b0};
    pRV <= VE * 10'sd292;   // 1.140
    pGU <= UE * 10'sd101;   // 0.395
    pGV <= VE * 10'sd149;   // 0.581
    pBU <= UE * 11'sd520;   // 2.032
end

// stage C: sums
reg signed [13:0] rC, gC, bC;
always @(posedge clk) begin
    rC <= yB + (pRV >>> 8);
    gC <= yB - ((pGU + pGV) >>> 8);
    bC <= yB + (pBU >>> 8);
end

function [7:0] clip8;
    input signed [13:0] v;
    begin
        if (v < 0)
            clip8 = 8'h00;
        else if (v > 255)
            clip8 = 8'hff;
        else
            clip8 = v[7:0];
    end
endfunction

// stage D: outputs (LAT clocks after the inputs)
always @(posedge clk) begin
    if (en) begin
        R_o <= clip8(rC);
        G_o <= clip8(gC);
        B_o <= clip8(bC);
    end else begin
        {R_o, G_o, B_o} <= rgb_d[LAT-2];
    end
end

// ---------------------------------------------------------------------------
// status: peak code in the active area of the last field, PAL alternation
reg [7:0] peak, peak_l;
// colour killer for the status: sampled in the middle of the field (the
// killer is briefly on after the vsync, where there is no burst, and the CPU
// reads the status right after the vsync)
reg [8:0] st_line;
reg kill_st;
// burst level: the reference (low-passed over lines when the filter is on)
wire [17:0] absFr = Sr[18] ? -Sr[17:0] : Sr[17:0];
wire [17:0] absFi = Si[18] ? -Si[17:0] : Si[17:0];
wire [18:0] fmag = (absFr > absFi) ? ({1'b0, absFr} + {2'b0, absFi[17:1]}) : ({1'b0, absFi} + {2'b0, absFr[17:1]});
always @(posedge clk) begin
    if (vs_fall) begin
        peak_l <= peak;
        peak <= 8'h0;
    end else if (vs0 && (h > 12'd250) && (h < 12'd1650) && (x0d[1] > peak)) begin
        peak <= x0d[1];
    end
    if (vs_fall)
        st_line <= 9'h0;
    else if ((hs0_prev & ~hs0) && (st_line != 9'h1ff))
        st_line <= st_line + 1'b1;
    if ((hs0_prev & ~hs0) && (st_line == 9'd120))
        kill_st <= kill;
    status <= {corr[21:17], 1'b0, alt_ok, kill_st, peak_l, blank, (fmag[18:14] != 0) ? 8'hff : fmag[13:6]};
end


// ---------------------------------------------------------------------------
// power-up values (also for simulation: the running sums need a defined start)
integer j;
initial begin
    ph = 0; corr = 0; st = 0; pv = 0; rpv = 0; vb = 0; RPr = 0; RPi = 0; rot_p = 0; rot_z = 1; rot_h = 7'h55; Sr = 0; Si = 0;
    Pr = 0; Pi = 0; Rr = 0; Ri = 0;
    w_re = 0; w_im = 0; kill = 1; Rr2 = 0;
    s_cur = 1; s_meas_prev = 0; s_meas_pv = 0; nob_cnt = 0; s_prev = 1;
    cl_q = 0; cl_loI = 0; cl_loQ = 0; cl_hiI = 0; cl_hiQ = 0; zIo = 0; zQo = 0; h_f = 0; yl_q = 0; yAo = 0;
    alt_cnt = 0; bad_cnt = 0; alt_ok = 0; peak = 0; peak_l = 0; st_line = 0; kill_st = 1;
    bl_acc = 0; bl_line = 0; bx_a = 0; bx_b = 0; bx6 = 0; accI = 0; accQ = 0; gate_zd = 0;
    b6I = 0; b6Q = 0; b7I = 0; b7Q = 0; b8I = 0; b8Q = 0; pI = 0; pQ = 0;
    se_tip = 0; se_thr2 = 0; se_ms = 0; se_arm = 0; se_found = 0; se_k = 0; se_wait = 0; se_n = 0; se_rem = 0; se_den = 0; se_q = 0; se_cnt = 0;
    se_acc = 0; se_init = 1; se_d = 0; d_line = 0; yl_qp = 0; d_f = 0;
    h = 0; c_bp = 0; sc_q = 0; x0 = 0; c0 = 0; hs0 = 1; hs0_prev = 1; vs0 = 1; vs0_prev = 1;
    for (j=1; j<=6; j=j+1) begin pI_d[j] = 0; pQ_d[j] = 0; end
    for (j=1; j<=7; j=j+1) begin b6I_d[j] = 0; b6Q_d[j] = 0; end
    for (j=1; j<=8; j=j+1) begin b7I_d[j] = 0; b7Q_d[j] = 0; cw[j] = 0; end
    for (j=1; j<=13; j=j+1) scd[j] = 0;
    for (j=1; j<=26; j=j+1) x0d[j] = 0;
    for (j=1; j<=8; j=j+1) cb[j] = 0;
    c_bp2 = 0; e_env = 0; e_att = 0; yA = 0; UA = 0; VA = 0; rmod = 0; U3 = 0; V3 = 0;
    for (j=1; j<=5; j=j+1) yE_d[j] = 0;

    for (j=1; j<=6; j=j+1) yAd[j] = 0;
    // synthesis translate_off
    for (j=0; j<2048; j=j+1) yl_mem[j] = 8'd32;
    // synthesis translate_on
    yS = 0;
end

endmodule

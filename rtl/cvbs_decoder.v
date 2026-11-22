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
//  - reference r = Zb(n) + Zb(n-1): the PAL +-45 degree swing cancels, so r
//    points along the -U axis for both PAL and NTSC
//  - PAL switch from the sign of Im(Zb(n) * conj(Zb(n-1))) (consecutive PAL
//    bursts are 90 degrees apart)
//  - r is low-pass filtered over lines; w = hc * conj(r) / |r|^2 (hc from the
//    CPU: saturation, hue and the standard's burst/chroma ratio), so that
//    U + jV comes from Z * w: burst-locked phase and automatic chroma gain
//  - a frequency loop trims the DDS increment so that r does not rotate from
//    line to line (source subcarrier / OSSC crystal tolerance)
// Luma: composite minus the remodulated chroma (an adaptive notch at the
// subcarrier, removed when the colour killer is active).
//
// The new w is used from the next line; the PAL switch from the current one.
//

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
    // for 4.43MHz (else 3.58MHz), [4] no luma notch, [5] frequency loop off,
    // [15:8] burst gate start (samples from the hsync leading edge),
    // [21:16] brightness (signed, x2)
    input [23:0] cfg,
    input [31:0] hc,            // [9:0] re, [25:16] im (signed)
    input [31:0] inc,           // DDS increment (2^32 = sample rate)
    output reg [7:0] R_o,
    output reg [7:0] G_o,
    output reg [7:0] B_o,
    output [SIDE_W-1:0] side_o,
    // [7:0] burst level, [15:8] blank level, [23:16] peak code (last field),
    // [24] colour killer, [25] PAL alternation seen, [26] frequency loop at
    // its limit, [31:27] frequency correction (signed, inc/16384)
    output reg [31:0] status
);

localparam LAT = 26;            // inputs -> outputs (clocks)
localparam GATE_LEN = 32;       // burst gate length (samples)
localparam ZD = 17;             // h counter -> Z stage delay

wire en       = cfg[0];
wire svideo   = cfg[1];
wire pal      = cfg[2];
wire bp443    = cfg[3];
wire nonotch  = cfg[4];
wire loop_off = cfg[5];
wire [7:0] bstart = cfg[15:8];
wire signed [5:0] bright = cfg[21:16];
wire signed [9:0] hc_re = hc[9:0];
wire signed [9:0] hc_im = hc[25:16];

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
reg [7:0] x0d [1:21];
always @(posedge clk) begin
    x0d[1] <= x0;
    for (i=2; i<=21; i=i+1)
        x0d[i] <= x0d[i-1];
end

// ---------------------------------------------------------------------------
// blank level: mean of x0 over the burst gate (the burst averages out), then
// low-pass over lines (4 fractional bits)
wire gate_h = (h >= {4'h0, bstart}) && (h < {4'h0, bstart} + GATE_LEN);
reg gate_h_prev;
reg [12:0] bl_acc;
reg bl_upd;
reg [11:0] blank_f = 12'd960;   // 60.0
reg [11:0] bl_line;     // mean of the gate, 4 fractional bits
wire signed [12:0] bl_diff = $signed({1'b0, bl_line}) - $signed({1'b0, blank_f});
wire [11:0] bl_step = {{3{bl_diff[12]}}, bl_diff[11:3]};
always @(posedge clk) begin
    gate_h_prev <= gate_h;
    bl_upd <= 1'b0;
    if (gate_h) begin
        bl_acc <= bl_acc + x0d[1];
    end else if (gate_h_prev) begin
        bl_line <= bl_acc[12:1];        // /32 (x16)
        bl_acc <= 13'h0;
        bl_upd <= 1'b1;
    end
    if (bl_upd)     // blank_f += (line*16 - blank_f) / 8
        blank_f <= blank_f + bl_step;
end
wire [7:0] blank = blank_f[11:4] + blank_f[3];

// ---------------------------------------------------------------------------
// band-pass: c_bp = 2*c(n-4) - c(n-4-k) - c(n-4+k), k = 3 (4.43) or 4 (3.58).
// Gain ~4 at the subcarrier, 0 at DC.
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

// ---------------------------------------------------------------------------
// DDS and sin/cos ROM (10-bit signed, +-511)
reg [31:0] ph;
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
wire signed [19:0] pI_full = c_bp * cos_q;
wire signed [19:0] pQ_full = c_bp * sin_q;
reg signed [10:0] pI, pQ;
always @(posedge clk) begin
    pI <= pI_full[18:8];
    pQ <= -pQ_full[18:8];
end

// sin/cos delayed to the Z stage (13 clocks after sc_q)
reg [19:0] scd [1:13];
always @(posedge clk) begin
    scd[1] <= sc_q;
    for (i=2; i<=13; i=i+1)
        scd[i] <= scd[i-1];
end
wire signed [9:0] sin_z = scd[13][19:10];
wire signed [9:0] cos_z = scd[13][9:0];

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
    b6I <= b6I + pI - pI_d[6];
    b6Q <= b6Q + pQ - pQ_d[6];
    b7I <= b7I + b6I - b6I_d[7];
    b7Q <= b7Q + b6Q - b6Q_d[7];
    b8I <= b8I + b7I - b7I_d[8];
    b8Q <= b8Q + b7Q - b7Q_d[8];
end
wire signed [15:0] zI = b8I[19:4];
wire signed [15:0] zQ = b8Q[19:4];

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
        accI <= accI + zI;
        accQ <= accQ + zQ;
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
reg signed [18:0] Rr, Ri, RPr, RPi; // r = Z + P, previous r
reg signed [18:0] Fr, Fi;           // low-pass filtered r
reg pv, rpv;                        // P / RP valid
reg vb;                             // current burst valid
reg signed [47:0] t1;
reg signed [47:0] dIm;
reg [31:0] m;
reg [41:0] rem;
reg [19:0] rec;
reg [5:0] div_i;
reg signed [23:0] ar, ai;
reg signed [17:0] w_re_n, w_im_n, w_re, w_im;
reg kill_n, kill;
reg s_cur, s_meas, s_meas_prev, s_meas_pv;
reg signed [23:0] dq;
reg corr_sat;
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

localparam signed [21:0] CORR_MAX = 22'sd2000000;
wire signed [25:0] corr_next = corr + dq + (dq >>> 1);
reg [3:0] nob_cnt;      // consecutive lines without a valid burst

// vsync leading edge (field counters)
wire vs_fall = vs0_prev & ~vs0;

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
                s_meas <= ((t1 - mres) < 0);
                if (vb & pv) begin
                    if (s_meas_pv) begin
                        if (((t1 - mres) < 0) != s_meas_prev)
                            alt_cnt <= (alt_cnt != 10'h3ff) ? alt_cnt + 1'b1 : alt_cnt;
                        else
                            bad_cnt <= (bad_cnt != 10'h3ff) ? bad_cnt + 1'b1 : bad_cnt;
                    end
                    s_meas_prev <= ((t1 - mres) < 0);
                    s_meas_pv <= 1'b1;
                    s_cur <= pal ? ((t1 - mres) < 0) : 1'b1;
                end else begin
                    s_meas_pv <= 1'b0;
                    s_cur <= pal ? ~s_cur : 1'b1;
                end
                if (vb & pv) begin
                    Rr <= Zr + Pr;
                    Ri <= Zi + Pi;
                end else begin
                    // no usable pair: keep the current state; colour off
                    // after 8 lines without a burst (not for the few
                    // lines of the vertical blanking)
                    if (!vb) begin
                        if (nob_cnt != 4'hf)
                            nob_cnt <= nob_cnt + 1'b1;
                        if (nob_cnt >= 4'd7)
                            kill_n <= 1'b1;
                    end
                    if (vb) begin
                        Pr <= Zr;
                        Pi <= Zi;
                        pv <= 1'b1;
                    end else begin
                        pv <= 1'b0;
                    end
                    st <= 7'd0;
                end
            end
            5: begin ma <= Ri; mb <= RPr; nob_cnt <= 4'h0; end
            6: begin ma <= Rr; mb <= RPi; end
            7: begin
                t1 <= mres;         // Ri*RPr
                // r low-pass over lines (1/4)
                Fr <= Fr + ((Rr - Fr) >>> 2);
                Fi <= Fi + ((Ri - Fi) >>> 2);
            end
            8: begin
                dIm <= t1 - mres;   // Im(r * conj(rp))
                ma <= Fr; mb <= Fr;
            end
            9: begin ma <= Fi; mb <= Fi; end
            10: t1 <= mres;
            11: begin
                m <= (t1 + mres) > 48'sh7fffffff ? 32'h7fffffff : t1[31:0] + mres[31:0];
                rem <= 42'h0;
                rec <= 20'h0;
                div_i <= 6'd40;
            end
            12: begin
                kill_n <= (m < KILL_THOLD);
                if (m < KILL_THOLD) begin
                    w_re_n <= 18'h0;
                    w_im_n <= 18'h0;
                    Pr <= Zr;
                    Pi <= Zi;
                    pv <= 1'b1;
                    RPr <= Rr;
                    RPi <= Ri;
                    rpv <= 1'b1;
                    st <= 7'd0;
                end
            end
            13: begin
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
            14: begin
                // dq = Im(r*conj(rp)) / |r|^2 (x2^16, ~radians per line)
                ma <= sat24(dIm >>> 8);
                mb <= {4'h0, rec};
            end
            15: begin ma <= Fr; mb <= {4'h0, rec}; end
            16: begin
                dq <= sat24(mres >>> 16);
                ma <= Fi; mb <= {4'h0, rec};
            end
            17: begin
                ar <= sat24(mres >>> 12);       // conj(F) * rec
                // frequency loop
                if (rpv & ~loop_off) begin
                    if (corr_next > CORR_MAX) begin
                        corr <= CORR_MAX;
                        corr_sat <= 1'b1;
                    end else if (corr_next < -CORR_MAX) begin
                        corr <= -CORR_MAX;
                        corr_sat <= 1'b1;
                    end else begin
                        corr <= corr_next[21:0];
                        corr_sat <= 1'b0;
                    end
                end
            end
            18: begin
                ai <= sat24(-(mres >>> 12));
            end
            // w = a * hc (x1/256)
            19: begin ma <= ar; mb <= hc_re; end
            20: begin ma <= ai; mb <= hc_im; end
            21: begin t1 <= mres; ma <= ar; mb <= hc_im; end
            22: begin w_re_n <= sat18((t1 - mres) >>> 8); ma <= ai; mb <= hc_re; end
            23: t1 <= mres;
            24: begin
                w_im_n <= sat18((t1 + mres) >>> 8);
                Pr <= Zr;
                Pi <= Zi;
                pv <= 1'b1;
                RPr <= Rr;
                RPi <= Ri;
                rpv <= 1'b1;
                st <= 7'd0;
            end
            default: st <= 7'd0;
        endcase
    end

    // new reference from the next line on (burst gate start)
    if (gate_z & ~gate_z_prev) begin
        w_re <= w_re_n;
        w_im <= w_im_n;
        kill <= kill_n;
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

    // U + jV from Z * w (1/2^20); U = -Re, V = -s*Im
    q1 <= zI * w_re;
    q2 <= zQ * w_im;
    q3 <= zI * w_im;
    q4 <= zQ * w_re;
    zre <= q1 - q2;
    zim <= q3 + q4;
    s_d <= s_cur;
    U3 <= sat11(-(zre >>> 20));
    V3 <= s_d ? sat11(-(zim >>> 20)) : sat11(zim >>> 20);
end

// ---------------------------------------------------------------------------
// stage A: luma, chroma off when killed
reg signed [10:0] yA;
always @(posedge clk) begin
    if (svideo | nonotch | kill)
        yA <= $signed({3'b0, x0d[21]}) - $signed({3'b0, blank});
    else
        yA <= $signed({3'b0, x0d[21]}) - $signed({3'b0, blank}) - rmod;
    UA <= kill ? 11'sd0 : U3;
    VA <= kill ? 11'sd0 : V3;
end

// stage B: luma gain (white 160 -> 255), brightness, YUV -> RGB products
wire signed [17:0] yB_p = yA * 7'sd51;
reg signed [11:0] yB;
reg signed [21:0] pRV, pGU, pGV, pBU;
always @(posedge clk) begin
    yB <= (yB_p >>> 5) + {{5{bright[5]}}, bright, 1'b0};
    pRV <= VA * 10'sd292;   // 1.140
    pGU <= UA * 10'sd101;   // 0.395
    pGV <= VA * 10'sd149;   // 0.581
    pBU <= UA * 11'sd520;   // 2.032
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
wire [17:0] absFr = Fr[18] ? -Fr[17:0] : Fr[17:0];
wire [17:0] absFi = Fi[18] ? -Fi[17:0] : Fi[17:0];
wire [18:0] fmag = (absFr > absFi) ? ({1'b0, absFr} + {2'b0, absFi[17:1]}) : ({1'b0, absFi} + {2'b0, absFr[17:1]});
always @(posedge clk) begin
    if (vs_fall) begin
        peak_l <= peak;
        peak <= 8'h0;
    end else if (vs0 && (h > 12'd250) && (h < 12'd1650) && (x0d[1] > peak)) begin
        peak <= x0d[1];
    end
    status <= {corr[21:17], corr_sat, alt_ok, kill, peak_l, blank, (fmag[18:14] != 0) ? 8'hff : fmag[13:6]};
end


// ---------------------------------------------------------------------------
// power-up values (also for simulation: the running sums need a defined start)
integer j;
initial begin
    ph = 0; corr = 0; st = 0; pv = 0; rpv = 0; vb = 0;
    Fr = 0; Fi = 0; Pr = 0; Pi = 0; RPr = 0; RPi = 0; Rr = 0; Ri = 0;
    w_re = 0; w_im = 0; w_re_n = 0; w_im_n = 0; kill = 1; kill_n = 1;
    s_cur = 1; s_meas_prev = 0; s_meas_pv = 0; corr_sat = 0; nob_cnt = 0;
    alt_cnt = 0; bad_cnt = 0; alt_ok = 0; peak = 0; peak_l = 0;
    bl_acc = 0; bl_line = 0; accI = 0; accQ = 0; gate_zd = 0;
    b6I = 0; b6Q = 0; b7I = 0; b7Q = 0; b8I = 0; b8Q = 0; pI = 0; pQ = 0;
    h = 0; c_bp = 0; sc_q = 0; x0 = 0; c0 = 0; hs0 = 1; hs0_prev = 1; vs0 = 1; vs0_prev = 1;
    for (j=1; j<=6; j=j+1) begin pI_d[j] = 0; pQ_d[j] = 0; end
    for (j=1; j<=7; j=j+1) begin b6I_d[j] = 0; b6Q_d[j] = 0; end
    for (j=1; j<=8; j=j+1) begin b7I_d[j] = 0; b7Q_d[j] = 0; cw[j] = 0; end
    for (j=1; j<=13; j=j+1) scd[j] = 0;
    for (j=1; j<=21; j=j+1) x0d[j] = 0;
end

endmodule

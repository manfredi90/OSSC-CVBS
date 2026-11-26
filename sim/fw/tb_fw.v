`timescale 1ns/1ps
module tb;
reg TVP_PCLK_i = 0; always #18.5 TVP_PCLK_i = ~TVP_PCLK_i;
wire [31:0] sl_config2 = 32'h00002100;   // cfg[0] and cfg[5]: flywheel on
integer VA, VP, VJ, MISS, MV, NL, tc = 0, k, ln, start, off, nacc = 0, ngen = 0, err, maxerr = 0, p;
reg [63:0] rs;
reg hs_in = 1;
reg TVP_HS_raw = 1, fw_hs_prev, fw_locked;
reg [11:0] fw_cnt, fw_len;
reg [3:0] fw_good;
wire fw_en = sl_config2[8] & sl_config2[13];
wire fw_edge = fw_hs_prev & ~TVP_HS_raw;
wire [12:0] fw_diff = {1'b0, fw_cnt} - {1'b0, fw_len};
wire fw_near = (fw_diff[12] ? (fw_diff[11:0] > 12'hff7) : (fw_diff[11:0] < 12'd9));
wire fw_accept = fw_edge & (~fw_locked | (fw_cnt >= fw_len - {3'b0, fw_len[11:3]}));
// (VHS firmware: missing edge generated 1/8 line after the expected end, the
// late side of the window as wide as the early one; CVBS: 4 clocks, a tape's
// longer lines got a generated edge and their real one was dropped: lines
// out of place, jagged vertical edges)
wire fw_gen = fw_locked & (fw_cnt == fw_len + {3'b0, fw_len[11:3]});
always @(posedge TVP_PCLK_i) begin
    fw_hs_prev <= TVP_HS_raw;
    if (fw_accept | fw_gen) begin
        fw_cnt <= 12'd0;
        if (fw_accept) begin
            if (fw_near | ~fw_locked)
                fw_len <= fw_cnt;
            if (~fw_locked) begin
                fw_good <= fw_near ? fw_good + 1'b1 : 4'd0;
                fw_locked <= fw_near & (fw_good == 4'd7);
            end
        end
    end else if (fw_cnt != 12'hfff)
        fw_cnt <= fw_cnt + 1'b1;
    if (~fw_en) begin
        fw_locked <= 1'b0;
        fw_good <= 4'd0;
    end

    TVP_HS_raw <= hs_in;
end
integer lastgen = -1, laststart = 0;
always @(posedge TVP_PCLK_i) begin
    tc <= tc + 1;
    if (fw_locked & fw_accept) begin nacc = nacc + 1; err = tc - laststart; if (err < 0) err = -err; if (err > maxerr && err < 100) maxerr = err; end
    if (fw_gen) ngen = ngen + 1;
end
initial begin
    if (!$value$plusargs("VA=%d", VA)) VA = 0;
    if (!$value$plusargs("VP=%d", VP)) VP = 200;
    if (!$value$plusargs("VJ=%d", VJ)) VJ = 0;
    if (!$value$plusargs("MISS=%d", MISS)) MISS = 0;
    if (!$value$plusargs("MV=%d", MV)) MV = 0;
    if (!$value$plusargs("NL=%d", NL)) NL = 3000;
    for (ln = 0; ln < NL; ln = ln + 1) begin
        off = $rtoi(VA * $sin(6.283185307 * ln / VP));
        if (VJ > 0) off = off + ($urandom % (2*VJ+1)) - VJ;
        start = 1000 + ln * 1728 + off;
        while (tc < start) @(posedge TVP_PCLK_i);
        laststart = start;
        if (!(MISS > 0 && (ln % MISS) == 0)) begin hs_in = 0; repeat (127) @(posedge TVP_PCLK_i); hs_in = 1; end
        if (MV && (ln % 312) < 15) for (p = 270; p < 1500; p = p + 216) begin
            while (tc < start + p) @(posedge TVP_PCLK_i);
            hs_in = 0; repeat (62) @(posedge TVP_PCLK_i); hs_in = 1;
        end
    end
    $display("accepted %0d generated %0d max offset of accepted edges vs real %0d", nacc, ngen, maxerr);
    $finish;
end
endmodule

//
// Copyright (C) 2015-2019  Markus Hiienkari <mhiienka@niksula.hut.fi>
//
// This file is part of Open Source Scan Converter project.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

//`define DEBUG
`define PO_RESET_WIDTH 27  //1us

module ossc (
    input clk27,

    inout scl,
    inout sda,

    input ir_rx,
    input [1:0] btn,
    input [1:0] cfg,

    input TVP_PCLK_i,
    input [7:0] TVP_R_i,
    input [7:0] TVP_G_i,
    input [7:0] TVP_B_i,
    input TVP_HS_i,
    input TVP_SOG_i,
    input TVP_VSYNC_i,

    output HDMI_TX_PCLK,
    output reg [7:0] HDMI_TX_RD,
    output reg [7:0] HDMI_TX_GD,
    output reg [7:0] HDMI_TX_BD,
    output reg HDMI_TX_DE,
    output reg HDMI_TX_HS,
    output reg HDMI_TX_VS,
    input HDMI_TX_INT_N,

    output hw_reset_n,

    output LED_G,
    inout LED_R,

    output LCD_RS,
    output LCD_CS_N,
    output LCD_BL,

    output SD_CLK,
    inout SD_CMD,
    inout [3:0] SD_DAT
);


wire [31:0] sys_ctrl;
wire lt_active = 1'b0;   // latency tester removed (downscaler build)
wire tvp_vsync_type = sys_ctrl[10];
wire pll_bypass = sys_ctrl[9];
wire remote_event = sys_ctrl[8];
assign SD_DAT[3] = sys_ctrl[7]; //SD_SPI_SS_N
// SD card SPI controller removed (SD card not used by the downscaler firmware)
assign SD_CLK = 1'b0;
assign SD_CMD = 1'bz;
assign LCD_CS_N = sys_ctrl[6];
assign LCD_RS = sys_ctrl[5];
wire lcd_bl_on = sys_ctrl[4];    //hw_reset_n in v1.2 PCB
wire [1:0] lcd_bl_time = sys_ctrl[3:2];
wire enable_sc = sys_ctrl[1];
assign hw_reset_n = sys_ctrl[0];   //HDMI_TX_RST_N in v1.2 PCB

wire [31:0] hv_in_config, hv_in_config2, hv_in_config3, hv_out_config, hv_out_config2, hv_out_config3, xy_out_config, xy_out_config2;
wire [31:0] misc_config, sl_config, sl_config2, sl_config3;

// Unused stock features are tied off here so synthesis prunes them:
// Lumacode + reverse LPF (frontend), scanlines + shadow mask + Panasonic hack
// (scan converter), latency tester.
wire [31:0] misc_config_fe = {misc_config[31:15], 3'h0, 5'h0, misc_config[6:0]};
wire [31:0] misc_config_sc = {misc_config[31:25], 1'b0, 8'h0, 1'b0, misc_config[14:0]};

// CVBS firmware: the downscaler is not used, its controls are tied off so
// that synthesis prunes it (15kHz passthrough only). The registers that
// carried them now configure the colour decoder:
//   sl_config  = chroma coefficient (hc) and luma gain
//   sl_config2 = [5:0] horizontal timing extension bits, [31:8] decoder flags
//   sl_config3 = DDS increment
wire [2:0] ds_vdec = 3'h0;
// HDMI RGB range: 0 = full 0-255, 1 = limited 16-235, 2 = 16-255 (DACs that
// clip below 16). misc_config[8:7]: the stock reverse LPF bits, not used
// here (masked off for the frontend)
wire [1:0] out_range = misc_config[8:7];
wire [1:0] ds_lb_layout = 2'h0;
wire [1:0] ds_hfilt = 2'h0;
wire ds_ilace = 1'b0;
wire ds_fswap = 1'b0;
wire ds_i2p = 1'b0;
wire ds_vflt = 1'b0;
wire [1:0] ds_vflt_bofs = 2'h0;
wire [4:0] ds_vflt_a = 5'h0;
wire ds_w31 = 1'b0;
wire [9:0] ds_lb_stride = 10'h0;
wire [5:0] ds_lb_slots = 6'h0;
wire ds_lb_rgb = 1'b0;
wire [12:0] ds_hofs = 13'h0;
wire [7:0] ds_resync_cnt;
wire [9:0] ds_fperiod_lo;
wire [9:0] ds_field_cfg = 10'h0;

wire pll_clkout, pll_clkswitch, pll_locked;
wire clkmux_clkout;

wire [15:0] ir_code;
wire [7:0] ir_code_cnt;

wire [7:0] R_sc, G_sc, B_sc;
wire HSYNC_sc, VSYNC_sc, DE_sc, CSYNC_sc;
wire pll_areset, pll_scanclk, pll_scanclkena, pll_configupdate, pll_scandata, pll_scandone, pll_activeclock;
wire PCLK_sc;
wire pclk_out = PCLK_sc;


reg [7:0] po_reset_ctr = 0;
reg po_reset_n = 1'b0;
wire jtagm_reset_req = 1'b0;   // JTAG debug master removed
wire ndmreset_req;
reg ndmreset_ack, ndmreset_pulse;
wire sys_reset_n = (po_reset_n & ~jtagm_reset_req & ~ndmreset_pulse);

reg [7:0] TVP_R, TVP_G, TVP_B;
reg TVP_HS, TVP_VS, TVP_FID;
reg TVP_VS_sync1_reg, TVP_VS_sync2_reg;
reg TVP_SOG_sync1_reg, TVP_SOG_sync2_reg, TVP_SOG_prev;
reg TVP_HSYNC_sync1_reg, TVP_HSYNC_sync2_reg;
reg TVP_VSYNC_sync1_reg, TVP_VSYNC_sync2_reg;

reg [1:0] btn_L, btn_LL;
reg ir_rx_L, ir_rx_LL, HDMI_TX_INT_N_L, HDMI_TX_INT_N_LL;
reg vsync_flag_sync1_reg, vsync_flag_sync2_reg;
reg [1:0] cfg_reg;
reg cfg_stored;
wire TVP_SOG_ALT_i;
wire TVP_HSYNC_i = cfg_reg[1] ? TVP_SOG_i : TVP_SOG_ALT_i;

reg [23:0] resync_led_ctr, warn_pll_lock_lost;
reg resync_strobe_sync1_reg, resync_strobe_sync2_reg, resync_strobe_prev;
wire resync_strobe_i;
wire resync_strobe = resync_strobe_sync2_reg;

wire [31:0] controls = {ir_code_cnt, 3'b000, vsync_flag_sync2_reg, pll_activeclock, cfg_reg[0], btn_LL, ir_code};

wire lt_sensor = btn_LL[1];
wire [1:0] lt_mode_synced = 2'h0;
wire [15:0] lt_lat_result = 16'h0;
wire [11:0] lt_stb_result = 12'h0;
wire lt_trig_waiting = 1'b0;
wire lt_finished = 1'b0;

wire [31:0] lumacode_data;
wire [7:0] lumacode_addr;
wire lumacode_rden;

reg remote_event_prev;
reg [14:0] to_ctr, to_ctr_ms;
wire lcd_bl_timeout;

wire [2:0] osd_color;
wire osd_enable_pre;
wire osd_enable = osd_enable_pre & ~lt_active;
wire [11:0] xpos_sc;
wire [10:0] ypos_sc;
wire [3:0] x_ctr_shmask, y_ctr_shmask;
wire [10:0] shmask_data;

wire resync_indicator = (warn_pll_lock_lost != 0)  | (resync_led_ctr != 0);
wire LED_R_i = lt_active ? lt_trig_waiting : resync_indicator;
assign LED_G = lt_active ? ~lt_sensor : (ir_code == 0) & (~resync_indicator | cfg_reg[1]);

assign LCD_BL = lcd_bl_on ? (~lcd_bl_timeout | lt_active) : 1'b0;
assign HDMI_TX_PCLK = pclk_out;


// HS flywheel, CVBS decoder with the 27MHz crystal clock (cfg[0] enable,
// cfg[5] xtal): the TVP7002 HSOUT is then not regenerated by its H-PLL and
// the half-line pulses of the vertical interval come through (extra lines,
// lines cut short). Once locked (8 lines of the same length), edges before
// 7/8 of the line are ignored and a missing edge is generated 4 clocks after
// the expected end of the line. Restarted at every setup (cfg[5] off for one
// register write).
reg TVP_HS_raw, fw_hs_prev, fw_locked;
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
    TVP_HS <= fw_en ? (fw_cnt >= 12'd32) : TVP_HS_i;     // (same timing as before when off)
end

wire [7:0] lock_err;
wire [7:0] blk_pp;
wire [2:0] dg_sel = {sl_config[14], sl_config2[31:30]};
// VHS diagnostics read by the CPU through the decoder status (cv_status[23:16],
// selected by sl_config2[31:30] / sl_config[14]): 2 = frame lock offset,
// 4 = blank level range (VHS 0.11: the flywheel line length ranges J / A and
// generated edges M removed, FPGA space)
wire [7:0] dg_val = (dg_sel == 3'h2) ? lock_err : blk_pp;


// TVP7002 RGB digitizer
always @(posedge TVP_PCLK_i) begin
    TVP_R <= TVP_R_i;
    TVP_G <= TVP_G_i;
    TVP_B <= TVP_B_i;
    TVP_HS_raw <= TVP_HS_i;
    TVP_VS <= TVP_VSYNC_i;

    // sync to pclk
    TVP_SOG_sync1_reg <= TVP_HSYNC_i;
    TVP_SOG_sync2_reg <= TVP_SOG_sync1_reg;
    TVP_SOG_prev <= TVP_SOG_sync2_reg;
    TVP_VS_sync1_reg <= TVP_VSYNC_i;
    TVP_VS_sync2_reg <= TVP_VS_sync1_reg;
end
always @(posedge clk27) begin
    // sync to always-running fixed meas clk
    TVP_HSYNC_sync1_reg <= TVP_HSYNC_i;
    TVP_HSYNC_sync2_reg <= TVP_HSYNC_sync1_reg;
    TVP_VSYNC_sync1_reg <= TVP_VSYNC_i;
    TVP_VSYNC_sync2_reg <= TVP_VSYNC_sync1_reg;
end

wire [7:0] TVP_R_post, TVP_G_post, TVP_B_post;
wire TVP_HSYNC_post, TVP_VSYNC_post, TVP_DE_post, TVP_FID_post, TVP_datavalid_post;
wire TVP_fe_interlace, TVP_fe_frame_change, TVP_sof_scaler, TVP_sync_active;
wire [19:0] TVP_fe_pcnt_field;
wire [7:0] TVP_hsync_width;
wire [10:0] TVP_fe_vtotal, TVP_fe_xpos, TVP_fe_ypos;
tvp7002_frontend u_tvp_frontend ( 
    .PCLK_i(TVP_PCLK_i),
    .CLK_MEAS_i(clk27),
    .reset_n(sys_reset_n),
    .R_i(TVP_R),
    .G_i(TVP_G),
    .B_i(TVP_B),
    .HS_i(TVP_HS),
    .VS_i(TVP_VS_sync2_reg),
    .HSYNC_i(TVP_HSYNC_sync2_reg),
    .VSYNC_i(TVP_VSYNC_sync2_reg),
    .DE_i(1'b0),
    .FID_i(1'b0),
    .sogref_update_i(TVP_SOG_prev & ~TVP_SOG_sync2_reg),
    .vsync_i_type(tvp_vsync_type),
    .hv_in_config(hv_in_config),
    .hv_in_config2(hv_in_config2),
    .hv_in_config3(hv_in_config3),
    .misc_config(misc_config_fe),
    .lumacode_data(32'h0),
    .R_o(TVP_R_post),
    .G_o(TVP_G_post),
    .B_o(TVP_B_post),
    .HSYNC_o(TVP_HSYNC_post),
    .VSYNC_o(TVP_VSYNC_post),
    .DE_o(TVP_DE_post),
    .FID_o(TVP_FID_post),
    .interlace_flag(TVP_fe_interlace),
    .datavalid_o(TVP_datavalid_post),
    .xpos_o(TVP_fe_xpos),
    .ypos_o(TVP_fe_ypos),
    .vtotal(TVP_fe_vtotal),
    .frame_change(TVP_fe_frame_change),
    .sof_scaler(TVP_sof_scaler),
    .pcnt_field(TVP_fe_pcnt_field),
    .hsync_width(TVP_hsync_width),
    .sync_active(TVP_sync_active),
    .lumacode_addr(lumacode_addr),
    .lumacode_rden(lumacode_rden)
);

// Composite / S-Video colour decoder (bypassed for the other inputs, same
// latency): video and every frontend signal the scan converter uses go
// through it together
wire [7:0] R_dec, G_dec, B_dec;
wire HSYNC_dec, VSYNC_dec, DE_dec, FID_dec, datavalid_dec, frame_change_dec;
wire [10:0] xpos_dec, ypos_dec;
wire [31:0] cv_status;
cvbs_decoder #(
    .SIDE_W(28)
) u_cvbs (
    .clk(TVP_PCLK_i),
    .R_i(TVP_R_post),
    .G_i(TVP_G_post),
    .B_i(TVP_B_post),
    .HS_i(TVP_HSYNC_post),
    .VS_i(TVP_VSYNC_post),
    .side_i({TVP_fe_frame_change, TVP_FID_post, TVP_datavalid_post, TVP_DE_post, TVP_VSYNC_post, TVP_HSYNC_post, TVP_fe_xpos, TVP_fe_ypos}),
    .cfg(sl_config2[31:8]),
    .hc(sl_config),
    .inc(sl_config3),
    .R_o(R_dec),
    .G_o(G_dec),
    .B_o(B_dec),
    .side_o({frame_change_dec, FID_dec, datavalid_dec, DE_dec, VSYNC_dec, HSYNC_dec, xpos_dec, ypos_dec}),
    .status(cv_status),
    .blk_pp(blk_pp)
);

// diagnostics: input frames (decoder frame change strobes)
reg fc_dec_prev;
reg [3:0] fc_cnt;
always @(posedge TVP_PCLK_i) begin
    fc_dec_prev <= frame_change_dec;
    if (frame_change_dec & ~fc_dec_prev)
        fc_cnt <= fc_cnt + 1'b1;
end


// Insert synchronizers to async inputs (synchronize to CPU clock)
always @(posedge clk27 or negedge po_reset_n)
begin
    if (!po_reset_n) begin
        {btn_L, btn_LL} <= '0;
        {ir_rx_L, ir_rx_LL} <= '0;
        {HDMI_TX_INT_N_L, HDMI_TX_INT_N_LL} <= '0;
        {cfg_stored, cfg_reg} <= '0;
    end else begin
        {btn_L, btn_LL} <= {btn, btn_L};
        {ir_rx_L, ir_rx_LL} <=  {ir_rx, ir_rx_L};
        {HDMI_TX_INT_N_L, HDMI_TX_INT_N_LL} <= {HDMI_TX_INT_N, HDMI_TX_INT_N_L};
        if (!cfg_stored)
            cfg_reg <= cfg;
        cfg_stored <= 1'b1;
    end
end

// Power-on reset pulse generation (not strictly necessary)
always @(posedge clk27)
begin
    if (po_reset_ctr == `PO_RESET_WIDTH)
        po_reset_n <= 1'b1;
    else
        po_reset_ctr <= po_reset_ctr + 1'b1;
end

// ndmreset pulse & ack for RISC-V DM
always @(posedge clk27)
begin
    ndmreset_pulse <= !ndmreset_ack & ndmreset_req;
    ndmreset_ack <= ndmreset_req;
end

// Sync vsync flag to CPU clock
always @(posedge clk27) begin
    {vsync_flag_sync1_reg, vsync_flag_sync2_reg} <= {~VSYNC_sc, vsync_flag_sync1_reg};
end

// LCD backlight timeout counters
always @(posedge clk27)
begin
    if (remote_event != remote_event_prev) begin
        to_ctr <= 15'd0;
        to_ctr_ms <= 15'd0;
    end else begin
        if (to_ctr == 27000-1) begin
            to_ctr <= 0;
            if (to_ctr_ms < 15'h7fff)
                to_ctr_ms <= to_ctr_ms + 1'b1;
        end else begin
            to_ctr <= to_ctr + 1'b1;
        end
    end

    case (lcd_bl_time)
        default: lcd_bl_timeout <= 0; //off
        2'b01:  lcd_bl_timeout <= (to_ctr_ms >= 3000);  //3s
        2'b10:  lcd_bl_timeout <= (to_ctr_ms >= 10000); //10s
        2'b11:  lcd_bl_timeout <= (to_ctr_ms >= 30000); //30s
    endcase

    remote_event_prev <= remote_event;
end

// Generate a warning signal from sync lock loss
always @(posedge clk27) begin
    if (enable_sc) begin
        if (~resync_strobe_prev & resync_strobe) begin
            resync_led_ctr <= {24{1'b1}};
        end else if (resync_led_ctr > 0) begin
            resync_led_ctr <= resync_led_ctr - 1'b1;
        end
    end

    resync_strobe_sync1_reg <= resync_strobe_i;
    resync_strobe_sync2_reg <= resync_strobe_sync1_reg;
    resync_strobe_prev <= resync_strobe_sync2_reg;
end

// Generate a warning signal from PLL lock loss
always @(posedge clk27 or negedge sys_reset_n)
begin
    if (!sys_reset_n) begin
        warn_pll_lock_lost <= 1'b0;
    end else begin
        if (~pll_areset & ~pll_locked)
            warn_pll_lock_lost <= 1;
        else if (warn_pll_lock_lost != 0)
            warn_pll_lock_lost <= warn_pll_lock_lost + 1'b1;
    end
end

// Control PLL reference clock switchover (VHS firmware: always the 27MHz
// reference, inclk0: the output clock does not move when the source comes
// and goes, stock: the TVP7002 clock while the scan converter is enabled)
always @(posedge clk27)
begin
    pll_clkswitch <= (pll_activeclock != 1'b0);
end

// (VHS firmware: HDMI only, no 15kHz output watchdog)

// Output registers: 3 stages (select, range scaling product, output pins);
// syncs and DE follow with the same latency
reg [7:0] R_o1, G_o1, B_o1, R_o2, G_o2, B_o2;
reg [15:0] R_p2, G_p2, B_p2;
reg HS_o1, VS_o1, DE_o1, HS_o2, VS_o2, DE_o2;
reg [1:0] out_range_r;
wire [7:0] range_k = (out_range_r == 2'h2) ? 8'd240 : 8'd220;   // 16-255 / 16-235
always @(posedge pclk_out) begin
    out_range_r <= out_range;

    // stage 1: picture / OSD
    if (osd_enable) begin
        {R_o1, G_o1, B_o1} <= {{8{osd_color[2]}}, {8{osd_color[1]}}, {8{osd_color[0]}}};
    end else begin
        {R_o1, G_o1, B_o1} <= {R_sc, G_sc, B_sc};
    end

    // separate syncs (HDMI), active low
    HS_o1 <= HSYNC_sc;
    VS_o1 <= VSYNC_sc;
    DE_o1 <= DE_sc;

    // stage 2: range scaling 16 + round(v * k / 256)
    {R_o2, G_o2, B_o2} <= {R_o1, G_o1, B_o1};
    R_p2 <= R_o1 * range_k + 16'd128;
    G_p2 <= G_o1 * range_k + 16'd128;
    B_p2 <= B_o1 * range_k + 16'd128;
    {HS_o2, VS_o2, DE_o2} <= {HS_o1, VS_o1, DE_o1};

    // stage 3: output pins
    if (out_range_r == 2'h0)
        {HDMI_TX_RD, HDMI_TX_GD, HDMI_TX_BD} <= {R_o2, G_o2, B_o2};
    else
        {HDMI_TX_RD, HDMI_TX_GD, HDMI_TX_BD} <= {8'd16 + R_p2[15:8], 8'd16 + G_p2[15:8], 8'd16 + B_p2[15:8]};
    {HDMI_TX_HS, HDMI_TX_VS, HDMI_TX_DE} <= {HS_o2, VS_o2, DE_o2};
end


// A modified <= v1.7 board uses LED_R for TVP_HSYNC input
ALT_IOBUF led_r_iobuf (.i(LED_R_i), .oe(cfg_reg[1]), .o(TVP_SOG_ALT_i), .io(LED_R));

pll_2x pll_pclk (
    .areset(pll_areset),
    .clkswitch(pll_clkswitch),
    .configupdate(pll_configupdate),
    .inclk0(clk27), // set videogen clock to primary (power-on default) since both reference clocks must be running during switchover
    .inclk1(TVP_PCLK_i), // is the secondary input clock fully compensated?
    .scanclk(pll_scanclk),
    .scanclkena(pll_scanclkena),
    .scandata(pll_scandata),
    .activeclock(pll_activeclock),
    .c0(pll_clkout),
    .locked(pll_locked),
    .scandataout(),
    .scandone(pll_scandone)
);

cycloneive_clkctrl clkctrl1 ( 
    .clkselect(pll_bypass ? 2'h0 : 2'h2),
    .ena(1'b1),
    .inclk({1'b0, pll_clkout, 1'b0, TVP_PCLK_i}), // fitter forbids using both clk27 and pclk_1x here since they're on opposite sides
    .outclk(clkmux_clkout)
// synopsys translate_off
    ,
    .devclrn(1'b1),
    .devpor(1'b1)
// synopsys translate_on
);
defparam
    clkctrl1.clock_type = "Global Clock",
    clkctrl1.ena_register_mode = "falling edge",
    clkctrl1.lpm_type = "cycloneive_clkctrl";

// Remote update hard block alone (1.0.1): the project keeps the remote
// configuration mode of 1.0 (the image is started by the official firmware's
// remote update, "Launch 2nd FW"), which needs the block in the design; the
// Avalon remote update IP around it (~230 logic elements) was never used by
// this firmware. Inputs idle, the output kept on an unused status bit.
wire ru_regout;
cycloneive_rublock ru_block (
    .clk(1'b0),
    .shiftnld(1'b0),
    .captnupdt(1'b0),
    .regin(1'b0),
    .rsttimer(1'b0),
    .rconfig(1'b0),
    .regout(ru_regout)
);

sys sys_inst(
    .clk_clk                                (clk27),
    .reset_sys_reset_n                      (sys_reset_n),
    .reset_po_reset_n                       (po_reset_n),
    .ibex_0_ndm_ndmreset_o                  (ndmreset_req),
    .ibex_0_ndm_ndmreset_ack_i              (ndmreset_ack),
    .ibex_0_config_boot_addr_i              (32'h020D0000),
    .ibex_0_config_core_sleep_o             (),
    .i2c_opencores_0_export_scl_pad_io      (scl),
    .i2c_opencores_0_export_sda_pad_io      (sda),
    .i2c_opencores_0_export_spi_miso_pad_i  (1'b0),
    .pio_0_sys_ctrl_out_export              (sys_ctrl),
    .pio_1_controls_in_export               (controls),
    // CVBS firmware: the downscaler diagnostics (resync count, frame period) are not
    // read: tied off (logic for the decoder)
    .sc_config_0_sc_if_fe_status_i          ({5'h0, ru_regout, fc_cnt, ds_resync_cnt, 1'b0, TVP_sync_active, TVP_fe_interlace, TVP_fe_vtotal}),
    .sc_config_0_sc_if_fe_status2_i         ({4'h0, TVP_hsync_width, TVP_fe_pcnt_field}),
    .sc_config_0_sc_if_lt_status_i          ({cv_status[31:24], dg_val, cv_status[15:0]}),     // colour decoder status, VHS diagnostics
    .sc_config_0_sc_if_hv_in_config_o       (hv_in_config),
    .sc_config_0_sc_if_hv_in_config2_o      (hv_in_config2),
    .sc_config_0_sc_if_hv_in_config3_o      (hv_in_config3),
    .sc_config_0_sc_if_hv_out_config_o      (hv_out_config),
    .sc_config_0_sc_if_hv_out_config2_o     (hv_out_config2),
    .sc_config_0_sc_if_hv_out_config3_o     (hv_out_config3),
    .sc_config_0_sc_if_xy_out_config_o      (xy_out_config),
    .sc_config_0_sc_if_xy_out_config2_o     (xy_out_config2),
    .sc_config_0_sc_if_misc_config_o        (misc_config),
    .sc_config_0_sc_if_sl_config_o          (sl_config),
    .sc_config_0_sc_if_sl_config2_o         (sl_config2),
    .sc_config_0_sc_if_sl_config3_o         (sl_config3),
    .sc_config_0_shmask_if_vclk             (PCLK_sc),
    .sc_config_0_shmask_if_shmask_xpos      (x_ctr_shmask),
    .sc_config_0_shmask_if_shmask_ypos      (y_ctr_shmask),
    .sc_config_0_shmask_if_shmask_data      (shmask_data),
    .sc_config_0_lc_ram_if_lumacode_clk_i   (TVP_PCLK_i),
    .sc_config_0_lc_ram_if_lumacode_addr_i  (lumacode_addr),
    .sc_config_0_lc_ram_if_lumacode_rden_i  (lumacode_rden),
    .sc_config_0_lc_ram_if_lumacode_data_o  (lumacode_data),
    .osd_generator_0_osd_if_vclk            (PCLK_sc),
    .osd_generator_0_osd_if_xpos            (xpos_sc),
    .osd_generator_0_osd_if_ypos            (ypos_sc),
    .osd_generator_0_osd_if_osd_enable      (osd_enable_pre),
    .osd_generator_0_osd_if_osd_color       (osd_color),
    .pll_reconfig_0_pll_reconfig_if_areset       (pll_areset),
    .pll_reconfig_0_pll_reconfig_if_scanclk      (pll_scanclk),
    .pll_reconfig_0_pll_reconfig_if_scanclkena   (pll_scanclkena),
    .pll_reconfig_0_pll_reconfig_if_configupdate (pll_configupdate),
    .pll_reconfig_0_pll_reconfig_if_scandata     (pll_scandata),
    .pll_reconfig_0_pll_reconfig_if_scandone     (pll_scandone)
);


scanconverter #(
    .EMIF_ENABLE(0),
    .NUM_LINE_BUFFERS(16)   // passthrough only uses the LSB (2 lines, line buffer RAM 4096 words)
  ) scanconverter_inst (
    .PCLK_CAP_i(TVP_PCLK_i),
    .PCLK_OUT_i(clkmux_clkout),
    .reset_n(hw_reset_n),  //TODO: sync to pclk_capture
    .R_i(R_dec),
    .G_i(G_dec),
    .B_i(B_dec),
    .HSYNC_i(HSYNC_dec),
    .VSYNC_i(VSYNC_dec),
    .DE_i(DE_dec),
    .FID_i(FID_dec),
    .datavalid_i(datavalid_dec),
    .interlaced_in_i(TVP_fe_interlace),
    .frame_change_i(frame_change_dec),
    .xpos_i(xpos_dec),
    .ypos_i(ypos_dec),
    .h_in_active(hv_in_config[23:12]),
    .hv_out_config(hv_out_config),
    .hv_out_config2(hv_out_config2),
    .hv_out_config3(hv_out_config3),
    .xy_out_config(xy_out_config),
    .xy_out_config2(xy_out_config2),
    .hv_ext_config(sl_config2),
    .misc_config(misc_config_sc),
    .sl_config(32'h0),
    .sl_config2(32'h0),
    .sl_config3(32'h0),
    .testpattern_enable(~enable_sc),
    .lb_enable(enable_sc),
    .ds_vdec(ds_vdec),
    .lb_layout(ds_lb_layout),
    .ds_hfilt(ds_hfilt),
    .ds_field_cfg(ds_field_cfg),
    .ds_ilace(ds_ilace),
    .ds_fswap(ds_fswap),
    .ds_i2p(ds_i2p),
    .ds_vflt(ds_vflt),
    .ds_vflt_bofs(ds_vflt_bofs),
    .ds_vflt_a(ds_vflt_a),
    .ds_w31(ds_w31),
    .lb_stride(ds_lb_stride),
    .lb_slots(ds_lb_slots),
    .lb_rgb(ds_lb_rgb),
    .ds_hofs(ds_hofs),
    .resync_always(fw_en & sl_config2[15]),    // cfg[7]: test option
    .lock_step(sl_config2[7:6]),
    .ext_sync_mode(1'b0),
    .ext_frame_change_i(1'b0),
    .ext_R_i(8'h00),
    .ext_G_i(8'h00),
    .ext_B_i(8'h00),
    .PCLK_o(PCLK_sc),
    .R_o(R_sc),
    .G_o(G_sc),
    .B_o(B_sc),
    .HSYNC_o(HSYNC_sc),
    .CSYNC_o(CSYNC_sc),
    .VSYNC_o(VSYNC_sc),
    .DE_o(DE_sc),
    .xpos_o(xpos_sc),
    .ypos_o(ypos_sc),
    .x_ctr_shmask(x_ctr_shmask),
    .y_ctr_shmask(y_ctr_shmask),
    .shmask_data(12'h0),
    .resync_strobe(resync_strobe_i),
    .ds_resync_cnt(ds_resync_cnt),
    .ds_fperiod_lo(ds_fperiod_lo),
    .lock_err(lock_err),
    .lt_active(lt_active),
    .lt_mode(lt_mode_synced),
    .emif_br_clk(1'b0),
    .emif_br_reset(1'b0),
    .emif_rd_addr(),
    .emif_rd_read(),
    .emif_rd_rdata(0),
    .emif_rd_waitrequest(0),
    .emif_rd_readdatavalid(0),
    .emif_rd_burstcount(),
    .emif_wr_addr(),
    .emif_wr_write(),
    .emif_wr_wdata(),
    .emif_wr_waitrequest(0),
    .emif_wr_burstcount()
);

ir_rcv ir0 (
    .clk27          (clk27),
    .reset_n        (po_reset_n),
    .ir_rx          (ir_rx_LL),
    .ir_code        (ir_code),
    .ir_code_ack    (),
    .ir_code_cnt    (ir_code_cnt)
);



endmodule

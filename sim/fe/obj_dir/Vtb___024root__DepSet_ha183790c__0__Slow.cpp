// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb.h for the primary calling header

#include "Vtb__pch.h"
#include "Vtb___024root.h"

VL_ATTR_COLD void Vtb___024root___eval_static__TOP(Vtb___024root* vlSelf);

VL_ATTR_COLD void Vtb___024root___eval_static(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_static\n"); );
    // Body
    Vtb___024root___eval_static__TOP(vlSelf);
}

VL_ATTR_COLD void Vtb___024root___eval_static__TOP(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_static__TOP\n"); );
    // Body
    vlSelf->tb__DOT__clk = 0U;
    vlSelf->tb__DOT__sog = 1U;
    vlSelf->tb__DOT__vs = 1U;
    vlSelf->tb__DOT__k = 0U;
    vlSelf->tb__DOT__tc = 0U;
    vlSelf->tb__DOT__hs_s = 7U;
    vlSelf->tb__DOT__vs_s = 7U;
    vlSelf->tb__DOT__vsp = 1U;
}

VL_ATTR_COLD void Vtb___024root___eval_initial__TOP(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial__TOP\n"); );
    // Init
    IData/*31:0*/ tb__DOT__LINE;
    tb__DOT__LINE = 0;
    // Body
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"PAL=%d"}, 
                                vlSelf->tb__DOT__PAL))) {
        vlSelf->tb__DOT__PAL = 1U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"MV=%d"}, 
                                vlSelf->tb__DOT__MV))) {
        vlSelf->tb__DOT__MV = 0U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"NF=%d"}, 
                                vlSelf->tb__DOT__NF))) {
        vlSelf->tb__DOT__NF = 0x10U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"VA=%d"}, 
                                vlSelf->tb__DOT__VA))) {
        vlSelf->tb__DOT__VA = 0U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"VP=%d"}, 
                                vlSelf->tb__DOT__VP))) {
        vlSelf->tb__DOT__VP = 0xc8U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"VHS=%d"}, 
                                vlSelf->tb__DOT__VHS))) {
        vlSelf->tb__DOT__VHS = 0U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"VJ=%d"}, 
                                vlSelf->tb__DOT__VJ))) {
        vlSelf->tb__DOT__VJ = 0U;
    }
    if ((0U != vlSelf->tb__DOT__PAL)) {
        tb__DOT__LINE = 0x6c0U;
        vlSelf->tb__DOT__FHL = 0x271U;
        vlSelf->tb__DOT__NEQ = 5U;
    } else {
        tb__DOT__LINE = 0x6b4U;
        vlSelf->tb__DOT__FHL = 0x20dU;
        vlSelf->tb__DOT__NEQ = 6U;
    }
    vlSelf->tb__DOT__HL = VL_DIVS_III(32, tb__DOT__LINE, (IData)(2U));
}

VL_ATTR_COLD void Vtb___024root___eval_final(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb___024root___dump_triggers__stl(Vtb___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb___024root___eval_phase__stl(Vtb___024root* vlSelf);

VL_ATTR_COLD void Vtb___024root___eval_settle(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_settle\n"); );
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelf->__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY((0x64U < __VstlIterCount))) {
#ifdef VL_DEBUG
            Vtb___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("tb_fe.v", 9, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtb___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb___024root___dump_triggers__stl(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb___024root___stl_sequent__TOP__0(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___stl_sequent__TOP__0\n"); );
    // Body
    vlSelf->tb__DOT__dut__DOT__meas_even_max_thold 
        = (0xfffU & (VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 1U) 
                     + VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U)));
    vlSelf->tb__DOT__dut__DOT__meas_h_cnt_ref = vlSelf->tb__DOT__dut__DOT__meas_h_cnt_sogref;
    vlSelf->tb__DOT__dut__DOT__HSYNC_i_np = (1U ^ ((IData)(vlSelf->tb__DOT__dut__DOT__hsync_i_pol) 
                                                   ^ 
                                                   ((IData)(vlSelf->tb__DOT__hs_s) 
                                                    >> 2U)));
    vlSelf->tb__DOT__dut__DOT__VSYNC_i_np = (1U ^ ((IData)(vlSelf->tb__DOT__dut__DOT__vsync_i_pol) 
                                                   ^ 
                                                   ((IData)(vlSelf->tb__DOT__vs_s) 
                                                    >> 2U)));
    vlSelf->tb__DOT__dut__DOT__meas_vblank_region = 
        ((vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr 
          < VL_SHIFTL_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 3U)) 
         | ((vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr 
             > ((vlSelf->tb__DOT__pcnt_field << (IData)(vlSelf->tb__DOT__interlace_flag)) 
                - VL_SHIFTL_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U))) 
            | ((IData)(vlSelf->tb__DOT__interlace_flag) 
               & ((vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr 
                   < (vlSelf->tb__DOT__pcnt_field + 
                      VL_SHIFTL_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 3U))) 
                  & (vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr 
                     > (vlSelf->tb__DOT__pcnt_field 
                        - VL_SHIFTL_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U)))))));
}

VL_ATTR_COLD void Vtb___024root___eval_stl(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vtb___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vtb___024root___eval_triggers__stl(Vtb___024root* vlSelf);

VL_ATTR_COLD bool Vtb___024root___eval_phase__stl(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vtb___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb___024root___dump_triggers__act(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge tb.clk)\n");
    }
    if ((2ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @([changed] (32'sh64 == tb.tc))\n");
    }
    if ((4ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb___024root___dump_triggers__nba(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge tb.clk)\n");
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @([changed] (32'sh64 == tb.tc))\n");
    }
    if ((4ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb___024root___ctor_var_reset(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->tb__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__PAL = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__MV = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__NF = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__VA = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__VP = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__VHS = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__VJ = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__HL = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__FHL = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__NEQ = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__sog = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__vs = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__k = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__f = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__off = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__w = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__ph = 0;
    vlSelf->tb__DOT__npos = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__tc = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__interlace_flag = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__sync_active = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__vtotal = VL_RAND_RESET_I(11);
    vlSelf->tb__DOT__pcnt_field = VL_RAND_RESET_I(20);
    vlSelf->tb__DOT__hsync_width = VL_RAND_RESET_I(8);
    vlSelf->tb__DOT__hs_s = VL_RAND_RESET_I(3);
    vlSelf->tb__DOT__vs_s = VL_RAND_RESET_I(3);
    vlSelf->tb__DOT__vsp = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__gen__DOT__start = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__gen__DOT__p = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__dut__DOT__HSYNC_i_np_prev = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__VSYNC_i_np_prev = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr = VL_RAND_RESET_I(21);
    vlSelf->tb__DOT__dut__DOT__syncpol_det_ctr = VL_RAND_RESET_I(18);
    vlSelf->tb__DOT__dut__DOT__hsync_hpol_ctr = VL_RAND_RESET_I(18);
    vlSelf->tb__DOT__dut__DOT__vsync_hpol_ctr = VL_RAND_RESET_I(18);
    vlSelf->tb__DOT__dut__DOT__sync_inactive_ctr = VL_RAND_RESET_I(3);
    vlSelf->tb__DOT__dut__DOT__pcnt_line = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__dut__DOT__pcnt_line_prev = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__dut__DOT__meas_h_cnt = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__dut__DOT__meas_h_cnt_sogref = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__dut__DOT__hs_ctr = VL_RAND_RESET_I(8);
    vlSelf->tb__DOT__dut__DOT__pcnt_line_stored = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__meas_v_cnt = VL_RAND_RESET_I(11);
    vlSelf->tb__DOT__dut__DOT__meas_fid = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__hsync_i_pol = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__vsync_i_pol = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__meas_h_cnt_ref = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__dut__DOT__meas_even_max_thold = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__dut__DOT__meas_vblank_region = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__VSYNC_i_np = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__dut__DOT__HSYNC_i_np = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__tb__DOT__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr_hec9a9fe2__0 = VL_RAND_RESET_I(1);
    vlSelf->__VactDidInit = 0;
}

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
    vlSelf->tb__DOT__TVP_PCLK_i = 0U;
    vlSelf->tb__DOT__tc = 0U;
    vlSelf->tb__DOT__nacc = 0U;
    vlSelf->tb__DOT__ngen = 0U;
    vlSelf->tb__DOT__maxerr = 0U;
    vlSelf->tb__DOT__hs_in = 1U;
    vlSelf->tb__DOT__TVP_HS_raw = 1U;
    vlSelf->tb__DOT__laststart = 0U;
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
            VL_FATAL_MT("tb_fw_old.v", 2, "", "Settle region did not converge.");
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
    // Init
    SData/*12:0*/ tb__DOT__fw_diff;
    tb__DOT__fw_diff = 0;
    // Body
    vlSelf->tb__DOT__fw_gen = ((IData)(vlSelf->tb__DOT__fw_locked) 
                               & ((IData)(vlSelf->tb__DOT__fw_cnt) 
                                  == (0xfffU & ((IData)(4U) 
                                                + (IData)(vlSelf->tb__DOT__fw_len)))));
    vlSelf->tb__DOT__fw_accept = ((~ (IData)(vlSelf->tb__DOT__TVP_HS_raw)) 
                                  & ((IData)(vlSelf->tb__DOT__fw_hs_prev) 
                                     & ((~ (IData)(vlSelf->tb__DOT__fw_locked)) 
                                        | ((IData)(vlSelf->tb__DOT__fw_cnt) 
                                           >= (0xfffU 
                                               & ((IData)(vlSelf->tb__DOT__fw_len) 
                                                  - 
                                                  VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__fw_len), 3U)))))));
    tb__DOT__fw_diff = (0x1fffU & ((IData)(vlSelf->tb__DOT__fw_cnt) 
                                   - (IData)(vlSelf->tb__DOT__fw_len)));
    vlSelf->tb__DOT__fw_near = ((0x1000U & (IData)(tb__DOT__fw_diff))
                                 ? (0xff7U < (0xfffU 
                                              & (IData)(tb__DOT__fw_diff)))
                                 : (9U > (0xfffU & (IData)(tb__DOT__fw_diff))));
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
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge tb.TVP_PCLK_i)\n");
    }
    if ((2ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
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
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge tb.TVP_PCLK_i)\n");
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb___024root___ctor_var_reset(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->tb__DOT__TVP_PCLK_i = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__VA = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__VP = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__VJ = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__MISS = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__MV = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__NL = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__tc = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__start = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__off = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__nacc = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__ngen = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__err = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__maxerr = VL_RAND_RESET_I(32);
    vlSelf->tb__DOT__hs_in = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__TVP_HS_raw = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__fw_hs_prev = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__fw_locked = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__fw_cnt = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__fw_len = VL_RAND_RESET_I(12);
    vlSelf->tb__DOT__fw_good = VL_RAND_RESET_I(4);
    vlSelf->tb__DOT__fw_near = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__fw_accept = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__fw_gen = VL_RAND_RESET_I(1);
    vlSelf->tb__DOT__laststart = VL_RAND_RESET_I(32);
    vlSelf->__Vtrigprevexpr___TOP__tb__DOT__TVP_PCLK_i__0 = VL_RAND_RESET_I(1);
}

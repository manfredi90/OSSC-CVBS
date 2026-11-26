// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb.h for the primary calling header

#include "Vtb__pch.h"
#include "Vtb___024root.h"

VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__0(Vtb___024root* vlSelf);
VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__1(Vtb___024root* vlSelf);

void Vtb___024root___eval_initial(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial\n"); );
    // Body
    Vtb___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb___024root___eval_initial__TOP__Vtiming__1(vlSelf);
    vlSelf->__Vtrigprevexpr___TOP__tb__DOT__TVP_PCLK_i__0 
        = vlSelf->tb__DOT__TVP_PCLK_i;
}

VL_INLINE_OPT VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__0(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial__TOP__Vtiming__0\n"); );
    // Init
    IData/*31:0*/ tb__DOT__ln;
    tb__DOT__ln = 0;
    IData/*31:0*/ tb__DOT____Vrepeat0;
    tb__DOT____Vrepeat0 = 0;
    // Body
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"VA=%d"}, 
                                vlSelf->tb__DOT__VA))) {
        vlSelf->tb__DOT__VA = 0U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"VP=%d"}, 
                                vlSelf->tb__DOT__VP))) {
        vlSelf->tb__DOT__VP = 0xc8U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"VJ=%d"}, 
                                vlSelf->tb__DOT__VJ))) {
        vlSelf->tb__DOT__VJ = 0U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"MISS=%d"}, 
                                vlSelf->tb__DOT__MISS))) {
        vlSelf->tb__DOT__MISS = 0U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"MV=%d"}, 
                                vlSelf->tb__DOT__MV))) {
        vlSelf->tb__DOT__MV = 0U;
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"NL=%d"}, 
                                vlSelf->tb__DOT__NL))) {
        vlSelf->tb__DOT__NL = 0xbb8U;
    }
    tb__DOT__ln = 0U;
    while (VL_LTS_III(32, tb__DOT__ln, vlSelf->tb__DOT__NL)) {
        vlSelf->tb__DOT__off = VL_RTOI_I_D((VL_ISTOR_D_I(32, vlSelf->tb__DOT__VA) 
                                            * sin((
                                                   (6.28318530700000011e+00 
                                                    * 
                                                    VL_ISTOR_D_I(32, tb__DOT__ln)) 
                                                   / 
                                                   VL_ISTOR_D_I(32, vlSelf->tb__DOT__VP)))));
        if (VL_LTS_III(32, 0U, vlSelf->tb__DOT__VJ)) {
            vlSelf->tb__DOT__off = ((vlSelf->tb__DOT__off 
                                     + VL_MODDIV_III(32, (IData)(
                                                                 VL_RANDOM_I()), 
                                                     ((IData)(1U) 
                                                      + 
                                                      VL_SHIFTL_III(32,32,32, vlSelf->tb__DOT__VJ, 1U)))) 
                                    - vlSelf->tb__DOT__VJ);
        }
        vlSelf->tb__DOT__start = ((IData)(0x3e8U) + 
                                  (VL_MULS_III(32, (IData)(0x6c0U), tb__DOT__ln) 
                                   + vlSelf->tb__DOT__off));
        while (VL_LTS_III(32, vlSelf->tb__DOT__tc, vlSelf->tb__DOT__start)) {
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               59);
        }
        vlSelf->tb__DOT__laststart = vlSelf->tb__DOT__start;
        if ((1U & (~ (VL_LTS_III(32, 0U, vlSelf->tb__DOT__MISS) 
                      & (0U == VL_MODDIVS_III(32, tb__DOT__ln, vlSelf->tb__DOT__MISS)))))) {
            vlSelf->tb__DOT__hs_in = 0U;
            tb__DOT____Vrepeat0 = 0x7fU;
            while (VL_LTS_III(32, 0U, tb__DOT____Vrepeat0)) {
                co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.TVP_PCLK_i)", 
                                                                   "tb_fw_old.v", 
                                                                   61);
                tb__DOT____Vrepeat0 = (tb__DOT____Vrepeat0 
                                       - (IData)(1U));
            }
            vlSelf->tb__DOT__hs_in = 1U;
        }
        if (((0U != vlSelf->tb__DOT__MV) & VL_GTS_III(32, 0xfU, 
                                                      VL_MODDIVS_III(32, tb__DOT__ln, (IData)(0x138U))))) {
            while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                              ((IData)(0x10eU) + vlSelf->tb__DOT__start))) {
                co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.TVP_PCLK_i)", 
                                                                   "tb_fw_old.v", 
                                                                   63);
            }
            vlSelf->tb__DOT__hs_in = 0U;
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            vlSelf->tb__DOT__hs_in = 1U;
            while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                              ((IData)(0x1e6U) + vlSelf->tb__DOT__start))) {
                co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.TVP_PCLK_i)", 
                                                                   "tb_fw_old.v", 
                                                                   63);
            }
            vlSelf->tb__DOT__hs_in = 0U;
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            vlSelf->tb__DOT__hs_in = 1U;
            while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                              ((IData)(0x2beU) + vlSelf->tb__DOT__start))) {
                co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.TVP_PCLK_i)", 
                                                                   "tb_fw_old.v", 
                                                                   63);
            }
            vlSelf->tb__DOT__hs_in = 0U;
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            vlSelf->tb__DOT__hs_in = 1U;
            while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                              ((IData)(0x396U) + vlSelf->tb__DOT__start))) {
                co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.TVP_PCLK_i)", 
                                                                   "tb_fw_old.v", 
                                                                   63);
            }
            vlSelf->tb__DOT__hs_in = 0U;
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            vlSelf->tb__DOT__hs_in = 1U;
            while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                              ((IData)(0x46eU) + vlSelf->tb__DOT__start))) {
                co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.TVP_PCLK_i)", 
                                                                   "tb_fw_old.v", 
                                                                   63);
            }
            vlSelf->tb__DOT__hs_in = 0U;
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            vlSelf->tb__DOT__hs_in = 1U;
            while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                              ((IData)(0x546U) + vlSelf->tb__DOT__start))) {
                co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.TVP_PCLK_i)", 
                                                                   "tb_fw_old.v", 
                                                                   63);
            }
            vlSelf->tb__DOT__hs_in = 0U;
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            co_await vlSelf->__VtrigSched_h698b33ed__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.TVP_PCLK_i)", 
                                                               "tb_fw_old.v", 
                                                               64);
            vlSelf->tb__DOT__hs_in = 1U;
        }
        tb__DOT__ln = ((IData)(1U) + tb__DOT__ln);
    }
    VL_WRITEF("accepted %0d generated %0d max offset of accepted edges vs real %0d\n",
              32,vlSelf->tb__DOT__nacc,32,vlSelf->tb__DOT__ngen,
              32,vlSelf->tb__DOT__maxerr);
    VL_FINISH_MT("tb_fw_old.v", 68, "");
}

VL_INLINE_OPT VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__1(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial__TOP__Vtiming__1\n"); );
    // Body
    while (1U) {
        co_await vlSelf->__VdlySched.delay(0x4844ULL, 
                                           nullptr, 
                                           "tb_fw_old.v", 
                                           3);
        vlSelf->tb__DOT__TVP_PCLK_i = (1U & (~ (IData)(vlSelf->tb__DOT__TVP_PCLK_i)));
    }
}

void Vtb___024root___eval_act(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vtb___024root___nba_sequent__TOP__0(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___nba_sequent__TOP__0\n"); );
    // Init
    SData/*12:0*/ tb__DOT__fw_diff;
    tb__DOT__fw_diff = 0;
    CData/*3:0*/ __Vdly__tb__DOT__fw_good;
    __Vdly__tb__DOT__fw_good = 0;
    SData/*11:0*/ __Vdly__tb__DOT__fw_cnt;
    __Vdly__tb__DOT__fw_cnt = 0;
    IData/*31:0*/ __Vdly__tb__DOT__tc;
    __Vdly__tb__DOT__tc = 0;
    // Body
    __Vdly__tb__DOT__tc = vlSelf->tb__DOT__tc;
    __Vdly__tb__DOT__fw_good = vlSelf->tb__DOT__fw_good;
    __Vdly__tb__DOT__fw_cnt = vlSelf->tb__DOT__fw_cnt;
    if (vlSelf->tb__DOT__fw_gen) {
        vlSelf->tb__DOT__ngen = ((IData)(1U) + vlSelf->tb__DOT__ngen);
    }
    if (((IData)(vlSelf->tb__DOT__fw_locked) & (IData)(vlSelf->tb__DOT__fw_accept))) {
        vlSelf->tb__DOT__nacc = ((IData)(1U) + vlSelf->tb__DOT__nacc);
        vlSelf->tb__DOT__err = (vlSelf->tb__DOT__tc 
                                - vlSelf->tb__DOT__laststart);
        if (VL_GTS_III(32, 0U, vlSelf->tb__DOT__err)) {
            vlSelf->tb__DOT__err = (- vlSelf->tb__DOT__err);
        }
        if ((VL_GTS_III(32, vlSelf->tb__DOT__err, vlSelf->tb__DOT__maxerr) 
             & VL_GTS_III(32, 0x64U, vlSelf->tb__DOT__err))) {
            vlSelf->tb__DOT__maxerr = vlSelf->tb__DOT__err;
        }
    }
    __Vdly__tb__DOT__tc = ((IData)(1U) + vlSelf->tb__DOT__tc);
    vlSelf->tb__DOT__fw_hs_prev = vlSelf->tb__DOT__TVP_HS_raw;
    if (((IData)(vlSelf->tb__DOT__fw_accept) | (IData)(vlSelf->tb__DOT__fw_gen))) {
        if (vlSelf->tb__DOT__fw_accept) {
            if ((1U & ((IData)(vlSelf->tb__DOT__fw_near) 
                       | (~ (IData)(vlSelf->tb__DOT__fw_locked))))) {
                vlSelf->tb__DOT__fw_len = vlSelf->tb__DOT__fw_cnt;
            }
            if ((1U & (~ (IData)(vlSelf->tb__DOT__fw_locked)))) {
                __Vdly__tb__DOT__fw_good = ((IData)(vlSelf->tb__DOT__fw_near)
                                             ? (0xfU 
                                                & ((IData)(1U) 
                                                   + (IData)(vlSelf->tb__DOT__fw_good)))
                                             : 0U);
                vlSelf->tb__DOT__fw_locked = ((IData)(vlSelf->tb__DOT__fw_near) 
                                              & (7U 
                                                 == (IData)(vlSelf->tb__DOT__fw_good)));
            }
        }
        __Vdly__tb__DOT__fw_cnt = 0U;
    } else if ((0xfffU != (IData)(vlSelf->tb__DOT__fw_cnt))) {
        __Vdly__tb__DOT__fw_cnt = (0xfffU & ((IData)(1U) 
                                             + (IData)(vlSelf->tb__DOT__fw_cnt)));
    }
    vlSelf->tb__DOT__tc = __Vdly__tb__DOT__tc;
    vlSelf->tb__DOT__fw_good = __Vdly__tb__DOT__fw_good;
    vlSelf->tb__DOT__fw_cnt = __Vdly__tb__DOT__fw_cnt;
    vlSelf->tb__DOT__TVP_HS_raw = vlSelf->tb__DOT__hs_in;
    vlSelf->tb__DOT__fw_gen = ((IData)(vlSelf->tb__DOT__fw_locked) 
                               & ((IData)(vlSelf->tb__DOT__fw_cnt) 
                                  == (0xfffU & ((IData)(4U) 
                                                + (IData)(vlSelf->tb__DOT__fw_len)))));
    tb__DOT__fw_diff = (0x1fffU & ((IData)(vlSelf->tb__DOT__fw_cnt) 
                                   - (IData)(vlSelf->tb__DOT__fw_len)));
    vlSelf->tb__DOT__fw_near = ((0x1000U & (IData)(tb__DOT__fw_diff))
                                 ? (0xff7U < (0xfffU 
                                              & (IData)(tb__DOT__fw_diff)))
                                 : (9U > (0xfffU & (IData)(tb__DOT__fw_diff))));
    vlSelf->tb__DOT__fw_accept = ((~ (IData)(vlSelf->tb__DOT__TVP_HS_raw)) 
                                  & ((IData)(vlSelf->tb__DOT__fw_hs_prev) 
                                     & ((~ (IData)(vlSelf->tb__DOT__fw_locked)) 
                                        | ((IData)(vlSelf->tb__DOT__fw_cnt) 
                                           >= (0xfffU 
                                               & ((IData)(vlSelf->tb__DOT__fw_len) 
                                                  - 
                                                  VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__fw_len), 3U)))))));
}

void Vtb___024root___eval_nba(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vtb___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vtb___024root___timing_resume(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___timing_resume\n"); );
    // Body
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VtrigSched_h698b33ed__0.resume("@(posedge tb.TVP_PCLK_i)");
    }
    if ((2ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VdlySched.resume();
    }
}

void Vtb___024root___timing_commit(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___timing_commit\n"); );
    // Body
    if ((! (1ULL & vlSelf->__VactTriggered.word(0U)))) {
        vlSelf->__VtrigSched_h698b33ed__0.commit("@(posedge tb.TVP_PCLK_i)");
    }
}

void Vtb___024root___eval_triggers__act(Vtb___024root* vlSelf);

bool Vtb___024root___eval_phase__act(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<2> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb___024root___eval_triggers__act(vlSelf);
    Vtb___024root___timing_commit(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vtb___024root___timing_resume(vlSelf);
        Vtb___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtb___024root___eval_phase__nba(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vtb___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb___024root___dump_triggers__nba(Vtb___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb___024root___dump_triggers__act(Vtb___024root* vlSelf);
#endif  // VL_DEBUG

void Vtb___024root___eval(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vtb___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("tb_fw_old.v", 2, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vtb___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("tb_fw_old.v", 2, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vtb___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vtb___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vtb___024root___eval_debug_assertions(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_debug_assertions\n"); );
}
#endif  // VL_DEBUG

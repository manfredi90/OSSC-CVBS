// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb.h for the primary calling header

#include "Vtb__pch.h"
#include "Vtb___024root.h"

VL_ATTR_COLD void Vtb___024root___eval_initial__TOP(Vtb___024root* vlSelf);
VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__0(Vtb___024root* vlSelf);
VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__1(Vtb___024root* vlSelf);

void Vtb___024root___eval_initial(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial\n"); );
    // Body
    Vtb___024root___eval_initial__TOP(vlSelf);
    Vtb___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb___024root___eval_initial__TOP__Vtiming__1(vlSelf);
    vlSelf->__Vtrigprevexpr___TOP__tb__DOT__clk__0 
        = vlSelf->tb__DOT__clk;
    vlSelf->__Vtrigprevexpr_hec9a9fe2__0 = (0x64U == vlSelf->tb__DOT__tc);
}

VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__0____Vfork_1__0(Vtb___024root* vlSelf);

VL_INLINE_OPT VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__0(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial__TOP__Vtiming__0\n"); );
    // Init
    VlUnpacked<IData/*31:0*/, 16> tb__DOT__pos;
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        tb__DOT__pos[__Vi0] = 0;
    }
    IData/*31:0*/ tb__DOT____Vrepeat0;
    tb__DOT____Vrepeat0 = 0;
    IData/*31:0*/ tb__DOT__gen__DOT__base;
    tb__DOT__gen__DOT__base = 0;
    IData/*31:0*/ tb__DOT__gen__DOT__hs_w;
    tb__DOT__gen__DOT__hs_w = 0;
    IData/*31:0*/ tb__DOT__gen__DOT__eq_w;
    tb__DOT__gen__DOT__eq_w = 0;
    IData/*31:0*/ tb__DOT__gen__DOT__br_w;
    tb__DOT__gen__DOT__br_w = 0;
    // Body
    tb__DOT__gen__DOT__hs_w = 0x7fU;
    tb__DOT__gen__DOT__eq_w = 0x3fU;
    tb__DOT__gen__DOT__br_w = (vlSelf->tb__DOT__HL 
                               - (IData)(0x7fU));
    while ((0x64U != vlSelf->tb__DOT__tc)) {
        co_await vlSelf->__VtrigSched_h78add7c0__0.trigger(1U, 
                                                           nullptr, 
                                                           "@([changed] (32'sh64 == tb.tc))", 
                                                           "tb_fe.v", 
                                                           49);
    }
    tb__DOT__gen__DOT__base = 0x64U;
    vlSelf->tb__DOT__k = 0U;
    while (VL_LTS_III(32, vlSelf->tb__DOT__k, VL_MULS_III(32, vlSelf->tb__DOT__NF, vlSelf->tb__DOT__FHL))) {
        vlSelf->tb__DOT__f = VL_MODDIVS_III(32, vlSelf->tb__DOT__k, vlSelf->tb__DOT__FHL);
        vlSelf->tb__DOT__ph = (VL_ISTOR_D_I(32, vlSelf->tb__DOT__VA) 
                               * sin(((6.28318530700000011e+00 
                                       * VL_ISTOR_D_I(32, 
                                                      VL_SHIFTR_III(32,32,32, vlSelf->tb__DOT__k, 1U))) 
                                      / VL_ISTOR_D_I(32, vlSelf->tb__DOT__VP))));
        vlSelf->tb__DOT__off = VL_RTOI_I_D(vlSelf->tb__DOT__ph);
        if (VL_LTS_III(32, 0U, vlSelf->tb__DOT__VJ)) {
            vlSelf->tb__DOT__off = ((vlSelf->tb__DOT__off 
                                     + VL_MODDIV_III(32, (IData)(
                                                                 VL_RANDOM_I()), 
                                                     ((IData)(1U) 
                                                      + 
                                                      VL_SHIFTL_III(32,32,32, vlSelf->tb__DOT__VJ, 1U)))) 
                                    - vlSelf->tb__DOT__VJ);
        }
        if ((((0U != vlSelf->tb__DOT__VHS) & VL_GTES_III(32, vlSelf->tb__DOT__f, 
                                                         ((vlSelf->tb__DOT__FHL 
                                                           - vlSelf->tb__DOT__NEQ) 
                                                          - (IData)(0xcU)))) 
             & VL_LTS_III(32, vlSelf->tb__DOT__f, (vlSelf->tb__DOT__FHL 
                                                   - vlSelf->tb__DOT__NEQ)))) {
            vlSelf->tb__DOT__off = (vlSelf->tb__DOT__off 
                                    + vlSelf->tb__DOT__VHS);
        }
        vlSelf->tb__DOT__gen__DOT__start = ((tb__DOT__gen__DOT__base 
                                             + VL_MULS_III(32, vlSelf->tb__DOT__k, vlSelf->tb__DOT__HL)) 
                                            + vlSelf->tb__DOT__off);
        vlSelf->tb__DOT__npos = 0U;
        if (VL_LTS_III(32, vlSelf->tb__DOT__f, vlSelf->tb__DOT__NEQ)) {
            tb__DOT__pos[0U] = 0U;
            vlSelf->tb__DOT__w = tb__DOT__gen__DOT__br_w;
            vlSelf->tb__DOT__npos = 1U;
        } else if ((VL_LTS_III(32, vlSelf->tb__DOT__f, 
                               VL_MULS_III(32, (IData)(2U), vlSelf->tb__DOT__NEQ)) 
                    | VL_GTES_III(32, vlSelf->tb__DOT__f, 
                                  (vlSelf->tb__DOT__FHL 
                                   - vlSelf->tb__DOT__NEQ)))) {
            tb__DOT__pos[0U] = 0U;
            vlSelf->tb__DOT__w = tb__DOT__gen__DOT__eq_w;
            vlSelf->tb__DOT__npos = 1U;
        } else if ((0U == VL_MODDIVS_III(32, vlSelf->tb__DOT__k, (IData)(2U)))) {
            tb__DOT__pos[0U] = 0U;
            vlSelf->tb__DOT__w = tb__DOT__gen__DOT__hs_w;
            vlSelf->tb__DOT__npos = 1U;
        }
        if ((0U == vlSelf->tb__DOT__f)) {
            Vtb___024root___eval_initial__TOP__Vtiming__0____Vfork_1__0(vlSelf);
        }
        while (VL_LTS_III(32, vlSelf->tb__DOT__tc, vlSelf->tb__DOT__gen__DOT__start)) {
            co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                               nullptr, 
                                                               "@(posedge tb.clk)", 
                                                               "tb_fe.v", 
                                                               72);
        }
        if ((0U != vlSelf->tb__DOT__npos)) {
            vlSelf->tb__DOT__sog = 0U;
            tb__DOT____Vrepeat0 = vlSelf->tb__DOT__w;
            while (VL_LTS_III(32, 0U, tb__DOT____Vrepeat0)) {
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   73);
                tb__DOT____Vrepeat0 = (tb__DOT____Vrepeat0 
                                       - (IData)(1U));
            }
            vlSelf->tb__DOT__sog = 1U;
        }
        if (((((0U != vlSelf->tb__DOT__MV) & (0U == 
                                              VL_MODDIVS_III(32, vlSelf->tb__DOT__k, (IData)(2U)))) 
              & VL_GTES_III(32, vlSelf->tb__DOT__f, 
                            ((IData)(2U) + VL_MULS_III(32, (IData)(2U), vlSelf->tb__DOT__NEQ)))) 
             & VL_LTS_III(32, vlSelf->tb__DOT__f, ((IData)(0x20U) 
                                                   + 
                                                   VL_MULS_III(32, (IData)(2U), vlSelf->tb__DOT__NEQ))))) {
            vlSelf->tb__DOT__gen__DOT__p = 0x10eU;
            while (VL_LTS_III(32, vlSelf->tb__DOT__gen__DOT__p, 
                              (vlSelf->tb__DOT__HL 
                               - (IData)(0x64U)))) {
                while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                                  (vlSelf->tb__DOT__gen__DOT__start 
                                   + vlSelf->tb__DOT__gen__DOT__p))) {
                    co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                       nullptr, 
                                                                       "@(posedge tb.clk)", 
                                                                       "tb_fe.v", 
                                                                       77);
                }
                vlSelf->tb__DOT__sog = 0U;
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   78);
                vlSelf->tb__DOT__sog = 1U;
                vlSelf->tb__DOT__gen__DOT__p = ((IData)(0xd8U) 
                                                + vlSelf->tb__DOT__gen__DOT__p);
            }
        } else if (((((0U != vlSelf->tb__DOT__MV) & 
                      (1U == VL_MODDIVS_III(32, vlSelf->tb__DOT__k, (IData)(2U)))) 
                     & VL_GTES_III(32, vlSelf->tb__DOT__f, 
                                   ((IData)(2U) + VL_MULS_III(32, (IData)(2U), vlSelf->tb__DOT__NEQ)))) 
                    & VL_LTS_III(32, vlSelf->tb__DOT__f, 
                                 ((IData)(0x20U) + 
                                  VL_MULS_III(32, (IData)(2U), vlSelf->tb__DOT__NEQ))))) {
            vlSelf->tb__DOT__gen__DOT__p = 0x14U;
            while (VL_LTS_III(32, vlSelf->tb__DOT__gen__DOT__p, 
                              (vlSelf->tb__DOT__HL 
                               - (IData)(0x64U)))) {
                while (VL_LTS_III(32, vlSelf->tb__DOT__tc, 
                                  (vlSelf->tb__DOT__gen__DOT__start 
                                   + vlSelf->tb__DOT__gen__DOT__p))) {
                    co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                       nullptr, 
                                                                       "@(posedge tb.clk)", 
                                                                       "tb_fe.v", 
                                                                       82);
                }
                vlSelf->tb__DOT__sog = 0U;
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                co_await vlSelf->__VtrigSched_h8aadc0b9__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge tb.clk)", 
                                                                   "tb_fe.v", 
                                                                   83);
                vlSelf->tb__DOT__sog = 1U;
                vlSelf->tb__DOT__gen__DOT__p = ((IData)(0xd8U) 
                                                + vlSelf->tb__DOT__gen__DOT__p);
            }
        }
        vlSelf->tb__DOT__k = ((IData)(1U) + vlSelf->tb__DOT__k);
    }
    VL_FINISH_MT("tb_fe.v", 87, "");
}

VL_INLINE_OPT VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__0____Vfork_1__0(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial__TOP__Vtiming__0____Vfork_1__0\n"); );
    // Body
    co_await vlSelf->__VdlySched.delay((0x3e8ULL * 
                                        VL_MULS_QQQ(32, (QData)((IData)(0x25U)), (QData)((IData)(
                                                                                ((IData)(0x14U) 
                                                                                + 
                                                                                (vlSelf->tb__DOT__gen__DOT__start 
                                                                                - vlSelf->tb__DOT__tc)))))), 
                                       nullptr, "tb_fe.v", 
                                       70);
    vlSelf->tb__DOT__vs = 0U;
    co_await vlSelf->__VdlySched.delay((0x3e8ULL * 
                                        VL_MULS_QQQ(32, (QData)((IData)(0xdeU)), (QData)((IData)(vlSelf->tb__DOT__HL)))), 
                                       nullptr, "tb_fe.v", 
                                       70);
    vlSelf->tb__DOT__vs = 1U;
}

VL_INLINE_OPT VlCoroutine Vtb___024root___eval_initial__TOP__Vtiming__1(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_initial__TOP__Vtiming__1\n"); );
    // Body
    while (1U) {
        co_await vlSelf->__VdlySched.delay(0x4844ULL, 
                                           nullptr, 
                                           "tb_fe.v", 
                                           11);
        vlSelf->tb__DOT__clk = (1U & (~ (IData)(vlSelf->tb__DOT__clk)));
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
    CData/*2:0*/ __Vdly__tb__DOT__hs_s;
    __Vdly__tb__DOT__hs_s = 0;
    CData/*2:0*/ __Vdly__tb__DOT__vs_s;
    __Vdly__tb__DOT__vs_s = 0;
    CData/*0:0*/ __Vdly__tb__DOT__dut__DOT__pcnt_line_stored;
    __Vdly__tb__DOT__dut__DOT__pcnt_line_stored = 0;
    IData/*20:0*/ __Vdly__tb__DOT__dut__DOT__pcnt_frame_ctr;
    __Vdly__tb__DOT__dut__DOT__pcnt_frame_ctr = 0;
    SData/*11:0*/ __Vdly__tb__DOT__dut__DOT__pcnt_line_prev;
    __Vdly__tb__DOT__dut__DOT__pcnt_line_prev = 0;
    SData/*11:0*/ __Vdly__tb__DOT__dut__DOT__pcnt_line_ctr;
    __Vdly__tb__DOT__dut__DOT__pcnt_line_ctr = 0;
    CData/*7:0*/ __Vdly__tb__DOT__dut__DOT__hs_ctr;
    __Vdly__tb__DOT__dut__DOT__hs_ctr = 0;
    IData/*17:0*/ __Vdly__tb__DOT__dut__DOT__hsync_hpol_ctr;
    __Vdly__tb__DOT__dut__DOT__hsync_hpol_ctr = 0;
    IData/*17:0*/ __Vdly__tb__DOT__dut__DOT__vsync_hpol_ctr;
    __Vdly__tb__DOT__dut__DOT__vsync_hpol_ctr = 0;
    SData/*11:0*/ __Vdly__tb__DOT__dut__DOT__meas_h_cnt;
    __Vdly__tb__DOT__dut__DOT__meas_h_cnt = 0;
    SData/*10:0*/ __Vdly__tb__DOT__dut__DOT__meas_v_cnt;
    __Vdly__tb__DOT__dut__DOT__meas_v_cnt = 0;
    CData/*0:0*/ __Vdly__tb__DOT__interlace_flag;
    __Vdly__tb__DOT__interlace_flag = 0;
    CData/*0:0*/ __Vdly__tb__DOT__dut__DOT__meas_fid;
    __Vdly__tb__DOT__dut__DOT__meas_fid = 0;
    // Body
    __Vdly__tb__DOT__hs_s = vlSelf->tb__DOT__hs_s;
    __Vdly__tb__DOT__vs_s = vlSelf->tb__DOT__vs_s;
    __Vdly__tb__DOT__dut__DOT__vsync_hpol_ctr = vlSelf->tb__DOT__dut__DOT__vsync_hpol_ctr;
    __Vdly__tb__DOT__dut__DOT__hsync_hpol_ctr = vlSelf->tb__DOT__dut__DOT__hsync_hpol_ctr;
    __Vdly__tb__DOT__dut__DOT__hs_ctr = vlSelf->tb__DOT__dut__DOT__hs_ctr;
    __Vdly__tb__DOT__dut__DOT__pcnt_line_ctr = vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr;
    __Vdly__tb__DOT__dut__DOT__pcnt_line_prev = vlSelf->tb__DOT__dut__DOT__pcnt_line_prev;
    __Vdly__tb__DOT__dut__DOT__pcnt_line_stored = vlSelf->tb__DOT__dut__DOT__pcnt_line_stored;
    __Vdly__tb__DOT__dut__DOT__pcnt_frame_ctr = vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr;
    __Vdly__tb__DOT__dut__DOT__meas_v_cnt = vlSelf->tb__DOT__dut__DOT__meas_v_cnt;
    __Vdly__tb__DOT__dut__DOT__meas_h_cnt = vlSelf->tb__DOT__dut__DOT__meas_h_cnt;
    __Vdly__tb__DOT__interlace_flag = vlSelf->tb__DOT__interlace_flag;
    __Vdly__tb__DOT__dut__DOT__meas_fid = vlSelf->tb__DOT__dut__DOT__meas_fid;
    if (VL_UNLIKELY(((IData)(vlSelf->tb__DOT__vsp) 
                     & (~ ((IData)(vlSelf->tb__DOT__vs_s) 
                           >> 2U))))) {
        VL_WRITEF("F vtotal %0# ilace %0# pcnt_field %0# pcnt_line %0# hsw %0#\n",
                  11,vlSelf->tb__DOT__vtotal,1,(IData)(vlSelf->tb__DOT__interlace_flag),
                  20,vlSelf->tb__DOT__pcnt_field,12,
                  (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line),
                  8,vlSelf->tb__DOT__hsync_width);
    }
    vlSelf->tb__DOT__tc = ((IData)(1U) + vlSelf->tb__DOT__tc);
    __Vdly__tb__DOT__hs_s = ((6U & ((IData)(vlSelf->tb__DOT__hs_s) 
                                    << 1U)) | (IData)(vlSelf->tb__DOT__sog));
    __Vdly__tb__DOT__vs_s = ((6U & ((IData)(vlSelf->tb__DOT__vs_s) 
                                    << 1U)) | (IData)(vlSelf->tb__DOT__vs));
    vlSelf->tb__DOT__vsp = (1U & ((IData)(vlSelf->tb__DOT__vs_s) 
                                  >> 2U));
    if (((IData)(vlSelf->tb__DOT__dut__DOT__VSYNC_i_np_prev) 
         & (~ (IData)(vlSelf->tb__DOT__dut__DOT__VSYNC_i_np)))) {
        __Vdly__tb__DOT__dut__DOT__meas_fid = (((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt_sogref) 
                                                < (0xfffU 
                                                   & VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U))) 
                                               | ((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt_sogref) 
                                                  > (IData)(vlSelf->tb__DOT__dut__DOT__meas_even_max_thold)));
    }
    if ((((IData)(vlSelf->tb__DOT__dut__DOT__HSYNC_i_np_prev) 
          & (~ (IData)(vlSelf->tb__DOT__dut__DOT__HSYNC_i_np))) 
         & ((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
            > (0xfffU & ((IData)(vlSelf->tb__DOT__dut__DOT__meas_vblank_region)
                          ? VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U)
                          : VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 3U)))))) {
        if ((((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
              > (VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 1U) 
                 - VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U))) 
             & ((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
                < (VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 1U) 
                   + VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U))))) {
            __Vdly__tb__DOT__dut__DOT__meas_h_cnt = 
                (0xfffU & ((IData)(1U) + (IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt)));
        } else if (((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
                    >= ((IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line) 
                        - VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 3U)))) {
            __Vdly__tb__DOT__dut__DOT__meas_v_cnt = 
                (0x7ffU & ((IData)(1U) + (IData)(vlSelf->tb__DOT__dut__DOT__meas_v_cnt)));
            __Vdly__tb__DOT__dut__DOT__meas_h_cnt = 0U;
        } else {
            __Vdly__tb__DOT__dut__DOT__meas_h_cnt = 
                (0xfffU & ((IData)(1U) + (IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt)));
        }
        if (((((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
               > (VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 1U) 
                  - VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U))) 
              & ((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
                 < (VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 1U) 
                    + VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U)))) 
             | ((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
                >= ((IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line) 
                    - VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 3U))))) {
            vlSelf->tb__DOT__dut__DOT__meas_h_cnt_sogref 
                = vlSelf->tb__DOT__dut__DOT__meas_h_cnt;
        }
    } else if (((IData)(vlSelf->tb__DOT__dut__DOT__meas_vblank_region) 
                & ((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt) 
                   >= ((IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line) 
                       + VL_SHIFTR_III(32,32,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 3U))))) {
        __Vdly__tb__DOT__dut__DOT__meas_v_cnt = (0x7ffU 
                                                 & ((IData)(1U) 
                                                    + (IData)(vlSelf->tb__DOT__dut__DOT__meas_v_cnt)));
        vlSelf->tb__DOT__dut__DOT__meas_h_cnt_sogref 
            = vlSelf->tb__DOT__dut__DOT__meas_h_cnt;
        __Vdly__tb__DOT__dut__DOT__meas_h_cnt = 0U;
    } else {
        __Vdly__tb__DOT__dut__DOT__meas_h_cnt = (0xfffU 
                                                 & ((IData)(1U) 
                                                    + (IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt)));
    }
    if (((IData)(vlSelf->tb__DOT__dut__DOT__VSYNC_i_np_prev) 
         & (~ (IData)(vlSelf->tb__DOT__dut__DOT__VSYNC_i_np)))) {
        if ((((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt_ref) 
              < (0xfffU & VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U))) 
             | ((IData)(vlSelf->tb__DOT__dut__DOT__meas_h_cnt_ref) 
                > (IData)(vlSelf->tb__DOT__dut__DOT__meas_even_max_thold)))) {
            __Vdly__tb__DOT__dut__DOT__meas_v_cnt = 0U;
            __Vdly__tb__DOT__interlace_flag = (1U & 
                                               (~ (IData)(vlSelf->tb__DOT__dut__DOT__meas_fid)));
            vlSelf->tb__DOT__vtotal = vlSelf->tb__DOT__dut__DOT__meas_v_cnt;
        } else {
            __Vdly__tb__DOT__interlace_flag = vlSelf->tb__DOT__dut__DOT__meas_fid;
            if ((1U & (~ (IData)(vlSelf->tb__DOT__dut__DOT__meas_fid)))) {
                __Vdly__tb__DOT__dut__DOT__meas_v_cnt = 0U;
                vlSelf->tb__DOT__vtotal = vlSelf->tb__DOT__dut__DOT__meas_v_cnt;
            }
        }
    }
    vlSelf->tb__DOT__dut__DOT__meas_h_cnt = __Vdly__tb__DOT__dut__DOT__meas_h_cnt;
    vlSelf->tb__DOT__dut__DOT__meas_v_cnt = __Vdly__tb__DOT__dut__DOT__meas_v_cnt;
    vlSelf->tb__DOT__dut__DOT__meas_h_cnt_ref = vlSelf->tb__DOT__dut__DOT__meas_h_cnt_sogref;
    if ((((IData)(vlSelf->tb__DOT__dut__DOT__VSYNC_i_np_prev) 
          & (~ (IData)(vlSelf->tb__DOT__dut__DOT__VSYNC_i_np))) 
         & ((~ (IData)(vlSelf->tb__DOT__interlace_flag)) 
            | (~ (IData)(vlSelf->tb__DOT__dut__DOT__meas_fid))))) {
        __Vdly__tb__DOT__dut__DOT__pcnt_line_stored = 0U;
        if (((IData)(vlSelf->tb__DOT__sync_active) 
             & (0x1fffffU != vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr))) {
            vlSelf->tb__DOT__pcnt_field = (0xfffffU 
                                           & ((IData)(vlSelf->tb__DOT__interlace_flag)
                                               ? VL_SHIFTR_III(21,21,32, vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr, 1U)
                                               : (0xfffffU 
                                                  & vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr)));
        }
        __Vdly__tb__DOT__dut__DOT__pcnt_frame_ctr = 1U;
    } else if ((0x1fffffU > vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr)) {
        __Vdly__tb__DOT__dut__DOT__pcnt_frame_ctr = 
            (0x1fffffU & ((IData)(1U) + vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr));
    } else {
        vlSelf->tb__DOT__pcnt_field = 0U;
    }
    if (((IData)(vlSelf->tb__DOT__dut__DOT__HSYNC_i_np_prev) 
         & (~ (IData)(vlSelf->tb__DOT__dut__DOT__HSYNC_i_np)))) {
        __Vdly__tb__DOT__dut__DOT__pcnt_line_prev = vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr;
        if (((((~ (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line_stored)) 
               & (0x34bc0U < vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr)) 
              & ((IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr) 
                 <= (0xfffU & ((IData)(8U) + (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line_prev))))) 
             & ((IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line_prev) 
                <= (0xfffU & ((IData)(8U) + (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr)))))) {
            vlSelf->tb__DOT__dut__DOT__pcnt_line = vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr;
            vlSelf->tb__DOT__hsync_width = vlSelf->tb__DOT__dut__DOT__hs_ctr;
            __Vdly__tb__DOT__dut__DOT__pcnt_line_stored = 1U;
        }
        __Vdly__tb__DOT__dut__DOT__pcnt_line_ctr = 1U;
        __Vdly__tb__DOT__dut__DOT__hs_ctr = 1U;
    } else {
        __Vdly__tb__DOT__dut__DOT__pcnt_line_ctr = 
            (0xfffU & ((IData)(1U) + (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr)));
        if ((1U & (~ (IData)(vlSelf->tb__DOT__dut__DOT__HSYNC_i_np)))) {
            __Vdly__tb__DOT__dut__DOT__hs_ctr = (0xffU 
                                                 & ((IData)(1U) 
                                                    + (IData)(vlSelf->tb__DOT__dut__DOT__hs_ctr)));
        }
    }
    vlSelf->tb__DOT__dut__DOT__meas_fid = __Vdly__tb__DOT__dut__DOT__meas_fid;
    vlSelf->tb__DOT__dut__DOT__pcnt_line_stored = __Vdly__tb__DOT__dut__DOT__pcnt_line_stored;
    vlSelf->tb__DOT__dut__DOT__pcnt_line_prev = __Vdly__tb__DOT__dut__DOT__pcnt_line_prev;
    vlSelf->tb__DOT__dut__DOT__pcnt_line_ctr = __Vdly__tb__DOT__dut__DOT__pcnt_line_ctr;
    vlSelf->tb__DOT__dut__DOT__hs_ctr = __Vdly__tb__DOT__dut__DOT__hs_ctr;
    vlSelf->tb__DOT__interlace_flag = __Vdly__tb__DOT__interlace_flag;
    vlSelf->tb__DOT__dut__DOT__pcnt_frame_ctr = __Vdly__tb__DOT__dut__DOT__pcnt_frame_ctr;
    vlSelf->tb__DOT__dut__DOT__meas_even_max_thold 
        = (0xfffU & (VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 1U) 
                     + VL_SHIFTR_III(12,12,32, (IData)(vlSelf->tb__DOT__dut__DOT__pcnt_line), 2U)));
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
    vlSelf->tb__DOT__dut__DOT__HSYNC_i_np_prev = vlSelf->tb__DOT__dut__DOT__HSYNC_i_np;
    vlSelf->tb__DOT__dut__DOT__VSYNC_i_np_prev = vlSelf->tb__DOT__dut__DOT__VSYNC_i_np;
    if ((0U == vlSelf->tb__DOT__dut__DOT__syncpol_det_ctr)) {
        vlSelf->tb__DOT__dut__DOT__hsync_i_pol = (0x1ffffU 
                                                  < vlSelf->tb__DOT__dut__DOT__hsync_hpol_ctr);
        vlSelf->tb__DOT__dut__DOT__vsync_i_pol = (0x1ffffU 
                                                  < vlSelf->tb__DOT__dut__DOT__vsync_hpol_ctr);
        if (((0U == vlSelf->tb__DOT__dut__DOT__vsync_hpol_ctr) 
             | (0x3ffffU == vlSelf->tb__DOT__dut__DOT__vsync_hpol_ctr))) {
            if ((7U == (IData)(vlSelf->tb__DOT__dut__DOT__sync_inactive_ctr))) {
                vlSelf->tb__DOT__sync_active = 0U;
            } else {
                vlSelf->tb__DOT__dut__DOT__sync_inactive_ctr 
                    = (7U & ((IData)(1U) + (IData)(vlSelf->tb__DOT__dut__DOT__sync_inactive_ctr)));
            }
        } else {
            vlSelf->tb__DOT__dut__DOT__sync_inactive_ctr = 0U;
            vlSelf->tb__DOT__sync_active = 1U;
        }
        __Vdly__tb__DOT__dut__DOT__hsync_hpol_ctr = 0U;
        __Vdly__tb__DOT__dut__DOT__vsync_hpol_ctr = 0U;
    } else {
        if ((4U & (IData)(vlSelf->tb__DOT__hs_s))) {
            __Vdly__tb__DOT__dut__DOT__hsync_hpol_ctr 
                = (0x3ffffU & ((IData)(1U) + vlSelf->tb__DOT__dut__DOT__hsync_hpol_ctr));
        }
        if ((4U & (IData)(vlSelf->tb__DOT__vs_s))) {
            __Vdly__tb__DOT__dut__DOT__vsync_hpol_ctr 
                = (0x3ffffU & ((IData)(1U) + vlSelf->tb__DOT__dut__DOT__vsync_hpol_ctr));
        }
    }
    vlSelf->tb__DOT__dut__DOT__syncpol_det_ctr = (0x3ffffU 
                                                  & ((IData)(1U) 
                                                     + vlSelf->tb__DOT__dut__DOT__syncpol_det_ctr));
    vlSelf->tb__DOT__dut__DOT__hsync_hpol_ctr = __Vdly__tb__DOT__dut__DOT__hsync_hpol_ctr;
    vlSelf->tb__DOT__dut__DOT__vsync_hpol_ctr = __Vdly__tb__DOT__dut__DOT__vsync_hpol_ctr;
    vlSelf->tb__DOT__hs_s = __Vdly__tb__DOT__hs_s;
    vlSelf->tb__DOT__vs_s = __Vdly__tb__DOT__vs_s;
    vlSelf->tb__DOT__dut__DOT__HSYNC_i_np = (1U ^ ((IData)(vlSelf->tb__DOT__dut__DOT__hsync_i_pol) 
                                                   ^ 
                                                   ((IData)(vlSelf->tb__DOT__hs_s) 
                                                    >> 2U)));
    vlSelf->tb__DOT__dut__DOT__VSYNC_i_np = (1U ^ ((IData)(vlSelf->tb__DOT__dut__DOT__vsync_i_pol) 
                                                   ^ 
                                                   ((IData)(vlSelf->tb__DOT__vs_s) 
                                                    >> 2U)));
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
    if ((2ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VtrigSched_h78add7c0__0.resume("@([changed] (32'sh64 == tb.tc))");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VtrigSched_h8aadc0b9__0.resume("@(posedge tb.clk)");
    }
    if ((4ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VdlySched.resume();
    }
}

void Vtb___024root___timing_commit(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___timing_commit\n"); );
    // Body
    if ((! (2ULL & vlSelf->__VactTriggered.word(0U)))) {
        vlSelf->__VtrigSched_h78add7c0__0.commit("@([changed] (32'sh64 == tb.tc))");
    }
    if ((! (1ULL & vlSelf->__VactTriggered.word(0U)))) {
        vlSelf->__VtrigSched_h8aadc0b9__0.commit("@(posedge tb.clk)");
    }
}

void Vtb___024root___eval_triggers__act(Vtb___024root* vlSelf);

bool Vtb___024root___eval_phase__act(Vtb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<3> __VpreTriggered;
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
            VL_FATAL_MT("tb_fe.v", 9, "", "NBA region did not converge.");
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
                VL_FATAL_MT("tb_fe.v", 9, "", "Active region did not converge.");
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

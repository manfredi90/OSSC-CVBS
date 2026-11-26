// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb.h for the primary calling header

#ifndef VERILATED_VTB___024ROOT_H_
#define VERILATED_VTB___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ tb__DOT__TVP_PCLK_i;
    CData/*0:0*/ tb__DOT__hs_in;
    CData/*0:0*/ tb__DOT__TVP_HS_raw;
    CData/*0:0*/ tb__DOT__fw_hs_prev;
    CData/*0:0*/ tb__DOT__fw_locked;
    CData/*3:0*/ tb__DOT__fw_good;
    CData/*0:0*/ tb__DOT__fw_near;
    CData/*0:0*/ tb__DOT__fw_accept;
    CData/*0:0*/ tb__DOT__fw_gen;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb__DOT__TVP_PCLK_i__0;
    CData/*0:0*/ __VactContinue;
    SData/*11:0*/ tb__DOT__fw_cnt;
    SData/*11:0*/ tb__DOT__fw_len;
    IData/*31:0*/ tb__DOT__VA;
    IData/*31:0*/ tb__DOT__VP;
    IData/*31:0*/ tb__DOT__VJ;
    IData/*31:0*/ tb__DOT__MISS;
    IData/*31:0*/ tb__DOT__MV;
    IData/*31:0*/ tb__DOT__NL;
    IData/*31:0*/ tb__DOT__tc;
    IData/*31:0*/ tb__DOT__start;
    IData/*31:0*/ tb__DOT__off;
    IData/*31:0*/ tb__DOT__nacc;
    IData/*31:0*/ tb__DOT__ngen;
    IData/*31:0*/ tb__DOT__err;
    IData/*31:0*/ tb__DOT__maxerr;
    IData/*31:0*/ tb__DOT__laststart;
    IData/*31:0*/ __VactIterCount;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h698b33ed__0;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtb__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtb___024root(Vtb__Syms* symsp, const char* v__name);
    ~Vtb___024root();
    VL_UNCOPYABLE(Vtb___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard

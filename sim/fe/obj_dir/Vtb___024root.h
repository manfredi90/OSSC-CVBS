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
    CData/*0:0*/ tb__DOT__clk;
    CData/*0:0*/ tb__DOT__sog;
    CData/*0:0*/ tb__DOT__vs;
    CData/*0:0*/ tb__DOT__interlace_flag;
    CData/*0:0*/ tb__DOT__sync_active;
    CData/*7:0*/ tb__DOT__hsync_width;
    CData/*2:0*/ tb__DOT__hs_s;
    CData/*2:0*/ tb__DOT__vs_s;
    CData/*0:0*/ tb__DOT__vsp;
    CData/*0:0*/ tb__DOT__dut__DOT__HSYNC_i_np_prev;
    CData/*0:0*/ tb__DOT__dut__DOT__VSYNC_i_np_prev;
    CData/*2:0*/ tb__DOT__dut__DOT__sync_inactive_ctr;
    CData/*7:0*/ tb__DOT__dut__DOT__hs_ctr;
    CData/*0:0*/ tb__DOT__dut__DOT__pcnt_line_stored;
    CData/*0:0*/ tb__DOT__dut__DOT__meas_fid;
    CData/*0:0*/ tb__DOT__dut__DOT__hsync_i_pol;
    CData/*0:0*/ tb__DOT__dut__DOT__vsync_i_pol;
    CData/*0:0*/ tb__DOT__dut__DOT__meas_vblank_region;
    CData/*0:0*/ tb__DOT__dut__DOT__VSYNC_i_np;
    CData/*0:0*/ tb__DOT__dut__DOT__HSYNC_i_np;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb__DOT__clk__0;
    CData/*0:0*/ __Vtrigprevexpr_hec9a9fe2__0;
    CData/*0:0*/ __VactDidInit;
    CData/*0:0*/ __VactContinue;
    SData/*10:0*/ tb__DOT__vtotal;
    SData/*11:0*/ tb__DOT__dut__DOT__pcnt_line;
    SData/*11:0*/ tb__DOT__dut__DOT__pcnt_line_ctr;
    SData/*11:0*/ tb__DOT__dut__DOT__pcnt_line_prev;
    SData/*11:0*/ tb__DOT__dut__DOT__meas_h_cnt;
    SData/*11:0*/ tb__DOT__dut__DOT__meas_h_cnt_sogref;
    SData/*10:0*/ tb__DOT__dut__DOT__meas_v_cnt;
    SData/*11:0*/ tb__DOT__dut__DOT__meas_h_cnt_ref;
    SData/*11:0*/ tb__DOT__dut__DOT__meas_even_max_thold;
    IData/*31:0*/ tb__DOT__PAL;
    IData/*31:0*/ tb__DOT__MV;
    IData/*31:0*/ tb__DOT__NF;
    IData/*31:0*/ tb__DOT__VA;
    IData/*31:0*/ tb__DOT__VP;
    IData/*31:0*/ tb__DOT__VHS;
    IData/*31:0*/ tb__DOT__VJ;
    IData/*31:0*/ tb__DOT__HL;
    IData/*31:0*/ tb__DOT__FHL;
    IData/*31:0*/ tb__DOT__NEQ;
    IData/*31:0*/ tb__DOT__k;
    IData/*31:0*/ tb__DOT__f;
    IData/*31:0*/ tb__DOT__off;
    IData/*31:0*/ tb__DOT__w;
    IData/*31:0*/ tb__DOT__npos;
    IData/*31:0*/ tb__DOT__tc;
    IData/*19:0*/ tb__DOT__pcnt_field;
    IData/*31:0*/ tb__DOT__gen__DOT__start;
    IData/*31:0*/ tb__DOT__gen__DOT__p;
    IData/*20:0*/ tb__DOT__dut__DOT__pcnt_frame_ctr;
    IData/*17:0*/ tb__DOT__dut__DOT__syncpol_det_ctr;
    IData/*17:0*/ tb__DOT__dut__DOT__hsync_hpol_ctr;
    IData/*17:0*/ tb__DOT__dut__DOT__vsync_hpol_ctr;
    IData/*31:0*/ __VactIterCount;
    double tb__DOT__ph;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h78add7c0__0;
    VlTriggerScheduler __VtrigSched_h8aadc0b9__0;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<3> __VactTriggered;
    VlTriggerVec<3> __VnbaTriggered;

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

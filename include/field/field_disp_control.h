// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_disp_control.h, the lines in
// use so far.
#ifndef BWPORT_FIELD_FIELD_DISP_CONTROL_H
#define BWPORT_FIELD_FIELD_DISP_CONTROL_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

void FieldDispControl_ReqAdjustAlphaA(FieldDispControl *control, u32 alpha, u32 complement);
void FieldDispControl_ReqSetAlphaA(FieldDispControl *control, u32 alpha, u32 beta, u32 planeMask, u32 complement);
void FieldDispControl_ReqSetBGEnabled(FieldDispControl *control, u32 bgId, BOOL enabled);

// Black 2's; twinned code confirms the offsets of requestA, requestB, alphaA, betaA, planeMaskA, complementA, allA,
// alphaB, betaB, planeMaskB, complementB, bgEnabled, brightnessRequest and brightnessValue, and its size in Black
struct FieldDispControl {
    u16 requestA;
    u16 requestB;
    u32 alphaA;
    u32 betaA;
    u32 planeMaskA;
    u32 complementA;
    u32 allA;
    u32 alphaB;
    u32 betaB;
    u32 planeMaskB;
    u32 complementB;
    u32 unk28;
    u8 bgEnabled[8];
    u8 brightnessRequest[2];
    s8 brightnessValue[2];
};

FieldDispControl *FieldDispControl_Create(HeapID heapId);
void FieldDispControl_Update(FieldDispControl *control);
void FieldDispControlProc_ResetBrightness(const u32 *params, u32 screen);
void FieldDispControlProc_SetAlpha(const u32 *params, u32 screen);
void FieldDispControlProc_SetBrightness(const u32 *params, u32 screen);
void FieldDispControlProc_SetAll(const u32 *params, u32 screen);
void FieldDispControlProc_AdjustAlpha(const u32 *params, u32 screen);
void FieldDispControlProc_AdjustBrightness(const u32 *params, u32 screen);
void FieldDispControl_ReqSetAllA(FieldDispControl *control, u32 alpha, u32 beta, u32 planeMask, u32 complement,
                                 u32 all);
void FieldDispControl_ReqSetAlphaB(FieldDispControl *control, u32 alpha, u32 beta, u32 planeMask, u32 complement);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldDispControl_Free(FieldDispControl *control);

#endif // BWPORT_FIELD_FIELD_DISP_CONTROL_H

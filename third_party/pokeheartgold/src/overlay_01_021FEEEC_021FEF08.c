#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FEEEC.h"

void ov01_021FEF08(void *a0) {
    ov01_021FEFF8(a0);
    ov01_021F1448(a0);
}


void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);

s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);

extern s32 ov01_02209160;

extern s32 ov01_02209178;

extern s32 ov01_02209190;

extern s32 ov01_022091A8;

void ov01_021FEF18(FieldEffectRendererContext *arg0);

void ov01_021FEF18(FieldEffectRendererContext *arg0) {
    ov01_021F18D4(arg0->fieldEffMan, 0, 0x7E);
    ov01_021F18D4(arg0->fieldEffMan, 5, 0x7F);
    ov01_021F1908(arg0->fieldEffMan, 0, 0x8C);
    ov01_021F1908(arg0->fieldEffMan, 3, 0x8D);
    ov01_021F1908(arg0->fieldEffMan, 4, 0x8E);
    ov01_021F1908(arg0->fieldEffMan, 5, 0x8F);
    ov01_021F1930(arg0->fieldEffMan, 0, 0x19, 1);
    ov01_021F1930(arg0->fieldEffMan, 4, 0x10, 1);
    ov01_021F1930(arg0->fieldEffMan, 5, 0, 1);
    ov01_021F1930(arg0->fieldEffMan, 6, 1, 1);
    ov01_021F1758(arg0->fieldEffMan, 0, 0, 0, 0, 0, &ov01_02209178);
    ov01_021F1758(arg0->fieldEffMan, 5, 5, 3, 4, 0, &ov01_02209190);
    ov01_021F1758(arg0->fieldEffMan, 6, 5, 4, 5, 0, &ov01_02209160);
    ov01_021F1758(arg0->fieldEffMan, 7, 5, 5, 6, 0, &ov01_022091A8);
}

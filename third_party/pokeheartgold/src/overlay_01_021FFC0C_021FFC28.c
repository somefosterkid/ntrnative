#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FFC0C.h"

void ov01_021FFC28(void *a0) {
    ov01_021FFC80(a0);
    ov01_021F1448(a0);
}


void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);

s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);

extern s32 ov01_02209258;

void ov01_021FFC38(FieldEffectRendererContext *arg0);

void ov01_021FFC38(FieldEffectRendererContext *arg0) {
    ov01_021F18D4(arg0->fieldEffMan, 8, 0x7B);
    ov01_021F1908(arg0->fieldEffMan, 8, 0x93);
    ov01_021F1930(arg0->fieldEffMan, 9, 0x16, 1);
    ov01_021F1758(arg0->fieldEffMan, 0xA, 8, 8, 9, 0, &ov01_02209258);
}

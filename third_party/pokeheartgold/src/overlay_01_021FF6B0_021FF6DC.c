#include "global.h"
#include "overlay_01_021F1348.h"


void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);

s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);

extern s32 ov01_02209200;

void ov01_021FF6DC(FieldEffectRendererContext *arg0);

void ov01_021FF6DC(FieldEffectRendererContext *arg0) {
    ov01_021F18D4(arg0->fieldEffMan, 6, 0x79);
    ov01_021F1908(arg0->fieldEffMan, 6, 0x91);
    ov01_021F1930(arg0->fieldEffMan, 7, 0x14, 1);
    ov01_021F1758(arg0->fieldEffMan, 8, 6, 6, 7, 0, &ov01_02209200);
}

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);

s32 ov01_021F1924(FieldEffectManager *fieldEffMan, s32);

s32 ov01_021F1970(FieldEffectManager *fieldEffMan, s32);

void ov01_021FF724(FieldEffectRendererContext *arg0);

void ov01_021FF724(FieldEffectRendererContext *arg0) {
    ov01_021F18FC(arg0->fieldEffMan, 6);
    ov01_021F1924(arg0->fieldEffMan, 6);
    ov01_021F1970(arg0->fieldEffMan, 7);
    ov01_021F18C8(arg0->fieldEffMan, 8);
}

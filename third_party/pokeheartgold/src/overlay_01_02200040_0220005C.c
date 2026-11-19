#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_02200040.h"

void ov01_0220005C(void *a0) {
    ov01_022000B4(a0);
    ov01_021F1448(a0);
}

s32 ov01_021F1758(FieldEffectManager *fieldEffMan, s32, s32, s32, s32, s32, s32 *);

void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);

s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);

extern s32 ov01_022092A8;

void ov01_0220006C(FieldEffectRendererContext *arg0);

void ov01_0220006C(FieldEffectRendererContext *arg0) {
    ov01_021F18D4(arg0->fieldEffMan, 9, 0x7C);
    ov01_021F1908(arg0->fieldEffMan, 9, 0x94);
    ov01_021F1930(arg0->fieldEffMan, 0xA, 0x17, 1);
    ov01_021F1758(arg0->fieldEffMan, 0xB, 9, 9, 0xA, 0, &ov01_022092A8);
}

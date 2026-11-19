#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FF854.h"

void ov01_021FF870(void *a0) {
    ov01_021FF8C8(a0);
    ov01_021F1448(a0);
}

s32 ov01_021F1758(FieldEffectManager *fieldEffMan, s32, s32, s32, s32, s32, s32 *);

void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);

s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);

extern s32 ov01_0220922C;

void ov01_021FF880(FieldEffectRendererContext *arg0);

void ov01_021FF880(FieldEffectRendererContext *arg0) {
    ov01_021F18D4(arg0->fieldEffMan, 7, 0x7A);
    ov01_021F1908(arg0->fieldEffMan, 7, 0x92);
    ov01_021F1930(arg0->fieldEffMan, 8, 0x15, 1);
    ov01_021F1758(arg0->fieldEffMan, 9, 7, 7, 8, 0, &ov01_0220922C);
}

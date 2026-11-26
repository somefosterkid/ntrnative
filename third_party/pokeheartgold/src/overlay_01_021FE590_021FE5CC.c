#include "global.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE590.h"


void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);

s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);

extern s32 ov01_022090AC;

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F1924(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_021F1970(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_021FE5CC(BerryPatchEffectCounter *arg0) {
    if (arg0->isEnabled == 0) {
        arg0->isEnabled = 1;
        ov01_021F18D4(arg0->fieldEffMan, 0xB, 0x80);
        ov01_021F1908(arg0->fieldEffMan, 0xB, 0x95);
        ov01_021F1930(arg0->fieldEffMan, 0xC, 0x1A, 1);
        ov01_021F1758(arg0->fieldEffMan, 0xD, 0xB, 0xB, 0xC, 0, &ov01_022090AC);
    }
}

void ov01_021FE61C(BerryPatchEffectCounter *counter) {
    if (counter->isEnabled == 1) {
        counter->isEnabled = 0;

        ov01_021F18FC(counter->fieldEffMan, 11);
        ov01_021F1924(counter->fieldEffMan, 11);
        ov01_021F1970(counter->fieldEffMan, 12);
        ov01_021F18C8(counter->fieldEffMan, 13);
    }
}

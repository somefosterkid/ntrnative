#include "global.h"
#include "overlay_01_021F1348.h"

void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

void ov01_021FDA40(FieldEffectRendererContext *arg0);

void ov01_021FDA40(FieldEffectRendererContext *arg0) {
    ov01_021F18D4(arg0->fieldEffMan, 2, 0x23);
    ov01_021F18D4(arg0->fieldEffMan, 0xD, 0x69);
}

void ov01_021FDA5C(FieldEffectRendererContext *arg0);

void ov01_021FDA5C(FieldEffectRendererContext *arg0) {
    ov01_021F18FC(arg0->fieldEffMan, 2);
    ov01_021F18FC(arg0->fieldEffMan, 0xD);
}

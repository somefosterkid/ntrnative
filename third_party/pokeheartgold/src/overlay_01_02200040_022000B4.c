#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_02200040.h"

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F1924(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_021F1970(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_022000B4(FieldEffectRendererContext *param0) {
    ov01_021F18FC(param0->fieldEffMan, 9);
    ov01_021F1924(param0->fieldEffMan, 9);
    ov01_021F1970(param0->fieldEffMan, 10);
    ov01_021F18C8(param0->fieldEffMan, 11);
}

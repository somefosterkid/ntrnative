#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FFC0C.h"

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F1924(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_021F1970(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_021FFC80(FieldEffectRendererContext *param0) {
    ov01_021F18FC(param0->fieldEffMan, 8);
    ov01_021F1924(param0->fieldEffMan, 8);
    ov01_021F1970(param0->fieldEffMan, 9);
    ov01_021F18C8(param0->fieldEffMan, 10);
}

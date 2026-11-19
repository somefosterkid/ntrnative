#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_022006A8.h"

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F1970(FieldEffectManager *fieldEffMan, u32 param1);
void ov01_02200710(FieldEffectRendererContext *param0) {
    ov01_021F18FC(param0->fieldEffMan, 4);
    ov01_021F1970(param0->fieldEffMan, 3);
    ov01_021F18C8(param0->fieldEffMan, 4);
}

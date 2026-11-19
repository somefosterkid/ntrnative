#include "global.h"
#include "overlay_01_021F1348.h"

void ov01_021F15EC(FieldEffectManager *fieldEffMan);

void FieldEffectManager_InitAnimManagerList(FieldEffectManager *fieldEffMan, u32 animManagerCount) {
    fieldEffMan->animManCount = animManagerCount;
    ov01_021F15EC(fieldEffMan);
}

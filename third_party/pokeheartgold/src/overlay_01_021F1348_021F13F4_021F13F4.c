#include "global.h"

#include "heap.h"
#include "overlay_01_021F1348.h"

s32 ov01_021F1490(FieldEffectManager *);

s32 ov01_021F1538(FieldEffectManager *);

s32 ov01_021F15FC(FieldEffectManager *fieldEffMan);

s32 ov01_021F16B8(FieldEffectManager *);

void FieldEffectManager_Free(FieldEffectManager *fieldEffectManager) {
    ov01_021F15FC(fieldEffectManager);
    ov01_021F1538(fieldEffectManager);
    ov01_021F16B8(fieldEffectManager);
    ov01_021F1490(fieldEffectManager);
    Heap_Free(fieldEffectManager->renderers);
    Heap_Free(fieldEffectManager);
}

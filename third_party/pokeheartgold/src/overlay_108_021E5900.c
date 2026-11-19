#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "sound_02004A44.h"
#include "constants/heap.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E59E4(SafariAreaCustomizer *arg0, SafariAreaCustomizerArgs *arg1);

s32 SafariAreaCustomizer_Init(OverlayManager *man, s32 *state);

void ov108_021E5A48(SafariAreaCustomizer *arg0);
s32 SafariAreaCustomizer_Exit(OverlayManager *man, s32 *state);

s32 SafariAreaCustomizer_Init(OverlayManager *man, s32 *state) {
    SafariAreaCustomizer *dest;
    SafariAreaCustomizerArgs *v0;

    v0 = OverlayManager_GetArgs(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_95, 0x50000U);
    dest = OverlayManager_CreateAndGetData(man, sizeof(SafariAreaCustomizer), HEAP_ID_95);
    MI_CpuFill8(dest, 0U, sizeof(SafariAreaCustomizer));
    dest->heapID = HEAP_ID_95;
    ov108_021E59E4(dest, v0);
    Sound_SetSceneAndPlayBGM(0x43U, 0U, 0);
    return 1;
}

s32 SafariAreaCustomizer_Exit(OverlayManager *man, s32 *state) {
    SafariAreaCustomizer *dest;
    enum HeapID heapID;

    dest = OverlayManager_GetData(man);
    ov108_021E5A48(dest);
    heapID = dest->heapID;
    MI_CpuFill8(dest, 0U, sizeof(SafariAreaCustomizer));
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);
    return 1;
}

#include "global.h"

#include "heap.h"
#include "overlay_manager.h"

s32 LegendaryCinematic_Exit(OverlayManager *man, s32 *state);

s32 LegendaryCinematic_Exit(OverlayManager *man, s32 *state) {
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_153);
    return 1;
}

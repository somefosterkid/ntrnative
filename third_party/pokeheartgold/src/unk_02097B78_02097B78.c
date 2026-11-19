#include "global.h"

#include "heap.h"
#include "overlay_106_legendary_cinematic.h"
#include "overlay_manager.h"

BOOL LegendaryCinematic_Init(OverlayManager *man, int *state) {
    LegendaryCinematic *dest;

    Heap_Create(HEAP_ID_3, HEAP_ID_153, 0x80000U);
    dest = OverlayManager_CreateAndGetData(man, sizeof(LegendaryCinematic), HEAP_ID_153);
    MI_CpuFill8(dest, 0U, sizeof(LegendaryCinematic));
    dest->args = OverlayManager_GetArgs(man);
    return 1;
}

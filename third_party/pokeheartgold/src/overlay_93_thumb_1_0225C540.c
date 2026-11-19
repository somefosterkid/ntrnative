#include "global.h"

#include "heap.h"
#include "overlay_93_thumb_1.h"
#include "overlay_manager.h"

s32 ov93_0225C6D8(void *, void *);

s32 ov93_0225C540(OverlayManager *man) {
    void *dest;
    void *v0;

    v0 = OverlayManager_GetArgs(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_117, 0x60000U);
    dest = OverlayManager_CreateAndGetData(man, 0x40U, HEAP_ID_117);
    MI_CpuFill8(dest, 0U, 0x40U);
    ov93_0225C6D8(dest, v0);
    return 1;
}

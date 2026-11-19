#include "global.h"

#include "heap.h"
#include "overlay_manager.h"

s32 sub_020915F0(s32, s32);

s32 sub_02091634(OverlayManager *arg0);

s32 sub_02091634(OverlayManager *man) {
    Heap_Create(HEAP_ID_3, HEAP_ID_48, 0x41000U);
    sub_020915F0(*(s32 *)((u8 *)(OverlayManager_GetArgs(man)) + 8), 0x30);
    Heap_Destroy(HEAP_ID_48);
    OS_ResetSystem(0U);
    return 1;
}

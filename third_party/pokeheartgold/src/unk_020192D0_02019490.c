#include "global.h"
struct UnkStruct_02017498;

#include "heap.h"
#include "overlay_manager.h"
#include "ptr_align.h"
#include "system.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_020915B0.h"
#include "unk_struct_02017498.h"

s32 sub_020194F8();

s32 sub_02019490(OverlayManager *man, s32 *state);

s32 sub_02019490(OverlayManager *man, s32 *state) {
    void **v0;

    v0 = OverlayManager_GetData(man);
    sub_020194F8();
    Heap_Free(*v0);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_123);
    return 1;
}

void sub_020194B4(struct UnkStruct_02017498 *arg0);

void sub_020194B4(UnkStruct_02017498 *arg0) {
    void *v0;

    if (*((s32 *)&arg0->unk_88) == 0) {
        LoadDwcOverlay();
        LoadOVY38();
        sub_02039FD8(HEAP_ID_123);
        v0 = Heap_Alloc(HEAP_ID_123, 0x20020U);
        arg0->unk_18 = v0;
        arg0->unk_1C = NNS_FndCreateExpHeapEx(PTR_ALIGN_UP(v0, 0x20), 0x20000U, 0U);
        sub_02034D8C();
        Sys_ClearSleepDisableFlag(4);
    }
}

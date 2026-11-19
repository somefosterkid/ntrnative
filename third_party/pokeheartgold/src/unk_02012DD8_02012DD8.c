#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "unk_02012DD8.h"
#include "wipe_screen.h"
#include "unk_struct_ov66_022342_f4.h"

s32 sub_02012E6C(void *, s32, s32, s32, s32, void *, s32, enum HeapID);

void sub_02012DD8(void *arg0, s32 arg1) {
    ((WipeScreen *)arg0)->work = Heap_Alloc(*((enum HeapID *)&((WipeScreen *)arg0)->heapId), 0x4CU);
    sub_02012E6C(((WipeScreen *)arg0)->work, arg1, *(s32 *)((u8 *)arg0 + 4), *(s32 *)((u8 *)arg0 + 8), ((WipeScreen *)arg0)->screen, *((void **)&((WipeScreen *)arg0)->wnd), *((s32 *)&((WipeScreen *)arg0)->hblank), *((enum HeapID *)&((WipeScreen *)arg0)->heapId));
    *(s32 *)((u8 *)arg0 + 0xC) = (s32)(*(s32 *)((u8 *)arg0 + 0xC) + 1);
}

void *sub_02010EC8(void *);

s32 sub_02010F34(s32, s32, s32);

s32 sub_02012F54(void *);

s32 sub_02012E10(void *arg0) {
    s32 v0;
    s32 v2;
    void *v1;

    v0 = *(s32 *)((u8 *)arg0 + 0xC);
    v2 = 0;
    v1 = ((WipeScreen *)arg0)->work;
    switch (v0) {
    case 1:
        if (sub_02012F54(v1) == 1) {
            sub_02010F34(*((s32 *)&((UnkStruct_ov66_022342F4 *)v1)->unk_34), *((s32 *)&((UnkStruct_ov66_022342F4 *)v1)->unk_44), ((WipeScreen *)arg0)->screen);
            *(s32 *)((u8 *)arg0 + 0xC) = (s32)(*(s32 *)((u8 *)arg0 + 0xC) + 1);
        }
        break;
    case 2:
        sub_02010EC8(v1);
        Heap_Free(((WipeScreen *)arg0)->work);
        ((WipeScreen *)arg0)->work = NULL;
        v2 = 1;
        *(s32 *)((u8 *)arg0 + 0xC) = (s32)(*(s32 *)((u8 *)arg0 + 0xC) + 1);
        break;
    case 3:
        v2 = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return v2;
}

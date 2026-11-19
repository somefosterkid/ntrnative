#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"
#include "overlay_96_app_0220a87c.h"

s32 ov96_0220AF30(UnkStruct_ov96_0220B148 *);
s32 ov96_0220B1B8(void *);

void ov96_0220A7F8(Ov96App_0220A87C *ptr);

void ov96_0220A7F8(Ov96App_0220A87C *ptr) {
    s32 v0;

    if (ptr == NULL) {
        GF_AssertFail();
    }
    if (ptr->unk_C == NULL) {
        GF_AssertFail();
    }
    Sprite_DeleteAndFreeResources(ptr->unk_C);
    Sprite_DeleteAndFreeResources(ptr->unk_10);
    v0 = 0;
    do {
        ov96_0220AF30(ptr->unk_154[v0]);
        v0 += 1;
    } while (v0 < 0xA);
    ov96_0220B1B8(ptr->unk_14);
    Heap_Free(ptr);
}

s32 ov96_0220AD64(Ov96App_0220A87C *);
s32 ov96_0220AE28(Ov96App_0220A87C *);
s32 ov96_0220B324(void *);

void ov96_0220A840(Ov96App_0220A87C *arg0);

void ov96_0220A840(Ov96App_0220A87C *arg0) {
    u32 v0;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    v0 = (u32)(arg0->unk_180 << 0x1C) >> 0x1C;
    switch (v0) {
    case 1:
        ov96_0220AD64(arg0);
        break;
    case 2:
        ov96_0220AE28(arg0);
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov96_0220B324(arg0->unk_14);
}

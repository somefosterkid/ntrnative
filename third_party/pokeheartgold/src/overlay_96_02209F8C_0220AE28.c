#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"
#include "overlay_96_app_0220a87c.h"

void ov96_0220AE28(Ov96App_0220A87C *param_1);

void ov96_0220AE28(Ov96App_0220A87C *param_1) {
    if (((u32)param_1->unk_180 & 0xff) >> 4 == 0) {
        param_1->unk_180 = (u32)param_1->unk_180 & 0xfffffff0 | 1;
    }
    return;
}

ManagedSprite *ov96_0220D13C(SpriteSystem *, SpriteManager *, s32, s32, s32, s32);

UnkStruct_ov96_0220B148 *ov96_0220AE40(enum HeapID arg0, SpriteSystem *arg1, SpriteManager *arg2, s32 arg3);

UnkStruct_ov96_0220B148 *ov96_0220AE40(enum HeapID heapID, SpriteSystem *arg1, SpriteManager *arg2, s32 arg3) {
    ManagedSprite *managedSprite;
    ManagedSprite *v0;
    u16 v1;
    UnkStruct_ov96_0220B148 *dest;

    if (arg1 == 0) {
        GF_AssertFail();
    }
    if (arg2 == 0) {
        GF_AssertFail();
    }
    dest = Heap_Alloc(heapID, sizeof(UnkStruct_ov96_0220B148));
    MI_CpuFill8(dest, 0U, sizeof(UnkStruct_ov96_0220B148));
    dest->unk_10 = (s32)((dest->unk_10 & 0xF80FFFFF) | ((u32)(arg3 << 0x19) >> 5));
    v1 = 0x16 - (arg3 * 2);
    managedSprite = ov96_0220D13C(arg1, arg2, 0, 0, 4, (s32)v1);
    dest->unk_4 = managedSprite;
    ManagedSprite_SetDrawFlag(managedSprite, 0);
    v0 = ov96_0220D13C(arg1, arg2, 0x78, 0, 1, (s32)(u16)(v1 + 1));
    dest->unk_0 = v0;
    ManagedSprite_SetDrawFlag(v0, 0);
    ManagedSprite_SetAnimateFlag(dest->unk_0, 1);
    return dest;
}

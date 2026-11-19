#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"

typedef struct UnkStruct_ov07_02228F14_ptr {
    u8 unk_0;
    u8 unk_1[0x3];
    void *unk_4;
    u8 unk_8[0x14];
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20;
    s16 unk_22;
    u8 unk_24[0x8];
    void *unk_2C;
    u8 unk_30[0x8];
    u8 unk_38;
} UnkStruct_ov07_02228F14_ptr;

s32 ov07_022227A8(void *, s16, s16, u32, s32);

s32 ov07_0222283C(void *, s32, s32, void *);

void ov07_02228F14(SysTask *arg0, void *param);

void ov07_02228F14(SysTask *arg0, void *param) {
    UnkStruct_ov07_02228F14_ptr *ptr = param;
    Point2D v0;
    u8 v1;

    v1 = ptr->unk_0;
    switch (v1) {
    case 0:
        ov07_022227A8((void *)&ptr->unk_38, ptr->unk_1C, ptr->unk_1E, ptr->unk_20, (s32) ptr->unk_22);
        ptr->unk_0 = (u8)(ptr->unk_0 + 1);
        return;
    case 1:
        ov07_02231FA0(ptr->unk_2C, &v0);
        if (ov07_0222283C((void *)&ptr->unk_38, v0.x, v0.y, ptr->unk_2C) == 0) {
            ptr->unk_0 = (u8)(ptr->unk_0 + 1);
            return;
        }
        return;
    default:
        ov07_0221C448(ptr->unk_4, arg0);
        Heap_Free(ptr);
        break;
    }
}

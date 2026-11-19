#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

void ov14_021E6C0C(struct UnkStruct_ov14_boxapp *arg0, void *arg1);

s32 ov14_021E6B48(void *, void *);
void ov14_021E6CC8(struct UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);

void ov14_021E6C0C(UnkStruct_ov14_boxapp *arg0, void *arg1) {
    s16 y[2];
    u8 *v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v5;
    u8 v4;

    v0 = *((u8 **)&arg0->sys);
    v4 = *(u8 *)((u8 *)(v0 + *(s32 *)((u8 *)arg1 + 8)) + 0x4094);
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)(v0 + (*(u8 *)((u8 *)(v0 + *(s32 *)((u8 *)arg1 + 4)) + 0x4094) * 4)) + 0x2FC), arg1 + 0x1C, arg1 + 0x1E);
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)(*((void **)&arg0->sys) + (v4 * 4)) + 0x2FC), &y[1], &y[0]);
    if ((s32) * (s16 *)((u8 *)arg1 + 0x1C) <= (s32) y[1]) {
        *(s16 *)((u8 *)arg1 + 0x18) = 1;
        v5 = y[1] - *(s16 *)((u8 *)arg1 + 0x1C);
    } else {
        *(s16 *)((u8 *)arg1 + 0x18) = -1;
        v5 = *(s16 *)((u8 *)arg1 + 0x1C) - y[1];
    }
    v1 = v5 << 0x10;
    *(s32 *)((u8 *)arg1 + 0x10) = (s32)((s32)(v1 + ((u32)(v1 >> 2) >> 0x1D)) >> 3);
    if ((s32) * (s16 *)((u8 *)arg1 + 0x1E) <= (s32) y[0]) {
        *(s16 *)((u8 *)arg1 + 0x1A) = 1;
        v2 = (y[0] - *(s16 *)((u8 *)arg1 + 0x1E)) << 0x10;
        *(s32 *)((u8 *)arg1 + 0x14) = (s32)((s32)(v2 + ((u32)(v2 >> 2) >> 0x1D)) >> 3);
        return;
    }
    *(s16 *)((u8 *)arg1 + 0x1A) = -1;
    v3 = (*(s16 *)((u8 *)arg1 + 0x1E) - y[0]) << 0x10;
    *(s32 *)((u8 *)arg1 + 0x14) = (s32)((s32)(v3 + ((u32)(v3 >> 2) >> 0x1D)) >> 3);
}

void ov14_021E6CC8(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    void *v0;

    v0 = ((UnkStruct_ov14_boxsys *)arg0->sys)->unk_C;
    *(s32 *)((u8 *)v0 + 4) = arg1;
    *(s32 *)((u8 *)v0 + 8) = arg2;
    *(s32 *)((u8 *)v0 + 0xC) = 1;
    ov14_021E6B48(arg0, v0);
    *(s32 *)((u8 *)v0 + 0x24) = arg2;
    *(s32 *)((u8 *)v0 + 0x28) = arg1;
    *(s32 *)((u8 *)v0 + 0x2C) = 1;
    ov14_021E6B48(arg0, v0 + 0x20);
}

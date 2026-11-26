#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "pokemon_storage_system.h"
#include "sprite_system.h"
#include "unk_02013534.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F46B0(struct UnkStruct_ov14_boxapp *arg0, u8 *arg1, u32 arg2, s32 arg3, u32 arg4);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F49E0(void *);
void ov14_021F4720(void *arg0);

void ov14_021F46B0(UnkStruct_ov14_boxapp *arg0, u8 *arg1, u32 arg2, s32 arg3, u32 arg4) {
    u32 v0;
    u8 v1;

    if (arg2 >= 0x10U) {
        if (PCStorage_IsBonusWallpaperUnlocked(arg0->storage, arg2 - 0x10) == 0) {
            v1 = 0x28;
        } else {
            v1 = arg2 + 0x10;
        }
    } else {
        v1 = arg2 + 0x10;
    }
    v0 = 0;
    while (v0 < arg4) {
        if (arg3 == *(u8 *)(arg1 + v0)) {
            *(u8 *)(arg1 + v0) = v1;
        }
        v0 += 1;
    }
}

void ov14_021F46F4(UnkStruct_ov14_boxsys *arg0) {
    sub_020136B4(arg0->textObjs[0].textObj, -0x48, -8);
    sub_020136B4(arg0->textObjs[1].textObj, 0x20, -8);
}

void ov14_021F4720(void *arg0) {
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[4], 0xC, -0x15);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[5], 0xF4, -0x15);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[6], 0x2B, -0x15);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[7], 0x80, -0x28);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[8], 0x80, -0x1C);
    ov14_021F49E0(arg0);
    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 7, 5);
    ov14_021F46F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    TextOBJ_SetSpritesDrawFlag(((UnkStruct_ov14_boxapp *)arg0)->sys->textObjs[0].textObj, 1);
    TextOBJ_SetSpritesDrawFlag(((UnkStruct_ov14_boxapp *)arg0)->sys->textObjs[1].textObj, 1);
}

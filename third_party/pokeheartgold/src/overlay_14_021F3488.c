#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2A9C(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

void ov14_021F34DC(void *arg0, s32 arg1);

void ov14_021F3420(void *arg0, s32 arg1, u32 arg2, u32 arg3, s32 arg4);

void ov14_021F3488(void *arg0, s32 arg1, s32 arg2);
void ov14_021F34C8(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

void ov14_021F3488(void *arg0, s32 arg1, s32 arg2) {
    s32 v0;

    if (0x80 & arg1) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    if (1 & arg1) {
        ov14_021F3420(arg0, arg2, 0, 0x1E, v0);
    }
    if (2 & arg1) {
        ov14_021F3420(arg0, arg2, 0x1E, 0x24, v0);
    }
}

void ov14_021F34C8(UnkStruct_ov14_boxsys * param_1, s32 param_2, s32 param_3) {
    ov14_021F2A9C(param_1, (u32) * &param_1->unk_4094[param_2], param_3);
    return;
}

void ov14_021F34DC(void *arg0, s32 arg1) {
    ov14_021F3420(arg0, 1, arg1, arg1 + 1, 1);
}

void ov14_021F34EC(UnkStruct_ov14_boxsys *arg0) {
    SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_0_8, 0x4C, 0, 2, 0xC11D);
    SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_0_8, 0x4C, 0, 2, 0xC11E);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_0_8, 0x4B, 0, 1, 2, 0xC0FA);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_0_8, 0x4B, 0, 1, 2, 0xC0FB);
    SpriteSystem_LoadCellResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_0_8, 0x4D, 0, 0xC0FA);
    SpriteSystem_LoadAnimResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_0_8, 0x4E, 0, 0xC0FA);
}

extern ManagedSpriteTemplate ov14_021F8210;

extern ManagedSpriteTemplate ov14_021F8244;

void ov14_021F35BC(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *v0;

    v0 = arg0->sys;
    v0->sprites[2] = SpriteSystem_NewSpriteWithYOffset(v0->unk_2F4, ((SpriteManager *)v0->unk_2F8), &ov14_021F8210, 0x200000);
    v0->sprites[3] = SpriteSystem_NewSpriteWithYOffset(v0->unk_2F4, ((SpriteManager *)v0->unk_2F8), &ov14_021F8244, 0x200000);
    ManagedSprite_SetDrawFlag(v0->sprites[2], 0);
    ManagedSprite_SetDrawFlag(v0->sprites[3], 0);
}

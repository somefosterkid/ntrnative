#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F3BC0(UnkStruct_ov14_boxsys *arg0);


void ov14_021F3BC0(UnkStruct_ov14_boxsys *arg0) {
    s8 v0[2];

    sub_02019B1C(arg0->unk_2F0, 0x10, &v0[1], &v0[0]);
    ManagedSprite_SetPositionXY(arg0->sprites[12], 0x12, (s16)((v0[0] * 8) + 0x168));
    ManagedSprite_SetDrawFlag(arg0->sprites[12], 1);
}

void ov14_021F3C08(UnkStruct_ov14_boxsys *arg0) {
    SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x52, 1, 2, 0xC121);
    SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x52, 1, 2, 0xC122);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_0_8, 0x4A, 0, 3, 2, 0xC0FE);
    SpriteSystem_LoadCellResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x53, 1, 0xC0FC);
    SpriteSystem_LoadAnimResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x54, 1, 0xC0FC);
}

extern ManagedSpriteTemplate ov14_021F844C;

extern ManagedSpriteTemplate ov14_021F8480;

void ov14_021F3CB4(UnkStruct_ov14_boxsys *arg0) {
    arg0->sprites[13] = SpriteSystem_NewSpriteWithYOffset(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F844C, 0x200000);
    arg0->sprites[14] = SpriteSystem_NewSpriteWithYOffset(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F8480, 0x200000);
    ManagedSprite_SetDrawFlag(arg0->sprites[13], 0);
    ManagedSprite_SetDrawFlag(arg0->sprites[14], 0);
}

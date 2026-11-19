#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_mon.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F3D0C(void *, u8, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F3D70(UnkStruct_ov14_boxsys *arg0, UnkStruct_ov14_mon *arg1) {
    u8 v0;

    if (arg1->isEgg != 0) {
        ManagedSprite_SetDrawFlag(arg0->sprites[13], 0);
        ManagedSprite_SetDrawFlag(arg0->sprites[14], 0);
        return;
    }
    ov14_021F3D0C(arg0, arg1->type1, 0xD, 0xC121);
    ManagedSprite_SetDrawFlag(arg0->sprites[13], 1);
    v0 = arg1->type2;
    if ((v0 != 0) && (arg1->type1 != v0)) {
        ov14_021F3D0C(arg0, v0, 0xE, 0xC122);
        ManagedSprite_SetDrawFlag(arg0->sprites[14], 1);
        return;
    }
    ManagedSprite_SetDrawFlag(arg0->sprites[14], 0);
}

void ov14_021F3DE8(UnkStruct_ov14_boxsys *arg0) {
    SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x42, 1, 1, 0xC12D);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x45, 0, 3, 1, 0xC101);
    SpriteSystem_LoadCellResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x43, 1, 0xC0FF);
    SpriteSystem_LoadAnimResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x44, 1, 0xC0FF);
}

extern ManagedSpriteTemplate ov14_021F81A8;

extern ManagedSpriteTemplate ov14_021F81DC;

extern ManagedSpriteTemplate ov14_021F8278;

extern ManagedSpriteTemplate ov14_021F82AC;

extern ManagedSpriteTemplate ov14_021F82E0;

extern ManagedSpriteTemplate ov14_021F8314;

extern ManagedSpriteTemplate ov14_021F8348;

extern ManagedSpriteTemplate ov14_021F837C;

extern ManagedSpriteTemplate ov14_021F83B0;

void ov14_021F3E70(UnkStruct_ov14_boxsys *arg0) {
    arg0->sprites[0] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F81A8);
    arg0->sprites[1] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F81DC);
    arg0->sprites[4] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F8278);
    arg0->sprites[5] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F82AC);
    arg0->sprites[6] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F82E0);
    arg0->sprites[7] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F8314);
    arg0->sprites[8] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F8348);
    arg0->sprites[9] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F837C);
    arg0->sprites[10] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F83B0);
    ov14_021F2A18(arg0, 0xA, 0);
}

#include "global.h"

#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2BE8(void *, u32);
void ov14_021F2C04(void *arg0);

void ov14_021F2C04(void *arg0) {
    u32 v0;

    v0 = 0;
    do {
        ov14_021F2BE8(arg0, v0);
        v0 += 1;
    } while (v0 < 0x45U);
}

void ov14_021F2C1C(void *arg0, s32 arg1, void *arg2, u32 arg3) {
    u32 offset;

    offset = NNS_G2dGetImageLocation(Sprite_GetImageProxy(((UnkStruct_ov14_boxsys *)arg0)->sprites[arg1]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(arg2, arg3);
    GX_LoadOBJ(arg2, offset, arg3);
}

void ov14_021F2C50(void *arg0, s32 arg1, void *arg2, u32 arg3) {
    u32 offset;

    offset = NNS_G2dGetImageLocation(Sprite_GetImageProxy(((UnkStruct_ov14_boxsys *)arg0)->sprites[arg1]->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    DC_FlushRange(arg2, arg3);
    GXS_LoadOBJ(arg2, offset, arg3);
}

void ov14_021F2C84(UnkStruct_ov14_boxsys *arg0) {
    u32 v0;

    v0 = 0;
    do {
        SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x4E, 1, 1, v0 + 0xC0F9);
        v0 += 1;
    } while (v0 < 0x24U);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), 0, 3, 1, 0xC0F9);
    SpriteSystem_LoadCellResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_poketool_icongra_poke_icon, (s32)sub_0207449C(), 0, 0xC0F9);
    SpriteSystem_LoadAnimResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_poketool_icongra_poke_icon, (s32)sub_020744A8(), 0, 0xC0F9);
}

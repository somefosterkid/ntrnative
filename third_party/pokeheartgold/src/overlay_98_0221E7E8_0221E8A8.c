#include "global.h"

#include "overlay_98.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_98_sprite_ctx.h"

extern s32 _0221F194;

void ov98_0221E8A8(PokeathlonSpriteContext *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4) {
    u32 v4;
    u32 v3;
    s32 vram;
    s32 fileId;
    s32 v2;
    s32 v1;
    void *v0;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    s32 v5;

    v0 = arg0;
    spriteSystem = arg0->spriteSystem;
    spriteManager = arg0->spriteManager;
    v1 = arg2;
    v2 = arg3;
    if (arg4 != 0) {
        fileId = 0x1C;
    } else {
        fileId = 0x1D;
    }
    v4 = sub_0207449C();
    v3 = sub_020744A8();
    v5 = 0;
    if (v1 > 0) {
        vram = *(s32 *)(&_0221F194 + v2);
        do {
            SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_7_7, fileId, 1, vram, *(s32 *)arg1 + v5);
            v5 += 1;
        } while (v5 < v1);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, (s32)v4, 0, *(s32 *)((u8 *)arg1 + 8));
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, (s32)v3, 0, *(s32 *)((u8 *)arg1 + 0xC));
    if (*(s8 *)((u8 *)v0 + 0x14) == -1) {
        *(s8 *)((u8 *)v0 + 0x14) = SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), 0, 3, *(s32 *)(&_0221F194 + v2), *(s32 *)((u8 *)arg1 + 4));
    }
}

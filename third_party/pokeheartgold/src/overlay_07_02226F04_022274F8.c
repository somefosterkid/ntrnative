#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_string_shot_sprite_context.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"

s32 ov07_0221BFC0(BattleAnimSystem *system);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

s32 ov07_0223192C(BattleAnimSystem *system, s32 battler);

void ov07_02231A20(s32 isPlayerSide, s32 mode, Point2D *pos);

s32 ov07_02231E08(BattleAnimSystem *system, s32 spriteAlpha, s32 bgAlpha);

void ov07_02227300(SysTask *task, void *param);

void ov07_022274F8(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_022274F8(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate unkTemplate;
    Point2D pos;
    ManagedSprite *managedSprite;
    s32 v5;
    s32 v6;
    StringShotSpriteContext *ctx;

    ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(StringShotSpriteContext));
    if (ctx == NULL) {
        GF_AssertFail();
    }
    ctx->delay = 0;
    ctx->state = 0;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->battleAnimSystem = system;
    unkTemplate = ov07_0221F9E8(system);
    ov07_02231E08(ctx->battleAnimSystem, -1, -1);
    ctx->spriteAlpha = 0xF;
    ctx->bgAlpha = 0;
    reg_G2_BLDALPHA = ctx->spriteAlpha | (ctx->bgAlpha << 8);
    ctx->sprites[0] = sprite;
    if (ov07_0223192C(system, ov07_0221C470(system)) == 3) {
        ov07_02231A20(0, ov07_0221BFC0(system), &pos);
    } else {
        ov07_02231A20(1, ov07_0221BFC0(system), &pos);
    }
    for (v5 = 1; v5 < ov07_0221C4A8(ctx->battleAnimSystem, 0); v5++) {
        managedSprite = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &unkTemplate);
        ctx->sprites[v5] = managedSprite;
        ManagedSprite_SetPositionXY(managedSprite, pos.x, pos.y);
    }
    ManagedSprite_SetPositionXY(ctx->sprites[0], pos.x, pos.y);
    for (v6 = 0; v6 < ov07_0221C4A8(ctx->battleAnimSystem, 0); v6++) {
        ctx->spriteCounters[v6] = 0;
        ManagedSprite_SetAffineOverwriteMode(ctx->sprites[v6], 2U);
        ManagedSprite_OffsetPositionXY(ctx->sprites[v6], 0, (s16)(0x20 - v6 * 4));
        ManagedSprite_SetOamMode(ctx->sprites[v6], GX_OAM_MODE_XLU);
    }
    ov07_0221C3F4(system, ov07_02227300, ctx, 0x1000);
}

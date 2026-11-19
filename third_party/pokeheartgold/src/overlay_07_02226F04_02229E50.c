#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_taunt_sprite_context.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

s32 ov07_0221BFC0(BattleAnimSystem *system);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

int ov07_0223192C(BattleAnimSystem *system, int battler);

s32 ov07_02231E08(BattleAnimSystem *system, s32 spriteAlpha, s32 bgAlpha);

void ov07_02229D40(SysTask *task, void *param);

void ov07_02229E50(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02229E50(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate unkTemplate;
    ManagedSprite *managedSprite;
    s32 v3;
    s32 v4;
    TauntSpriteContext *ctx;

    managedSprite = sprite;
    ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(TauntSpriteContext));
    if (ctx == NULL) {
        GF_AssertFail();
    }
    ctx->fadeInFrames = 0;
    ctx->state = 0;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->battleAnimSys = system;
    unkTemplate = ov07_0221F9E8(system);
    ov07_02231E08(ctx->battleAnimSys, -1, -1);
    ctx->handCount = ov07_0221C4A8(ctx->battleAnimSys, 0);
    ctx->spriteAlpha = 0;
    ctx->bgAlpha = 0xF;
    v3 = 1;
    reg_G2_BLDALPHA = ctx->spriteAlpha | (ctx->bgAlpha << 8);
    ctx->handSprites[0] = managedSprite;
    for (; v3 < (s32)ctx->handCount; v3++) {
        ctx->handSprites[v3] = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &unkTemplate);
    }
    if (ov07_0223192C(system, ov07_0221C470(system)) == 3) {
        if ((ov07_0223192C(system, ov07_0221C470(system)) == 3) && (ov07_0223192C(system, ov07_0221C468(system)) == 3)) {
            ManagedSprite_SetAnim(ctx->handSprites[0], 0);
        } else {
            ManagedSprite_SetAnim(ctx->handSprites[0], 1);
        }
        ManagedSprite_SetPositionXY(ctx->handSprites[0], 0x80, 0x50);
    } else {
        if ((ov07_0223192C(system, ov07_0221C470(system)) == 4) && (ov07_0223192C(system, ov07_0221C468(system)) == 4)) {
            ManagedSprite_SetAnim(ctx->handSprites[0], 1);
        } else {
            ManagedSprite_SetAnim(ctx->handSprites[0], 0);
        }
        ManagedSprite_SetPositionXY(ctx->handSprites[0], 0x80, 0x50);
    }
    for (v4 = 0; v4 < (s32)ctx->handCount; v4++) {
        ctx->fadeInCycles[v4] = 0;
        ManagedSprite_SetAffineOverwriteMode(ctx->handSprites[v4], 2U);
        ManagedSprite_SetOamMode(ctx->handSprites[v4], GX_OAM_MODE_XLU);
        if (ov07_0221BFC0(ctx->battleAnimSys) == 1) {
            ManagedSprite_SetAffineScale(ctx->handSprites[v4], -1.0f, 1.0f);
        }
    }
    ov07_0221C3F4(system, ov07_02229D40, ctx, 0x44C);
}

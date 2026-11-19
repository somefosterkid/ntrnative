#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_unused_sprite_context.h"

#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

void ov07_0222FA64(SysTask *task, void *param);

void ov07_0222FB24(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222FB24(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate unkTemplate;
    ManagedSprite *newSprite;
    Pokepic *pokepic;
    s32 i;
    UnusedSpriteContext *ctx;

    ctx = ov07_022324D8(system, sizeof(UnusedSpriteContext));
    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
    ctx->defenderSprite = pokepic;
    ctx->defenderX = (s16)Pokepic_GetAttr(pokepic, POKEPIC_X);
    ctx->defenderY = (s16)Pokepic_GetAttr(ctx->defenderSprite, POKEPIC_Y);
    ctx->defenderY = (s16)(ctx->defenderY - Pokepic_GetAttr(ctx->defenderSprite, POKEPIC_SHADOW_H));
    unkTemplate = ov07_0221F9E8(system);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            ctx->sprites[i] = sprite;
            ManagedSprite_SetAnim(sprite, 1);
        } else {
            newSprite = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &unkTemplate);
            ctx->sprites[i] = newSprite;
            ManagedSprite_SetOamMode(newSprite, GX_OAM_MODE_XLU);
        }
        ManagedSprite_SetDrawPriority(ctx->sprites[i], 0x64U);
        ManagedSprite_SetPriority(ctx->sprites[i], 1);
        ManagedSprite_SetPositionXY(ctx->sprites[i], ctx->defenderX, ctx->defenderY);
        ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
        ManagedSprite_SetAffineOverwriteMode(ctx->sprites[i], 2U);
    }
    ov07_0221C410(ctx->battleAnimSys, ov07_0222FA64, ctx);
}

#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "struct_defs/resdat_narc_structs.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

void ov07_0222A8D8(ManagedSprite *sprite);

typedef struct MetalClawSpriteContext {
    BattleAnimScriptFuncCommon common;
    s16 rightClawDelay;
    s16 counters[4];
    ManagedSprite *sprites[4];
} MetalClawSpriteContext;

void ov07_0222A8EC(SysTask *task, void *param);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);
void ov07_0222A988(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222A8EC(SysTask *task, void *param) {
    MetalClawSpriteContext *ctx = param;

    if (ctx->rightClawDelay <= 0) {
        ov07_0222A8D8(ctx->sprites[2]);
        ov07_0222A8D8(ctx->sprites[3]);
    } else {
        ctx->rightClawDelay--;
    }

    ov07_0222A8D8(ctx->sprites[0]);
    ov07_0222A8D8(ctx->sprites[1]);

    int doneCount = 0;
    int i;

    for (i = 0; i < 4; i++) {
        ctx->counters[i]++;

        if (ctx->counters[i] >= 40) {
            ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
            doneCount++;
        }
    }

    if (doneCount == 4) {
        for (i = 0; i < 4; i++) {
            Sprite_DeleteAndFreeResources(ctx->sprites[i]);
        }

        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}

void ov07_0222A988(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    MetalClawSpriteContext *ctx = ov07_022324D8(system, sizeof(MetalClawSpriteContext));
    ctx->rightClawDelay = 10;

    ov07_02231FE4(system, &ctx->common);

    ManagedSpriteTemplate template;
    template = ov07_0221F9E8(ctx->common.battleAnimSys);

    ctx->sprites[0] = sprite;
    ctx->counters[0] = 0;

    for (int i = 1; i < 4; i++) {
        ctx->counters[i] = 0;
        ctx->sprites[i] = SpriteSystem_NewSprite(ctx->common.spriteSystem, ctx->common.primarySpriteManager, &template);
    }

    ManagedSprite_SetFlipMode(ctx->sprites[0], 1);
    ManagedSprite_SetFlipMode(ctx->sprites[1], 1);

    ManagedSprite_OffsetPositionXY(ctx->sprites[0], -32, 0);
    ManagedSprite_OffsetPositionXY(ctx->sprites[1], -32, +32);
    ManagedSprite_OffsetPositionXY(ctx->sprites[2], +32, 0);
    ManagedSprite_OffsetPositionXY(ctx->sprites[3], +32, +32);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222A8EC, ctx);
}

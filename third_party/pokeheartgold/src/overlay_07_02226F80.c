#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_render_pokemon_sprites.h"

SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);

u16 ov07_0221C468(BattleAnimSystem *system);

u16 ov07_0221C470(BattleAnimSystem *system);

s32 ov07_0221C4A8(BattleAnimSystem *system, int index);

ManagedSprite *ov07_0221C4E8(BattleAnimSystem *system, int index);

int ov07_0221FB04(BattleAnimSystem *system, int bg);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common);

typedef struct SetPokemonSpritePriorityContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
    int spriteID;
    int maxFrames;
    int bg;
    int spritePriority;
    int mode;
    int windowType;
    int stepCount;
    int sinkFullyStartFrame;
} SetPokemonSpritePriorityContext;

void ov07_02226F04(SysTask *task, void *param);

void ov07_02226F80(BattleAnimSystem *system);

void ov07_0222719C(SysTask *task, void *param);

void ov07_02226F80(BattleAnimSystem *system) {
    SetPokemonSpritePriorityContext *ctx = ov07_022324D8(system, sizeof(SetPokemonSpritePriorityContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->spriteID = ov07_0221C4A8(system, 0);
    ctx->maxFrames = ov07_0221C4A8(system, 1);
    ctx->bg = ov07_0221C4A8(system, 2);
    ctx->spritePriority = ov07_0221C4A8(system, 3);
    ctx->mode = ov07_0221C4A8(system, 5);
    ctx->windowType = ov07_0221C4A8(system, 6);
    ctx->sprite = ov07_0221C4E8(ctx->common.battleAnimSys, ctx->spriteID);
    ctx->stepCount = 0;

    if (ctx->bg != 0xFF) {
        int bgPriority = ov07_0221FB04(system, ctx->bg);
        ManagedSprite_SetPriority(ctx->sprite, bgPriority);
    }

    if (ctx->spritePriority != 0xFF) {
        ManagedSprite_SetDrawPriority(ctx->sprite, ctx->spritePriority);
    }

    if (ov07_0221FAB0(system) != 1) {
        if (ov07_0221C4A8(system, 4) == 2 || ov07_0221C4A8(system, 4) == 3) {
            ManagedSprite_SetDrawFlag(ctx->sprite, 0);
            Heap_Free(ctx);
            return;
        }
    }

    int battlerRole = ov07_0221C4A8(system, 4);
    int attacker = ov07_0221C468(system);
    int defender = ov07_0221C470(system);

    if (ctx->spritePriority != 0xFF) {
        int attackerType = ov07_02231924(system, attacker);
        int defenderType = ov07_02231924(system, defender);

        switch (battlerRole) {
        case 0:
            switch (attackerType) {
            case 2:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            }
            break;
        case 2:
            switch (attackerType) {
            case 2:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            }
            break;
        case 1:
            switch (defenderType) {
            case 2:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            }
            break;
        case 3:
            switch (defenderType) {
            case 2:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(ctx->sprite, 20);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(ctx->sprite, 10);
                break;
            }
            break;
        }
    }

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02226F04, ctx);
}

void ov07_0222719C(SysTask *task, void *param) {
    RenderPokemonSpritesContext *ctx = param;
    ManagedSprite *managedSprite;
    s32 v0;
    s32 v1;

    v0 = ctx->frames;
    if (v0 == 0) {
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }
    ctx->frames = (s32)(v0 - 1);
    for (v1 = 0; v1 < 4; v1++) {
        managedSprite = ctx->sprites[v1];
        if (managedSprite != NULL) {
            ManagedSprite_TickFrame(managedSprite);
        }
    }
    SpriteSystem_DrawSprites(ctx->common.pokemonSpriteManager);
}

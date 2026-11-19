#include "global.h"

#include "pokepic.h"
#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_harden_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_splash_context.h"

enum HardenState {
    HARDEN_STATE_WAIT = 0,
    HARDEN_STATE_ENABLE_SPRITES,
    HARDEN_STATE_WAIT_DRAW,
};
void ov07_02228684(SysTask *task, void *param);

void ov07_02228638(BattleAnimSystem *system);

void ov07_02228638(BattleAnimSystem *system) {
    Pokepic *pokepic;
    SplashContext *ctx;

    ctx = ov07_022324D8(system, sizeof(SplashContext));
    ctx->state = 0;
    ctx->iteration = 0;
    ctx->battleAnimSys = system;
    pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->attackerSprite = pokepic;
    ctx->attackerY = Pokepic_GetAttr(pokepic, POKEPIC_Y);
    ctx->attackerHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ov07_0221C410(ctx->battleAnimSys, ov07_02228510, ctx);
}

void ov07_02228684(SysTask *task, void *param) {
    HardenContext *ctx = param;

    switch (ctx->state) {
    case HARDEN_STATE_WAIT:
        if (++ctx->timer >= 10) {
            ctx->timer = 0;
            ctx->state++;
        }
        break;
    case HARDEN_STATE_ENABLE_SPRITES:
        ManagedSprite_SetDrawFlag(ctx->attackerSprite1, 1);
        ManagedSprite_SetDrawFlag(ctx->attackerSprite2, 1);
        ManagedSprite_SetOamMode(ctx->attackerSprite2, GX_OAM_MODE_OBJWND);
        ctx->state++;
        break;
    case HARDEN_STATE_WAIT_DRAW:
        if (++ctx->timer >= 10) {
            G2_SetWndOutsidePlane((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3) | GX_WND_PLANEMASK_OBJ, 0);
            G2_SetWndOBJInsidePlane((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3) | GX_WND_PLANEMASK_OBJ, 0);
            ManagedSprite_SetDrawFlag(ctx->attackerSprite1, 0);
            ManagedSprite_SetDrawFlag(ctx->attackerSprite2, 0);
            ctx->timer = 0;
            ctx->state++;
        }
        break;
    default:
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    ManagedSprite_TickFrame(ctx->attackerSprite1);
    ManagedSprite_TickFrame(ctx->attackerSprite2);
    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
}

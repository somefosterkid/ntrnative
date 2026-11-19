#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_spite_context.h"

BOOL ov07_0221FAC8(BattleAnimSystem *system, int battler);

int ov07_0221FAE8(BattleAnimSystem *system);

enum GFBgLayer ov07_0221FAEC(BattleAnimSystem *system, int bg);

int ov07_0221FB04(BattleAnimSystem *system, int bg);

enum SpiteState {
    SPITE_STATE_INIT = 0,
    SPITE_STATE_SCROLL_AND_FADE,
    SPITE_STATE_CLEANUP,

    SPITE_SUB_STATE_FADE_IN = 0,
    SPITE_SUB_STATE_SCROLL,
    SPITE_SUB_STATE_FADE_OUT,
    SPITE_SUB_STATE_FINISH,
};

BOOL ov07_0222CFCC(SpiteContext *ctx);

void ov07_0222D050(SysTask *task, void *param);

void ov07_0222D050(SysTask *task, void *param) {
    SpiteContext *ctx = param;

    switch (ctx->state) {
    case SPITE_STATE_INIT:
        ov07_0222CFCC(ctx);

        if (ManagedSprite_GetDrawFlag(ctx->monSprite) == 1) {
            if (ov07_0221FAC8(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)) == 0) {
                Pokepic_SetAttr(ctx->defenderSprite, POKEPIC_VANISHED, 1);
            }
        }

        ctx->state++;
        break;
    case SPITE_STATE_SCROLL_AND_FADE:
        if (ov07_0222CFCC(ctx)) {
            ctx->state++;
        }
        break;
    case SPITE_STATE_CLEANUP:
        SetBgPriority(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            ov07_0221FB04(ctx->battleAnimSys, 1));
        SetBgPriority(GF_BG_LYR_MAIN_0, ov07_0221FAE8(ctx->battleAnimSys));

        if (ov07_0221FAC8(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)) == 0) {
            Pokepic_SetAttr(ctx->defenderSprite, POKEPIC_VANISHED, 0);
        }

        BG_ClearCharDataRange(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            0x4000,
            0,
            ov07_0221BFD0(ctx->battleAnimSys));
        BgClearTilemapBufferAndCommit(
            ov07_0221C4A0(ctx->battleAnimSys),
            ov07_0221FAEC(ctx->battleAnimSys, 1));

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_bg_scroll_wave_context.h"

u32 ov07_0221FAF8(BattleAnimSystem *system, int bg);

u32 ov07_02222D88(s32 x, s32 y);

void ov07_02227248(SysTask *task, void *param) {
    BgScrollWaveContext *ctx = param;
    s32 v1;
    s32 v2;
    s32 v3;

    switch (ctx->common.state) {
    case 0:
        ctx->counter = 0;
        v2 = ov07_0221FAF8(ctx->common.battleAnimSys, 2);
        v3 = ov07_02222D88(0, 0);
        ctx->bgScroll = ov07_02222CCC(0, 0xA0, 0x155, 0x20000, 0xC8, v2, 0, v3, ov07_0221BFD0(ctx->common.battleAnimSys));
        ctx->common.state++;
        return;
    case 1:
        v1 = ctx->counter + 1;
        ctx->counter = v1;
        if (v1 < ctx->frames) {
            return;
        }
        ov07_02222D3C(ctx->bgScroll);
        ctx->common.state++;
    default:
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }
}

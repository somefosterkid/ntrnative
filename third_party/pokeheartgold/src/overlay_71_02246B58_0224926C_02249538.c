#include "global.h"
struct TradeBallFlybyPhase;

#include "screen_fade.h"
#include "sys_task_api.h"
#include "unk_02026E30.h"
#include "overlay_71_trade_ball_flyby_phase.h"

s32 ov71_022474CC(void *);

void ov71_02249538(struct TradeBallFlybyPhase *arg0);

void ov71_02249538(TradeBallFlybyPhase *arg0) {
    if (*((s32 *)&arg0->scene) != 0) {
        G3X_Reset();
        ov71_022474CC(*((void **)&arg0->scene));
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

s32 ov71_02249670(void *arg0);

s32 ov71_022497E0(void *);

s32 ov71_022498F0(void *);

void ov71_02249C98(SysTask *task, void *data);

s32 ov71_02249558(struct TradeBallFlybyPhase *arg0);

s32 ov71_02249558(TradeBallFlybyPhase *arg0) {
    ov71_02249670(arg0);
    ov71_022497E0(arg0);
    ov71_022498F0(arg0);
    *((s32 *)&arg0->screenSwapPending) = 0;
    SysTask_CreateOnVWaitQueue(ov71_02249C98, arg0, 0U);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 3, 1, HEAP_ID_57);
    return 1;
}

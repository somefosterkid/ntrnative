#include "global.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "overlay_71_trade_tube_phase.h"

struct BgConfig;

struct SysTask;

struct SysTaskQueue;

struct TradeTubePhase;

typedef struct TubeParticleScrollState {
    SysTask **taskHandle;
    TradeTubePhase *phase;
    BOOL spriteActive[20];
    VecFx32 spritePos[20];
    int activateTimer;
    int nextSpriteToActivate;
} TubeParticleScrollState;

typedef struct TubeBgScrollState {
    TradeTubePhase *phase;
    TubeParticleScrollState particleState;
    BgConfig *bgConfig;
    fx32 mainBgStartOffset;
    fx32 subBgStartOffset;
    fx32 scrollAccel;
    fx32 totalScrolled;
    SysTask **taskHandle;
    SysTask *particleTask;
} TubeBgScrollState;

void ov71_02248BBC(TradeTubePhase *ttPhase, SysTask **task, TubeParticleScrollState *tpsState);

void ov71_02248D80(SysTask *unused, void *param);

void ov71_02248D80(SysTask *unused, void *param) {
    TubeBgScrollState *tbsState = param;

    if (tbsState->totalScrolled < (512 << 12)) {
        tbsState->scrollAccel += 288;
        tbsState->totalScrolled += tbsState->scrollAccel;

        BgSetPosTextAndCommit(tbsState->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, (tbsState->mainBgStartOffset - tbsState->totalScrolled) >> 12);
        BgSetPosTextAndCommit(tbsState->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, (tbsState->subBgStartOffset - tbsState->totalScrolled) >> 12);

        if (tbsState->totalScrolled >= (128 << 12)) {
            if (tbsState->particleTask == 0L) {
                ov71_02248BBC(tbsState->phase, &(tbsState->particleTask), &(tbsState->particleState));
            }
        }
    }
}

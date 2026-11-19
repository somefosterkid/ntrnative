#include "global.h"

#include "sys_task.h"
#include "unk_02005D10.h"
#include "heap.h"
#include "math_util.h"
#include "sys_task_api.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_send_phase.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_trade_tube_phase.h"

struct Trade3DModel;

struct TradeSendPhase;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);

typedef struct BallBounceState {
    TradeSendPhase *phase;
    Trade3DModel *model;
    VecFx32 position;
    VecFx16 rotation;
    fx32 velocityY;
    int bounceCount;
    BOOL settled;
    int settleTimer;
    fx16 spinSpeed;
} BallBounceState;

void ov71_022481EC(TradeSendPhase *tsPhase);

void ov71_0224820C(SysTask *task, void *param);

struct BgConfig *ov71_02247384(TradeSequenceData *arg0);
void ov71_02248B60(SysTask *task, void *data);
TradeTubePhase *ov71_022482EC(TradeSequenceData *arg0);
void ov71_0224889C(TradeTubePhase *ttPhase);
void ov71_022489F8(TradeTubePhase *ttPhase);
void ov71_02248B24(TradeTubePhase *ttPhase);
s32 ov71_02248E04();
void ov71_02248358(TradeTubePhase *ptr);

void ov71_0224820C(SysTask *task, void *param) {
    BallBounceState *bbState = param;

    if (bbState->settled == 0) {
        bbState->position.y += bbState->velocityY;

        if (bbState->velocityY >= 0) {
            bbState->velocityY += -1920;
        } else {
            bbState->velocityY += -1920;

            if (bbState->position.y <= -16384) {
                bbState->position.y = -16384;
                bbState->velocityY = -((bbState->velocityY * 44) / 100);

                if (bbState->velocityY < 4000) {
                    bbState->settled = 1;
                }

                PlaySE(1510);
                bbState->bounceCount++;

                switch (bbState->bounceCount) {
                case 1:
                    bbState->spinSpeed = 176;
                    break;
                case 3:
                    bbState->spinSpeed += 80;
                    break;
                }
            }
        }
    }

    bbState->rotation.x += bbState->spinSpeed;
    bbState->rotation.z -= bbState->spinSpeed;

    ov71_022476EC(bbState->model, &(bbState->rotation));

    bbState->position.x += (bbState->spinSpeed * 5);
    bbState->position.z += (bbState->spinSpeed * 5);

    ov71_022476C4(bbState->model, &(bbState->position));

    if (bbState->settled) {
        bbState->spinSpeed -= 14;

        if (++(bbState->settleTimer) > 30) {
            ov71_022481EC(bbState->phase);
        }
    }
}

TradeTubePhase *ov71_022482EC(TradeSequenceData *arg0) {
    TradeTubePhase *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(TradeTubePhase));
    if (v0 != NULL) {
        v0->sequenceData = arg0;
        v0->subStepCounter = 0;
        v0->bgConfig = ov71_02247384(arg0);
        v0->scene = NULL;
        v0->shimmerEffect = NULL;
        v0->bgScaleAnim = NULL;
        v0->task0 = NULL;
        v0->task1 = NULL;
        v0->rngSeed = MTRandom();
        v0->bgScaleAnimDone = FALSE;
        v0->screenSwapPending = FALSE;
        SysTask_CreateOnVWaitQueue(ov71_02248B60, v0, 0U);
    }
    return v0;
}

void ov71_02248358(TradeTubePhase *ptr) {
    SysTask *sysTask;
    u32 state;

    state = OS_DisableInterrupts();
    if (ptr != NULL) {
        ov71_0224889C(ptr);
        ov71_022489F8(ptr);
        ov71_02248B24(ptr);
        sysTask = ptr->task0;
        if (sysTask != NULL) {
            SysTask_Destroy(sysTask);
        }
        if (ptr->task1 != NULL) {
            ov71_02248E04();
        }
        SetMTRNGSeed(ptr->rngSeed);
        Heap_Free(ptr);
    }
    OS_RestoreInterrupts(state);
}

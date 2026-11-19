#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_71_trade_receive_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"

struct Pokepic;

struct PokepicManager;

struct PokepicDrawParam;

struct Trade3DModel;

struct Trade3DScene;

struct TradeSequenceData;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476B4(const Trade3DModel *tradeModel, VecFx32 *outPosition);

void ov71_022476D4(const Trade3DModel *tradeModel, VecFx16 *outRotation);

void ov71_02247704(Trade3DModel *tradeModel, BOOL enabled);

void ov71_02247708(Trade3DModel *tradeModel, int alpha);

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct BallOpenState {
    SysTask **taskHandle;
    int subState;
    void *phase;
    int timer;
    Trade3DScene *scene;
    Trade3DModel *model;
    fx32 currentAlpha;
    fx32 alphaStep;
    int alphaFrames;
    VecFx32 position;
    fx32 velocityY;
    VecFx16 rotation;
    fx16 rotationSpeed;
    fx16 rotationSpeedStep;
    fx16 targetRotationSpeed;
    int rotationTransitionFrames;
} BallOpenState;

void ov71_0224BABC(SysTask *task, void *param);

void ov71_0224B9CC(TradeReceivePhase *trPhase, SysTask **task);

void ov71_0224BA48(SysTask *arg0);
s32 ov71_022476EC(void *, void *);
void ov71_0224BA64(void *arg0);
void ov71_0224BAA0(BallOpenState *arg0, s32 arg1, s32 arg2);

void ov71_0224B9CC(TradeReceivePhase *trPhase, SysTask **task) {
    BallOpenState *boState = Heap_Alloc((enum HeapID)57, sizeof(BallOpenState));

    if (boState) {
        boState->subState = 0;
        boState->taskHandle = task;
        boState->scene = trPhase->scene;
        boState->model = trPhase->model;
        boState->phase = trPhase;
        boState->currentAlpha = 0;
        boState->alphaStep = (31 << 12) / 20;
        boState->alphaFrames = 20;
        boState->rotationSpeed = 4096;
        boState->rotationTransitionFrames = 0;

        ov71_02247708(boState->model, 0);
        ov71_02247704(boState->model, 1);
        ov71_022476B4(boState->model, &(boState->position));
        ov71_022476D4(boState->model, &(boState->rotation));

        *task = SysTask_CreateOnMainQueue(ov71_0224BABC, boState, 0);

        if (*task == 0L) {
            Heap_Free(boState);
        }
    } else {
        *task = 0L;
    }
}

void ov71_0224BA48(SysTask *task) {
    s32 **ptr;

    if (task != NULL) {
        ptr = SysTask_GetData(task);
        **ptr = 0;
        Heap_Free(ptr);
        SysTask_Destroy(task);
    }
}

void ov71_0224BA64(void *arg0) {
    s32 v0;

    if (*((s32 *)&((BallOpenState *)arg0)->rotationTransitionFrames) != 0) {
        *((s16 *)&((BallOpenState *)arg0)->rotationSpeed) = (s16)(*((s16 *)&((BallOpenState *)arg0)->rotationSpeed) + *((s16 *)&((BallOpenState *)arg0)->rotationSpeedStep));
        v0 = *((s32 *)&((BallOpenState *)arg0)->rotationTransitionFrames) - 1;
        *((s32 *)&((BallOpenState *)arg0)->rotationTransitionFrames) = v0;
        if (v0 == 0) {
            *((s16 *)&((BallOpenState *)arg0)->rotationSpeed) = (s16) *((s16 *)&((BallOpenState *)arg0)->targetRotationSpeed);
        }
    }
    *((s16 *)&((BallOpenState *)arg0)->rotation.y) = (s16)(*((s16 *)&((BallOpenState *)arg0)->rotation.y) + *((s16 *)&((BallOpenState *)arg0)->rotationSpeed));
    ov71_022476EC(*((void **)&((BallOpenState *)arg0)->model), arg0 + 0x34);
}

void ov71_0224BAA0(BallOpenState *arg0, s32 arg1, s32 arg2) {
    if (arg2 != 0) {
        *((s16 *)&arg0->rotationSpeedStep) = (s16)((s32)(arg1 - *((s16 *)&arg0->rotationSpeed)) / arg2);
        *((s32 *)&arg0->rotationTransitionFrames) = arg2;
    }
}

#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "sys_task_api.h"

struct BgScaleAnimation;

struct PaletteShimmerEffect;

struct Trade3DModel;

struct Trade3DScene;

struct TradeSequenceData;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

typedef struct TradeSequenceData TradeSequenceData;

typedef struct PaletteShimmerEffect PaletteShimmerEffect;

typedef struct BgScaleAnimation BgScaleAnimation;

typedef struct SpriteAnimResources {
    void *cellBuffer;
    void *animBuffer;
    NNSG2dCellDataBank *cellBank;
    NNSG2dAnimBankData *animBank;
} SpriteAnimResources;

typedef struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
} AxisRotAnimator;

typedef struct BallPathState {
    Trade3DModel *model;
    Trade3DScene *scene;
    VecFx32 orbitCenter;
    VecFx32 currentPos;
    VecFx16 rotation;
    u8 pad_26[2];
    fx32 currentY;
    fx32 yStep;
    fx32 yAccel;
    fx32 unused_34;
    fx32 orbitRadiusX;
    fx32 orbitRadiusZ;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusZStep;
    fx32 targetOrbitRadiusX;
    fx32 targetOrbitRadiusZ;
    u32 orbitTransitionFrames;
    s16 angleAccum;
    s16 rotationSpeed;
    s16 targetRotationSpeed;
    s16 rotationSpeedStep;
    AxisRotAnimator axisRotAnimators[3];
    u32 rotSpeedTransitionFrames;
    BOOL exitedView;
    BOOL midpointReached;
    int entryFramesRemaining;
} BallPathState;

typedef struct TradeTubePhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    SpriteAnimResources animResources;
    Sprite *sprites[20][2];
    u32 rngSeed;
    BgConfig *bgConfig;
    PaletteShimmerEffect *shimmerEffect;
    BOOL shimmerActive;
    BgScaleAnimation *bgScaleAnim;
    volatile BOOL bgScaleAnimDone;
    Trade3DScene *scene;
    Trade3DModel *model;
    struct { struct Trade3DModel *model; struct Trade3DScene *scene; struct { fx32 x; fx32 y; fx32 z; } orbitCenter; struct { fx32 x; fx32 y; fx32 z; } currentPos; struct { fx16 x; fx16 y; fx16 z; } rotation; u8 unk_28_pad[2]; fx32 currentY; fx32 yStep; fx32 yAccel; fx32 unused_34; fx32 orbitRadiusX; fx32 orbitRadiusZ; fx32 orbitRadiusXStep; fx32 orbitRadiusZStep; fx32 targetOrbitRadiusX; fx32 targetOrbitRadiusZ; u32 orbitTransitionFrames; s16 angleAccum; s16 rotationSpeed; s16 targetRotationSpeed; s16 rotationSpeedStep; struct { fx16 currentValue; fx16 step; fx16 targetValue; u16 framesRemaining; } axisRotAnimators[3]; u32 rotSpeedTransitionFrames; BOOL exitedView; BOOL midpointReached; int entryFramesRemaining; } ballAnim;
    SysTask *task0;
    SysTask *task1;
    BOOL screenSwapPending;
} TradeTubePhase;

typedef struct TubeParticleScrollState {
    SysTask **taskHandle;
    TradeTubePhase *phase;
    BOOL spriteActive[20];
    VecFx32 spritePos[20];
    int activateTimer;
    int nextSpriteToActivate;
} TubeParticleScrollState;

void ov71_02248C24(SysTask *task, void *param);

void ov71_02248BBC(TradeTubePhase *ttPhase, SysTask **task, TubeParticleScrollState *tpsState);

void ov71_02248BBC(TradeTubePhase *ttPhase, SysTask **task, TubeParticleScrollState *tpsState) {
    tpsState->phase = ttPhase;
    tpsState->taskHandle = task;
    tpsState->activateTimer = 0;
    tpsState->nextSpriteToActivate = 0;

    for (int i = 0; i < 20; i++) {
        tpsState->spriteActive[i] = 0;
        tpsState->spritePos[i] = *(Sprite_GetMatrixPtr(ttPhase->sprites[i][0]));
    }

    *(tpsState->taskHandle) = SysTask_CreateOnMainQueue(ov71_02248C24, tpsState, 0);

    if (*(tpsState->taskHandle) == NULL) {
        Heap_Free(tpsState);
    }
}

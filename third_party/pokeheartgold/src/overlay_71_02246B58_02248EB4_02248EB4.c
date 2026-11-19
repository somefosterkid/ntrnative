#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sys_task_api.h"

struct BgScaleAnimation;

struct PaletteShimmerEffect;

struct Trade3DModel;

struct Trade3DScene;

struct TradeSequenceData;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476B4(const Trade3DModel *tradeModel, VecFx32 *outPosition);

void ov71_022476D4(const Trade3DModel *tradeModel, VecFx16 *outRotation);

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
    BallPathState ballAnim;
    SysTask *task0;
    SysTask *task1;
    BOOL screenSwapPending;
} TradeTubePhase;

void ov71_02248F8C(BallPathState *bpState, s16 targetSpeed, int duration);

void ov71_02248FC8(BallPathState *bpState, fx32 targetRadiusX, fx32 targetRadiusZ, int duration);

void ov71_02249184(SysTask *task, void *param);

SysTask *ov71_02248EB4(TradeTubePhase *ttPhase);

SysTask *ov71_02248EB4(TradeTubePhase *ttPhase) {
    BallPathState *bpState = &(ttPhase->ballAnim);

    ov71_022476B4(ttPhase->model, &bpState->orbitCenter);
    ov71_022476D4(ttPhase->model, &bpState->rotation);

    bpState->currentPos = bpState->orbitCenter;
    bpState->scene = ttPhase->scene;
    bpState->model = ttPhase->model;
    bpState->currentY = bpState->orbitCenter.y;
    bpState->yStep = ((-8 << 12) - bpState->orbitCenter.y) / 30;
    bpState->entryFramesRemaining = 30;
    bpState->orbitRadiusX = 0;
    bpState->orbitRadiusZ = 0;
    bpState->orbitRadiusXStep = 0;
    bpState->orbitRadiusZStep = 0;
    bpState->angleAccum = 0;
    bpState->rotationSpeed = 16384;
    bpState->rotationSpeedStep = 0;
    bpState->midpointReached = 0;

    for (int i = 0; i < 3; i++) {
        bpState->axisRotAnimators[i].currentValue = 0;
        bpState->axisRotAnimators[i].step = 0;
        bpState->axisRotAnimators[i].targetValue = 0;
        bpState->axisRotAnimators[i].framesRemaining = 0;
    }

    bpState->exitedView = 0;

    ov71_02248F8C(bpState, 3328, 30);
    ov71_02248FC8(bpState, 167936, 81920, 30);

    return SysTask_CreateOnMainQueue(ov71_02249184, bpState, 0);
}

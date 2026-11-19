#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "heap.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "g2d_renderer.h"

typedef struct UnkStruct_ov71_02248D0C_Heap_Alloc {
    void *unk_0;
    u8 unk_4[0x150];
    BgConfig *unk_154;
    s32 unk_158;
    s32 unk_15C;
    s32 unk_160;
    s32 unk_164;
    SysTask **unk_168;
    s32 unk_16C;
} UnkStruct_ov71_02248D0C_Heap_Alloc;





typedef struct Trade3DModel {
    void *modelData;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
    BOOL enabled;
    VecFx32 position;
    VecFx32 scale;
    VecFx16 rotation;
    u8 pad_86[2];
    int alpha;
} Trade3DModel;

typedef struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
} Trade3DScene;

typedef u16 charcode_t;

typedef struct TrainerInfo {
    charcode_t name[7 + 1];
    u32 id;
    u32 money;
    u8 gender;
    u8 language;
    u8 badgeMask;
    u8 appearance;
    u8 gameCode;
    u8 isMainStoryCleared : 1;
    u8 hasNationalDex : 1;
    u8 : 6;
} TrainerInfo;

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};

typedef struct TradeAnimationTemplate {
    const BoxPokemon *sendingPokemon;
    const BoxPokemon *receivingPokemon;
    const TrainerInfo *otherTrainer;
    enum TradeBackground background;
    enum TradeType tradeType;
    const Options *options;
} TradeAnimationTemplate;

typedef struct TradeSequenceData {
    const TradeAnimationTemplate *animationTemplate;
    int currentPhase;
    BgConfig *bgConfig;
    MessageFormat *strTemplate;
    MsgData *msgLoader;
    String *strBuffer;
    SpriteList *spriteList;
    G2dRenderer renderer;
    SysTask *task;
    BOOL flipSprite;
    u16 sendingSpecies;
    u16 sendingForm;
    u16 receivingSpecies;
    u16 receivingForm;
    void *currentPhaseData;
} TradeSequenceData;

typedef struct BgScaleAnimation {
    BgConfig *bgConfig;
    volatile BOOL *done;
    fx32 currentScale;
    fx32 targetScale;
    fx32 rate;
    fx32 acceleration;
    int direction;
    int centerIndex;
    SysTask *task;
} BgScaleAnimation;

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

void ov71_02248C24(SysTask *unused, void *param);

void ov71_02248D80(SysTask *task, void *data);
void ov71_02248D0C(TradeTubePhase *arg0, SysTask **arg1);

void ov71_02248C24(SysTask *unused, void *param) {
    TubeParticleScrollState *tpsState = param;
    VecFx32 spritePos;

    if (++(tpsState->activateTimer) > 12) {
        tpsState->activateTimer = 0;

        if (tpsState->nextSpriteToActivate < 20) {
            tpsState->spriteActive[tpsState->nextSpriteToActivate] = 1;
            tpsState->nextSpriteToActivate++;
        }
    }

    for (int i = 0; i < 20; i++) {
        tpsState->spritePos[i].y += 135168;

        if (tpsState->spritePos[i].y >= (424 << 12)) {
            tpsState->spritePos[i].y = -114688 + (tpsState->spritePos[i].y - 1736704);

            if (tpsState->spriteActive[i]) {
                Sprite_SetDrawFlag(tpsState->phase->sprites[i][0], 1);
                Sprite_SetDrawFlag(tpsState->phase->sprites[i][1], 1);
            }
        }

        Sprite_SetMatrix(tpsState->phase->sprites[i][0], &(tpsState->spritePos[i]));

        spritePos = tpsState->spritePos[i];
        spritePos.y += 229376;

        Sprite_SetMatrix(tpsState->phase->sprites[i][1], &spritePos);
    }
}

void ov71_02248CF4(SysTask *task);

void ov71_02248CF4(SysTask *task) {
    if (task) {
        TubeParticleScrollState *tpsState = SysTask_GetData(task);

        *(tpsState->taskHandle) = NULL;
        SysTask_Destroy(task);
    }
}

void ov71_02248D0C(TradeTubePhase *arg0, SysTask **arg1) {
    SysTask *sysTask;
    UnkStruct_ov71_02248D0C_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_02248D0C_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg0;
        v0->unk_154 = (BgConfig *)arg0->bgConfig;
        v0->unk_158 = (s32)(Bg_GetYpos(v0->unk_154, GF_BG_LYR_SUB_1_F) << 0xC);
        v0->unk_15C = (s32)(Bg_GetYpos(v0->unk_154, GF_BG_LYR_SUB_2) << 0xC);
        v0->unk_160 = 0;
        v0->unk_164 = 0;
        v0->unk_168 = arg1;
        v0->unk_16C = 0;
        sysTask = SysTask_CreateOnVBlankQueue(ov71_02248D80, v0, 0U);
        *arg1 = sysTask;
        if (sysTask == NULL) {
            Heap_Free(v0);
        }
    }
}

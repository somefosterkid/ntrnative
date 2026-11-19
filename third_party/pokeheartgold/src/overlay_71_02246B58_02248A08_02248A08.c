#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "math_util.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "msgdata.h"
#include "g2d_renderer.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct StringTemplate StringTemplate;

typedef struct TrainerInfo TrainerInfo;

typedef struct TradeAnimationTemplate TradeAnimationTemplate;

typedef struct BgScaleAnimation BgScaleAnimation;

typedef struct TradeSequenceData TradeSequenceData;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

typedef struct SpriteAnimResources SpriteAnimResources;

typedef struct AxisRotAnimator AxisRotAnimator;

typedef struct BallPathState BallPathState;

typedef struct TradeTubePhase TradeTubePhase;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct StringTemplate {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
};

struct TrainerInfo {
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
};

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

struct TradeAnimationTemplate {
    const BoxPokemon *sendingPokemon;
    const BoxPokemon *receivingPokemon;
    const TrainerInfo *otherTrainer;
    enum TradeBackground background;
    enum TradeType tradeType;
    const Options *options;
};

struct BgScaleAnimation {
    BgConfig *bgConfig;
    volatile BOOL *done;
    fx32 currentScale;
    fx32 targetScale;
    fx32 rate;
    fx32 acceleration;
    int direction;
    int centerIndex;
    SysTask *task;
};

struct TradeSequenceData {
    const TradeAnimationTemplate *animationTemplate;
    int currentPhase;
    BgConfig *bgConfig;
    StringTemplate *strTemplate;
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
};

struct Trade3DModel {
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
};

struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
};

struct SpriteAnimResources {
    void *cellBuffer;
    void *animBuffer;
    NNSG2dCellDataBank *cellBank;
    NNSG2dAnimBankData *animBank;
};

struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
};

struct BallPathState {
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
};

struct TradeTubePhase {
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
};

s32 ov71_022472C4(SpriteAnimResources *, s32, s32, s32);

s32 ov71_02247320(s32 *, SpriteAnimResources *, NNSG2dImageProxy *, NNSG2dImagePaletteProxy *, s32);

Sprite *ov71_02247340(TradeSequenceData *, s32 *, s32, s32, s32, s32);

void ov71_02248A08(TradeTubePhase *ttPhase);

void ov71_02248A08(TradeTubePhase *ttPhase) {
    NNSG2dImagePaletteProxy sp60;
    NNSG2dImageProxy sp3C;
    s32 sp18[9];
    s32 sp14;
    TradeTubePhase *var_r4;
    s32 temp_r0;
    s32 temp_r5;
    s32 temp_r6;

    ov71_022472C4(&ttPhase->animResources, 0x59, 0xB, 0xC);
    NNS_G2dInitImagePaletteProxy(&sp60);
    NNS_G2dInitImageProxy(&sp3C);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 0xD, 1, GF_BG_LYR_MAIN_FIRST, 0U, NNS_G2D_VRAM_TYPE_2DMAIN, 0U, HEAP_ID_57, &sp3C);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 0xD, 1, GF_BG_LYR_MAIN_FIRST, 0U, NNS_G2D_VRAM_TYPE_2DSUB, 0U, HEAP_ID_57, &sp3C);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 0xE, NNS_G2D_VRAM_TYPE_2DMAIN, 0U, HEAP_ID_57, &sp60);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 0xE, NNS_G2D_VRAM_TYPE_2DSUB, 0U, HEAP_ID_57, &sp60);
    ov71_02247320(sp18, &ttPhase->animResources, &sp3C, &sp60, 1);
    SetMTRNGSeed(0x035947D1U);
    sp14 = 0;
    var_r4 = ttPhase;
    do {
        temp_r6 = (MTRandom() % 232U) + 0xC;
        temp_r5 = (MTRandom() % 452U) - 0x1C;
        var_r4->sprites[0][0] = ov71_02247340(ttPhase->sequenceData, sp18, temp_r6, temp_r5, 0, 1);
        var_r4->sprites[0][1] = ov71_02247340(ttPhase->sequenceData, sp18, temp_r6, temp_r5 + 0x38, 0, 1);
        Sprite_SetAnimCtrlSeq(var_r4->sprites[0][0], 0);
        Sprite_SetAnimCtrlSeq(var_r4->sprites[0][1], 1);
        Sprite_SetDrawFlag(var_r4->sprites[0][0], 0);
        Sprite_SetDrawFlag(var_r4->sprites[0][1], 0);
        var_r4 = (void *)((u8 *)var_r4 + (8));
        temp_r0 = sp14 + 1;
        sp14 = temp_r0;
    } while (temp_r0 < 0x14);
}

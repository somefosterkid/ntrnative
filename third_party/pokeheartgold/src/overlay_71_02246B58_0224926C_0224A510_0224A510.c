#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "msgdata.h"
#include "g2d_renderer.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct StringTemplate StringTemplate;

typedef struct TrainerInfo TrainerInfo;

typedef struct TradeAnimationTemplate TradeAnimationTemplate;

typedef struct TradeSequenceData TradeSequenceData;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

typedef struct SpriteAnimResources SpriteAnimResources;

typedef struct BallArrivalPathState BallArrivalPathState;

typedef struct TradeBallArrivalPhase TradeBallArrivalPhase;

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

struct BallArrivalPathState {
    void *arrivalPhase;
    SysTask *task;
    Trade3DScene *scene;
    Trade3DModel *model;
    VecFx32 orbitCenter;
    VecFx32 orbitOffset;
    VecFx32 modelPos;
    VecFx16 rotation;
    fx32 orbitCenterStepY;
    fx32 orbitCenterStepZ;
    fx32 orbitRadiusX;
    fx32 orbitRadiusY;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusYStep;
    fx32 targetOrbitRadiusX;
    fx32 targetOrbitRadiusY;
    fx32 scale;
    fx32 scaleStep;
    fx32 targetScale;
    int transitionFrames;
    int subState;
    int exitTimer;
    int frameCount;
    fx16 rotationAngle;
    fx16 rotationSpeed;
};

struct TradeBallArrivalPhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int unused_08;
    BOOL screenSwapPending;
    SpriteAnimResources animResources;
    Sprite *sprite;
    BgConfig *bgConfig;
    Trade3DScene *scene;
    Trade3DModel *model;
    SysTask *task;
    BallArrivalPathState bapState;
};

s32 ov71_022472C4(SpriteAnimResources *, s32, s32, s32);

s32 ov71_02247320(s32 *, SpriteAnimResources *, NNSG2dImageProxy *, NNSG2dImagePaletteProxy *, s32);

Sprite *ov71_02247340(TradeSequenceData *, s32 *, s32, s32, s32, s32);

void ov71_0224A510(TradeBallArrivalPhase *arrivalPhase);

void ov71_0224A510(TradeBallArrivalPhase *arrivalPhase) {
    NNSG2dImagePaletteProxy sp5C;
    NNSG2dImageProxy sp38;
    s32 sp14[9];
    Sprite *temp_r0;

    ov71_022472C4(&arrivalPhase->animResources, 0x59, 7, 8);
    NNS_G2dInitImagePaletteProxy(&sp5C);
    NNS_G2dInitImageProxy(&sp38);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, 1, GF_BG_LYR_MAIN_FIRST, 0U, NNS_G2D_VRAM_TYPE_2DMAIN, 0U, HEAP_ID_57, &sp38);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 0xA, NNS_G2D_VRAM_TYPE_2DMAIN, 0U, HEAP_ID_57, &sp5C);
    ov71_02247320(sp14, &arrivalPhase->animResources, &sp38, &sp5C, 1);
    temp_r0 = ov71_02247340(arrivalPhase->sequenceData, sp14, 0x80, 0x60, 0, 1);
    arrivalPhase->sprite = temp_r0;
    Sprite_SetDrawFlag(temp_r0, 0);
}

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

typedef struct PokemonSpriteTemplate PokemonSpriteTemplate;

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct PokemonSprite PokemonSprite;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef struct PokemonSpriteShadow PokemonSpriteShadow;

typedef struct PokemonSpriteManager PokemonSpriteManager;

typedef struct TradeAnimationTemplate TradeAnimationTemplate;

typedef struct TradeSequenceData TradeSequenceData;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

typedef struct SpriteAnimResources SpriteAnimResources;

typedef struct TradeReceivePhase TradeReceivePhase;

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

struct PokemonSpriteTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
};

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
};

struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokemonSpriteTemplate template;
    PokemonSpriteTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};

struct PokemonSpriteManager {
    PokemonSprite sprites[4];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy plttProxy;
    enum HeapID heapID;
    u32 charBaseAddr;
    u32 charSize;
    u32 plttBaseAddr;
    u32 plttSize;
    u8 *charRawData;
    u16 *plttRawData;
    u16 *plttRawDataUnfaded;
    NNSG2dCharacterData charData;
    NNSG2dPaletteData plttData;
    u8 dummy330;
    u8 needLoadChar;
    u8 needLoadPltt;
    u8 excludeIdentity;
    u32 hideShadows;
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

struct TradeReceivePhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    BOOL fanfarePlaying;
    PokemonSpriteManager *spriteManager;
    PokemonSprite *pokemonSprite;
    SpriteAnimFrame animFrames[10];
    Sprite *sprites[2];
    SpriteAnimResources animResources;
    BgConfig *bgConfig;
    Window window;
    String *string0;
    String *string1;
    Trade3DScene *scene;
    Trade3DModel *model;
    SysTask *task0;
    SysTask *task1;
    NARC *pokeDataNarc;
};

s32 ov71_022472C4(SpriteAnimResources *, s32, s32, s32);

s32 ov71_02247320(s32 *, SpriteAnimResources *, NNSG2dImageProxy *, NNSG2dImagePaletteProxy *, s32);

Sprite *ov71_02247340(TradeSequenceData *, s32 *, s32, s32, s32, s32);

void ov71_0224B848(TradeReceivePhase *trPhase);

void ov71_0224B848(TradeReceivePhase *trPhase) {
    NNSG2dImagePaletteProxy sp5C;
    NNSG2dImageProxy sp38;
    s32 sp14[9];
    Sprite *temp_r0;

    ov71_022472C4(&trPhase->animResources, 0x59, 7, 8);
    NNS_G2dInitImagePaletteProxy(&sp5C);
    NNS_G2dInitImageProxy(&sp38);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, 1, GF_BG_LYR_MAIN_FIRST, 0U, NNS_G2D_VRAM_TYPE_2DMAIN, 0U, HEAP_ID_57, &sp38);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 0xA, NNS_G2D_VRAM_TYPE_2DMAIN, 0U, HEAP_ID_57, &sp5C);
    ov71_02247320(sp14, &trPhase->animResources, &sp38, &sp5C, 2);
    trPhase->sprites[0] = ov71_02247340(trPhase->sequenceData, sp14, 0x80, 0x64, 0, 1);
    temp_r0 = ov71_02247340(trPhase->sequenceData, sp14, 0x80, 0x5A, 0, 1);
    trPhase->sprites[1] = temp_r0;
    Sprite_SetPriority(temp_r0, 1U);
    Sprite_SetDrawFlag(trPhase->sprites[0], 0);
    Sprite_SetDrawFlag(trPhase->sprites[1], 0);
}

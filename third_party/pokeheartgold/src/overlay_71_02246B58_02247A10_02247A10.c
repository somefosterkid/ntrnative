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
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0203A3B0.h"
#include "msgdata.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct Pokepic Pokepic;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef struct PokemonSpriteShadow PokemonSpriteShadow;

typedef struct PokepicManager PokepicManager;

typedef struct TradeAnimationTemplate TradeAnimationTemplate;

typedef struct TradeSequenceData TradeSequenceData;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

typedef struct SpriteAnimResources SpriteAnimResources;

typedef struct TradeSendPhase TradeSendPhase;

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

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

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

struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
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

struct PokepicManager {
    Pokepic sprites[4];
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
    const PlayerProfile *otherTrainer;
    enum TradeBackground background;
    enum TradeType tradeType;
    const Options *options;
};

struct TradeSequenceData {
    const TradeAnimationTemplate *animationTemplate;
    int currentPhase;
    BgConfig *bgConfig;
    MessageFormat *strTemplate;
    MsgData *msgLoader;
    String *strBuffer;
    SpriteList *spriteList;
    struct { struct { struct { struct NNSG2dRndCoreSurface *pCurrentTargetSurface; u32 affineOverwriteMode; struct NNSG2dImageProxy *pImgProxy; struct NNSG2dImagePaletteProxy *pPltProxy; u32 base2DCharOffset; u32 baseTexAddr3D; u32 basePltAddr3D; void *pFuncOamRegister; void *pFuncOamAffineRegister; u32 flipFlag; struct NNSG2dRndCore2DMtxCache *pCurrentMtxCacheFor2D; void *pCurrentMxt; BOOL bDrawEnable; fx32 zFor3DSoftwareSprite; struct { u8 unk_0_anon[4]; u8 unk_4_anon[4]; } currentOam; u8 mtxFor3DGE[48]; } rendererCore; struct NNSG2dRenderSurface *pTargetSurfaceList; struct NNSG2dRenderSurface *pCurrentSurface; struct NNSG2dPaletteSwapTable *pPaletteSwapTbl; u32 opzHint; fx32 spriteZoffsetStep; u32 overwriteEnableFlag; u16 overwritePriority; u16 overwritePlttNo; u32 overwriteObjMode; BOOL overwriteMosaicFlag; u16 overwritePlttNoOffset; u16 pad16_; } renderer; struct { u8 unk_0_anon[40]; void *pFuncOamRegister; void *pFuncOamAffineRegister; void *pNextSurface; void *pFuncVisibilityCulling; void *pBeforeDrawCellBackFunc; void *pAfterDrawCellBackFunc; void *pBeforeDrawOamBackFunc; void *pAfterDrawOamBackFunc; } mainScreen; struct { u8 unk_0_anon[40]; void *pFuncOamRegister; void *pFuncOamAffineRegister; void *pNextSurface; void *pFuncVisibilityCulling; void *pBeforeDrawCellBackFunc; void *pAfterDrawCellBackFunc; void *pBeforeDrawOamBackFunc; void *pAfterDrawOamBackFunc; } subScreen; } renderer;
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

struct TradeSendPhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    PokepicManager *spriteManager;
    Pokepic *pokemonSprite;
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

Trade3DScene *ov71_0224744C(u32 modelCount, fx32 targetX, fx32 targetY, fx32 targetZ);

Trade3DModel *ov71_02247610(Trade3DScene *tradeScene, int modelIndex, enum NarcId narcID, u32 memberIndex, fx32 x, fx32 y, fx32 z, BOOL enabled);

void ov71_02247ED0(void *arg0);

void ov71_02247FF8(TradeSendPhase *tsPhase);

int ov71_022473D0(TradeSequenceData *);

s32 ov71_022476EC(Trade3DModel *, const VecFx16 *);

Pokepic *ov71_02247F9C(TradeSendPhase *);

s32 sub_0203A948(s32, s32);

extern GraphicsModes ov71_0224BC74;

extern BgTemplate ov71_0224BC84;

extern BgTemplate ov71_0224BCA0;

extern GraphicsBanks ov71_0224BCBC;

s32 ov71_02247A10(TradeSendPhase *tsPhase, s32 *unused);

s32 ov71_02247A10(TradeSendPhase *tsPhase, s32 *unused) {
    unsigned int new_var;
    GfGfx_SetBanks(&ov71_0224BCBC);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    SetBothScreensModesAndDisable(&ov71_0224BC74);
    InitBgFromTemplate(tsPhase->bgConfig, 1U, &ov71_0224BC84, 0U);
    InitBgFromTemplate(tsPhase->bgConfig, 2U, &ov71_0224BCA0, 0U);
    InitBgFromTemplate(tsPhase->bgConfig, 6U, &ov71_0224BCA0, 0U);
    GfGfx_EngineATogglePlanes(1U, 1U);
    reg_G2_BG0CNT = (reg_G2_BG0CNT & (~3)) | 1;
    BG_FillCharDataRange(tsPhase->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 1U, 0U);
    FillBgTilemapRect(tsPhase->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x20U, 0U);
    LoadUserFrameGfx2(tsPhase->bgConfig, GF_BG_LYR_SUB_0_F, 0x6DU, 2U, (u8)((s32)ov71_022473D0(tsPhase->sequenceData)), HEAP_ID_57);
    AddWindowParameterized(tsPhase->bgConfig, &tsPhase->window, 1U, 2U, 0x13U, 0x1BU, 4U, 1U, 1U);
    FillWindowPixelBuffer(&tsPhase->window, 0xFU);
    GfGfxLoader_GXLoadPal(NARC_graphic_font, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, 0x20U, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 0x16, tsPhase->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0x15, tsPhase->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 0x17, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 0x16, tsPhase->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0x15, tsPhase->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_57);
    new_var = 1U;
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 0x17, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    BgCommitTilemapBufferToVram(tsPhase->bgConfig, 1U);
    ov71_02247ED0(tsPhase);
    tsPhase->scene = ov71_0224744C(new_var, 0, 0, 0);
    tsPhase->pokemonSprite = ov71_02247F9C(tsPhase);
    tsPhase->model = ov71_02247610(tsPhase->scene, 0, NARC_a_0_8_9, 0x1BU, 0, 0x1A800, 0x73800, 0);
    VecFx16 sp14 = { 0, -0x1000, 0 };
    ov71_022476EC(tsPhase->model, &sp14);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    ov71_02247FF8(tsPhase);
    reg_G2_BLDCNT = 0;
    sub_0203A880();
    sub_0203A948(1, 0x38);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 0x10, 1, HEAP_ID_57);
    return 1;
}

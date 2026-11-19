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
#include "pokepic.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "msgdata.h"
#include "g2d_renderer.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef struct PokemonSpriteShadow PokemonSpriteShadow;

typedef struct TradeAnimationTemplate TradeAnimationTemplate;

typedef struct TradeSequenceData TradeSequenceData;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

typedef struct SpriteAnimResources SpriteAnimResources;

typedef struct TradeReceivePhase TradeReceivePhase;

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

void ov71_0224B720(void *arg0);

void ov71_0224B848(TradeReceivePhase *trPhase);

int ov71_022473D0(TradeSequenceData *);

s32 ov71_022476EC(Trade3DModel *, s32 *);

Pokepic *ov71_0224B7EC(TradeReceivePhase *);

extern const VecFx16 ov71_0224BF7C;

extern GraphicsModes ov71_0224BF94;

extern BgTemplate ov71_0224BFA4;

extern BgTemplate ov71_0224BFC0;

extern GraphicsBanks ov71_0224BFDC;

s32 ov71_0224B280(TradeReceivePhase *trPhase, s32 *unused);

s32 ov71_0224B280(TradeReceivePhase *trPhase, s32 *unused) {
    Pokepic *temp_r0;
    TradeReceivePhase *new_var;
    new_var = trPhase;
    GfGfx_SetBanks(&ov71_0224BFDC);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&ov71_0224BF94);
    InitBgFromTemplate(trPhase->bgConfig, 1U, &ov71_0224BFA4, 0U);
    InitBgFromTemplate(new_var->bgConfig, 2U, &ov71_0224BFC0, 0U);
    GfGfx_EngineATogglePlanes(1U, 1U);
    reg_G2_BG0CNT = (reg_G2_BG0CNT & (~3)) | 1;
    BG_FillCharDataRange(new_var->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 1U, 0U);
    FillBgTilemapRect(new_var->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x20U, 0U);
    LoadUserFrameGfx2(trPhase->bgConfig, GF_BG_LYR_SUB_0_F, 0x6DU, 2U, (u8)((s32)ov71_022473D0(trPhase->sequenceData)), HEAP_ID_57);
    AddWindowParameterized(trPhase->bgConfig, &trPhase->window, 1U, 2U, 0x13U, 0x1BU, 4U, 1U, 1U);
    FillWindowPixelBuffer(&new_var->window, 0xFU);
    GfGfxLoader_GXLoadPal(NARC_graphic_font, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, 0x20U, HEAP_ID_57);
    BgCommitTilemapBufferToVram(trPhase->bgConfig, 1U);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 0x16, trPhase->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0x15, trPhase->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 0x17, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    ov71_0224B720(new_var);
    new_var->scene = ov71_0224744C(1U, 0, 0, 0);
    temp_r0 = ov71_0224B7EC(trPhase);
    new_var->pokemonSprite = temp_r0;
    Pokepic_SetAttr((Pokepic *)temp_r0, 6, 1);
    new_var->model = ov71_02247610(trPhase->scene, 0, NARC_a_0_8_9, 0x1BU, 0, 0x13000, 0x78000, 0);
    VecFx16 sp14 = ov71_0224BF7C;
    ov71_022476EC(new_var->model, (s32 *)&sp14);
    ov71_0224B848(new_var);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    G2_SetBlendAlpha(1, 4, 0x10, 0);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 0x10, 1, HEAP_ID_57);
    return 1;
}

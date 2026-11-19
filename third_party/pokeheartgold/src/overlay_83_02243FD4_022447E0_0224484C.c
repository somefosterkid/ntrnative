#include "global.h"
#include "gf_gfx_planes.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BattleFactoryApp;

struct BattleFactoryAppCursorSprite;

struct BattleFactoryAppPanelSprite;

struct BattleFactoryAppPokeballSprite;

struct BgConfig;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct Pokepic;

struct PokepicManager;

struct PokemonSpriteShadow;

struct PokepicTemplate;

struct PokemonSpriteTransforms;

struct PokemonSummary;

struct SaveData;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct TextPrinterTemplate;

struct Window;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

typedef struct String String;

typedef struct SaveData SaveData;

typedef struct Party Party;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct NARC NARC;

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef struct Pokepic Pokepic;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;

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

typedef struct PokemonSpriteShadow {
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
} PokemonSpriteShadow;

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

typedef struct PokepicManager PokepicManager;

typedef struct BattleFactoryAppPanelSprite BattleFactoryAppPanelSprite;

typedef struct BattleFactoryAppPokeballSprite BattleFactoryAppPokeballSprite;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct Options Options;

typedef struct MsgData MsgData;

void ReadMsgDataIntoString(const MsgData *loader, u32 entryID, String *string, u32);

typedef struct PaletteData PaletteData;

typedef struct MessageFormat MessageFormat;

void StringExpandPlaceholders(const MessageFormat *template, String *dst, const String *fmtString);

typedef struct TextPrinterTemplate TextPrinterTemplate;

typedef BOOL (*PrinterCallback_t)(TextPrinterTemplate *, u16);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

typedef struct PokemonSummary PokemonSummary;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct ListMenuItem {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} ListMenuItem;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D ListMenu2D;

enum BattleFactoryAppState {
    STATE_FADE_IN_APP = 0,
    STATE_SETUP_APP_STRINGS,
    STATE_MON_SUMMARY,
    STATE_SELECT_INITIAL_PARTY,
    STATE_CONFIRM_PARTY_SELECTION,
    STATE_RETURN_TO_INITIAL_PARTY_SELECT,
    STATE_SELECT_MON_TO_EXCHANGE,
    STATE_CANCEL_TRADE_FROM_EXCHANGE_SELECT,
    STATE_SELECT_MON_TO_RECEIVE,
    STATE_CANCEL_TRADE_FROM_RECEIVE_SELECT,
    STATE_CHANGE_EXCHANGE_TO_RECEIVE_SELECT,
    STATE_SEND_TRADE_RESULT_COMM_MESSAGE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT_APP,
    STATE_SHOW_TRADE_OCCURRED_MESSAGE,
};

typedef struct BattleFactoryApp {
    OverlayManager *appMan;
    OverlayManager *monSummaryAppMan;
    u8 subState;
    u8 challengeType;
    u8 isOpenLevel;
    u8 isExchangeMode;
    int conveyorXOffset;
    u8 printerID;
    u8 numMonsSelected;
    u8 numPokeballs;
    u8 menuWasCancelled : 1;
    u8 summaryScreenOpened : 1;
    u8 exchangeMonSelected : 1;
    u8 tradedMon : 1;
    u8 firstTimeSetupDone : 1;
    u8 clearPartnerInfo : 1;
    u8 summaryScreenOpen : 1;
    u8 unused : 1;
    u32 wheelPaletteCounter;
    u8 partnersNumSelectedMons;
    u8 animationTimer;
    u8 exchangeSelectCursorPositions;
    u8 numReceiveSelectCursorPositions;
    struct MsgData *msgLoader;
    MsgData *strTemplate;
    MessageFormat *displayStr;
    String *fmtStr;
    struct String *menuStr[4];
    u16 unused2[8];
    struct BgConfig *bgConfig;
    Window windows[10];
    ListMenu2DTemplate menuTemplate;
    struct ListMenu2D *menu;
    struct { void *entry; u8 unk_4_anon[4]; } strList[4];
    struct PaletteData *unk_120_;
    PaletteData *unk_124;
    G3DPipelineBuffers *unk_128;
    u8 pad_12C[12];
    struct Options *unk_138;
    Options *unk_13C;
    SaveData *unk_140;
    u8 pad_144[48];
    u32 unk_174;
    u8 pad_178[4];
    u32 unk_17C;
    u8 pad_180[32];
    struct PaletteData *plttData;
    struct G3DPipelineBuffers *g3dPipeline;
    struct PokepicManager *monSpriteMan;
    struct Pokepic *monSprites[3];
    struct Options *options;
    struct SaveData *saveData;
    struct PokemonSummary *monSummary;
    struct { struct SpriteList *spriteList; G2dRenderer g2dRenderer; struct SpriteResourceCollection *resourceCollection[4]; struct SpriteResource *resources[4][1]; } spriteMan;
    u8 unk_310[80];
    struct BattleFactoryAppPokeballSprite *ballSprites[6];
    struct BattleFactoryAppCursorSprite *unk_378_;
    u32 unk_37C;
    u32 unk_380;
    u32 unk_384;
    u32 unk_388;
    u32 unk_38C;
    u32 unk_390;
    struct BattleFactoryAppCursorSprite *monCursor;
    struct BattleFactoryAppCursorSprite *menuCursor;
    struct BattleFactoryAppPanelSprite *bluePanelSprite;
    struct BattleFactoryAppPanelSprite *greenPanelSprite;
    struct Party *unk_3A4_;
    u32 unk_3A8[2];
    u32 unk_3B0;
    u8 pad_3B4[4];
    u16 *unk_3B8;
    u8 pad_3BC[4];
    struct Party *personalParty;
    struct Party *receivableParty;
    u16 selectedIndices[6];
    u16 *selectedIndicesFinal;
    int startingSlot;
    struct NARC *narc;
    u16 commPayload[60];
    u16 partnerTradedMon;
    u16 partnerMonSpecies[2];
    u16 partnenMonGenders[2];
    u8 numTradeResultMsgReceived;
    u8 unk_463;
    u32 unk_464;
    u32 unk_468;
    u32 unk_46C;
    u8 pad_470[4];
    u32 unk_474;
    u32 unk_478;
    u32 unk_47C;
    u8 pad_480[429];
    u8 partnerListingUpdateNeeded;
    u8 pad_62E[2];
    u32 unused3;
} BattleFactoryApp;

void ChangeState(BattleFactoryApp *app, int *state, enum BattleFactoryAppState newState);

void ScheduleWindowCopyToVram(Window *window);

void UpdateBackgroundPalette(BattleFactoryApp *app, enum GFBgLayer bgLayer, u8 palette, u8 width, u8 height);

void LoadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadAppStartupBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadConveyorBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadWheelBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadNoScreensBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSelectionConfirmBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

u8 ov83_0224484C(BattleFactoryApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);

u8 ov83_0224484C(BattleFactoryApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font) {
    ReadMsgDataIntoString(app->strTemplate, entryID, app->menuStr[0], xOffset);
    StringExpandPlaceholders(app->displayStr, app->fmtStr, app->menuStr[0]);

    u8 result = AddTextPrinterParameterizedWithColor(window, font, app->fmtStr, xOffset, yOffset, renderDelay, ((TextColor)(((fgColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
    ScheduleWindowCopyToVram(window);
    return result;
}

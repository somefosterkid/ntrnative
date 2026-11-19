#include "global.h"
#include "gf_gfx_planes.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BattleHallApp;

struct BattleHallAppSprite;

struct BattleHallAppSpriteManager;

struct BgConfig;

struct FontSpecialCharsContext;

struct G2dRenderer;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct PokemonSummary;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct Window;

typedef struct String String;

typedef struct SaveData SaveData;

typedef struct Options Options;

typedef struct NARC NARC;

typedef struct PaletteData PaletteData;

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

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct MessageFormat MessageFormat;

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

typedef struct BattleHallAppSpriteManager {
    u8 unk_0[4];
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *resources[2][4];
} BattleHallAppSpriteManager;

typedef struct BattleHallAppSprite BattleHallAppSprite;

void ov82_0223FCB0(BattleHallAppSprite *sprite, BOOL draw);

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

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

typedef struct MsgData MsgData;

typedef struct PokemonSummary PokemonSummary;

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

void ClearFrameAndWindow2(Window *window, u8 skipTransfer);

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};

typedef struct BattleHallApp {
    OverlayManager *unused;
    OverlayManager *monSummaryAppMan;
    u8 subState;
    u8 challengeType;
    u8 printerID;
    u8 summaryScreenOpened;
    u8 prevCursorPos;
    u8 cursorPos;
    u8 unused2;
    u8 yesNoMenuOpen;
    u16 summaryScreenOpen;
    u16 isSynced;
    u16 openMenuDelay;
    u8 numSelectionMsgsReceived;
    u8 startMultiSelectionConfirm;
    u8 selectionID;
    u8 selectionCanceled;
    u8 selectedCursorPos;
    u8 fadeOutDelay;
    u16 currentStreak;
    u8 matronBattleAvailable;
    u8 savedCursorPos;
    MsgData *msgLoader;
    MessageFormat *strTemplate;
    String *displayStr;
    String *fmtStr;
    String *yesNoStrs[2];
    u16 unused3[8];
    BgConfig *bgConfig;
    Window windows[4];
    u32 unk_8C;
    u8 pad_90[4];
    struct PaletteData *plttData;
    struct FontSpecialCharsContext *specialChars;
    Options *options;
    PokemonSummary *saveData;
    struct PokemonSummary *monSummary;
    struct { struct SpriteList *spriteList; G2dRenderer renderer; struct SpriteResourceCollection *resourceCollection[4]; struct SpriteResource *resources[4][2]; } spriteMan;
    BattleHallAppSprite *cursorSprite;
    BattleHallAppSprite *monSprite;
    int monIndex;
    u16 *selectedCellPtr;
    Party *party;
    u8 *typeRanks;
    u16 *partnersLevel;
    NARC *narc;
    u16 commPayload[44];
    u8 partnersSelectedCell;
    u8 partnersDecision;
    u8 unused4;
    u8 pad_27F[1];
    u32 unused5;
    u8 pad_284[24];
} BattleHallApp;

void ov82_0223F834(BattleHallApp *app);

void LoadMainScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void BattleArcadeGameBoard_SetState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

BOOL ov82_0223E888(BattleHallApp *app);

BOOL ov82_0223E888(BattleHallApp *app) {
    switch (app->subState) {
    case 0:
        ov82_0223F834(app);
        ov82_0223FCB0(app->monSprite, 1);
        ClearFrameAndWindow2(&app->windows[0], 0);

        app->subState++;
        break;
    case 1:
        return 1;
    }

    return 0;
}

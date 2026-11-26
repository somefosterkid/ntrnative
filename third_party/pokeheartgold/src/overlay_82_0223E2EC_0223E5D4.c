#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "font.h"

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

typedef struct SaveData SaveData;

typedef struct Options Options;

int Options_GetFrame(const Options *options);

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

void BattleFrontier_SetPartnerInStrTemplate(MessageFormat *template, u32 idx);

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

void BattleHallAppSprite_SetDrawFlag(BattleHallAppSprite *sprite);

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

void BattleHallApp_DrawMessageBox(Window *window, int frame);

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

typedef struct MsgData MsgData;

typedef struct PokemonSummary PokemonSummary;

u16 sub_0203769C(void);

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

u32 ov82_0223FE18(ListMenu2D *menu);

BOOL PlaySE(u16 seqID);

void StopSE(u16 seqID, int fadeOutFrames);

void sub_020379A0(enum HeapID heapID);

void sub_02037AC0(u8 syncNo);

BOOL sub_02037B38(u8 syncState);

void sub_02037BEC(void);

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};

enum BattleHallAppMultiConfirmSubState {
    MCONFIRM_STATE_SEND_COMM,
    MCONFIRM_STATE_WAIT_FOR_PARTNER_ACK,
    MCONFIRM_STATE_OPEN_YES_NO,
    MCONFIRM_STATE_PARTNER_IS_CHOOSING,
    MCONFIRM_STATE_HANDLE_PARTNERS_DECISION,
    MCONFIRM_STATE_PROCESS_YES_NO,
    MCONFIRM_STATE_SEND_CONFIRM_COMM,
    MCONFIRM_STATE_SEND_CANCELED_COMM,
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
    ListMenu2D *yesNoMenu;
    u8 unk_90[12];
    Options *options;
    PokemonSummary *monSummary;
    BattleHallAppSpriteManager spriteMan;
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
    u32 unused5;
} BattleHallApp;


void ov82_0223F90C(BattleHallApp *app);

void ov82_0223F5E0(BgConfig *bgConfig, u8 boxIdx, u8 background);

void ov82_0223F8E4(BattleHallApp *app);

void DrawMessageWithYesNoMenu(BattleHallApp *app);

void FreeYesNoMenu(BattleHallApp *app);

u8 ov82_0223F6C4(u8 cursorPos);

BOOL ov82_0223F6E8(BattleHallApp *app, u16 cmd, u16 arg);

void ov82_0223EDF0(BattleHallApp *app, enum GFBgLayer bgLayer);

void ov82_0223EE6C(BattleHallApp *app, enum GFBgLayer bgLayer);

void BattleArcadeGameBoard_SetState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

BOOL ov82_0223E5D4(BattleHallApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov82_0223FD78(Window *window, u8 frame);
#else
void ov82_0223FD78(Window *window, int frame);
#endif

void ov80_0222A7CC(MessageFormat *template, u32 idx);

u8 ov82_0223EF7C(BattleHallApp *app, int messageID, u8 fontID);

void ov82_0223F84C(BattleHallApp *arg0);

void ov82_0223F834(BattleHallApp *app);

void ov82_0223F948(s32 delay);

void ov82_0223FCB0(BattleHallAppSprite *sprite, void *arg1);

BOOL ov82_0223E5D4(BattleHallApp *app) {
    switch (app->subState) {
    case MCONFIRM_STATE_SEND_COMM:

        if (ov82_0223F6E8(app, 5, ov82_0223F6C4(app->selectedCursorPos)) == 1) {
            app->fadeOutDelay = 0;
            app->selectedCursorPos = 0;
            app->startMultiSelectionConfirm = 0;
            app->subState = MCONFIRM_STATE_WAIT_FOR_PARTNER_ACK;
        }
        break;
    case MCONFIRM_STATE_WAIT_FOR_PARTNER_ACK:
        if (app->fadeOutDelay > 0) {
            app->fadeOutDelay--;
            break;
        }

        if (app->numSelectionMsgsReceived < 2) {
            break;
        }
        app->numSelectionMsgsReceived = 0;

        if (app->selectionID < (4 * 5)) {
            if (sub_0203769C() == 0) {
                app->subState = MCONFIRM_STATE_OPEN_YES_NO;
            } else {
                app->subState = MCONFIRM_STATE_PARTNER_IS_CHOOSING;
            }
        } else {
            if (sub_0203769C() == 0) {
                app->subState = MCONFIRM_STATE_PARTNER_IS_CHOOSING;
            } else {
                app->subState = MCONFIRM_STATE_OPEN_YES_NO;
            }
        }
        break;
    case MCONFIRM_STATE_PARTNER_IS_CHOOSING:
        ov82_0223F948(0);
        app->openMenuDelay = 0;
        ov82_0223FCB0(app->monSprite, 0);
        ov82_0223FD78(&app->windows[0], Options_GetFrame(app->options));
        ov80_0222A7CC(app->strTemplate, 0);

        app->printerID = ov82_0223EF7C(app, 11, FONT_MESSAGE);
        app->fadeOutDelay = 10;

        ov82_0223F5E0(app->bgConfig, app->partnersSelectedCell, 2);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);

        app->subState = MCONFIRM_STATE_HANDLE_PARTNERS_DECISION;
        break;
    case MCONFIRM_STATE_HANDLE_PARTNERS_DECISION:
        if (app->partnersDecision == 0) {
            break;
        }

        if (app->partnersDecision == 1) {
            app->cursorPos = app->partnersSelectedCell;
            return 1;
        } else {
            ov82_0223F90C(app);
            ov82_0223F5E0(app->bgConfig, app->partnersSelectedCell, 0);

            ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);

            app->partnersSelectedCell = 0xff;
            app->partnersDecision = 0;
            app->selectionID = 0xff;
            app->selectionCanceled = 1;

            return 1;
        }
        break;
    case MCONFIRM_STATE_OPEN_YES_NO:
        ov82_0223F5E0(app->bgConfig, app->cursorPos, 1);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        ov82_0223F84C(app);
        app->subState = MCONFIRM_STATE_PROCESS_YES_NO;
        break;
    case MCONFIRM_STATE_PROCESS_YES_NO:
        switch (ov82_0223FE18(app->yesNoMenu)) {
        case 0:
            break;
        case 1:
            StopSE(1500, 0);
            PlaySE(1571);
            ov82_0223F834(app);
            app->subState = MCONFIRM_STATE_SEND_CONFIRM_COMM;
            break;
        case 2:
            ov82_0223F834(app);
            app->subState = MCONFIRM_STATE_SEND_CANCELED_COMM;
            break;
        }
        break;
    case MCONFIRM_STATE_SEND_CONFIRM_COMM:
        if (ov82_0223F6E8(app, 6, 1) == 1) {
            return 1;
        }
        break;
    case MCONFIRM_STATE_SEND_CANCELED_COMM:
        if ((s16)app->openMenuDelay > 0) {
            app->openMenuDelay = (s16)app->openMenuDelay - 1;
            ov82_0223F948(-(s16)app->openMenuDelay);
            break;
        }
        if (ov82_0223F6E8(app, 6, 2) == 1) {
            ov82_0223F8E4(app);
            app->partnersSelectedCell = 0xff;
            app->selectionID = 0xff;
            app->selectionCanceled = 1;
            return 1;
        }
        break;

    case 8:
        sub_02037BEC();
        sub_02037AC0(104);
        app->subState = 9;
        break;
    case 9:
        if (sub_02037B38(104) == 1) {
            sub_02037BEC();
            sub_020379A0(HEAP_ID_105);
            app->selectionID = 0xff;
            return 1;
        }
        break;
    }

    return 0;
}

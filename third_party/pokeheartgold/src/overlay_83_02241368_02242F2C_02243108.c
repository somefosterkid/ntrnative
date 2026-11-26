#include "global.h"
#include "battle_castle_app_sprite_manager.h"
#include "pm_string.h"
#include "save.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "bg_window.h"
#include "list_menu.h"
#include "battle_castle_save.h"
#include "string_util.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"
#include "font.h"
#include "overlay_83_battle_castle_opponent_app.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BattleCastleAppSprite;

struct BattleCastleOpponentApp;

struct BattleCastlePersistentSave;

struct BattleCastleSave;

struct BattleFrontierSave;

struct BgConfig;

struct ListMenuCursor;

struct FontSpecialCharsContext;

struct ListMenu;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct SaveData;

struct String;

struct ListMenuItem;

struct MessageFormat;

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

void *OverlayManager_GetData(OverlayManager *appMan, int *);

void ov80_0222A7CC(MessageFormat *template, u32 idx);

typedef struct BattleFrontierSave BattleFrontierSave;

void *ov83_0224753C(BattleCastleAppSprite *sprite);

enum BattleCastleOpponentAppWindow {
    OPPONENT_APP_WINDOW_HEADER = 0,
    OPPONENT_APP_WINDOW_EXIT,
    OPPONENT_APP_WINDOW_LEVELS,
    OPPONENT_APP_WINDOW_HP_BARS,
    OPPONENT_APP_WINDOW_SUMMARY_SCREEN,
    OPPONENT_APP_WINDOW_MON_OPTIONS_MENU,
    OPPONENT_APP_WINDOW_SUMMARY_MENU,
    OPPONENT_APP_WINDOW_MSG_BOX,
    OPPONENT_APP_WINDOW_MON_SELECTION_MSG_BOX,
    OPPONENT_APP_WINDOW_MON_OPTION_MSG_BOX,
    OPPONENT_APP_WINDOW_YES_NO_MENU,
    OPPONENT_APP_WINDOW_LEVEL_MENU,
    NUM_OPPONENT_APP_WINDOWS,
};

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02247944(Window *window, u8 frame);
#else
void ov83_02247944(Window *window, int frame);
#endif

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

typedef struct ListMenuCursor ListMenuCursor;

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

BOOL ov80_02237D8C(u8 challengeType);

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

BOOL ov83_022432B4(BattleCastleOpponentApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov83_022433F8(BattleCastleOpponentApp *app);
#else
BOOL ov83_022433F8(BattleCastleOpponentApp *app);
#endif

BOOL ov83_02243C88(BattleCastleOpponentApp *app);

BOOL ov83_02243D7C(BattleCastleOpponentApp *arg0);

BOOL ov83_02243DE8(BattleCastleOpponentApp *app);

void ov83_02244CD4(BattleCastleOpponentApp *arg0, s32 *arg1, enum BattleCastleAppState newState);

u8 ov83_022448AC(BattleCastleOpponentApp *param_1, int entryID, u8 font);

void ov83_02245074(BattleCastleOpponentApp *param_1);

void ov83_022459A0(BattleCastleOpponentApp *unused);

void ov83_022459AC(BattleCastleOpponentApp *app);

void LoadMainBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void SetStringTemplateNumber(BattleCastleOpponentApp *app, u32 idx, s32 num, u32 maxDigits, enum PrintingMode paddingMode);

BOOL ov83_02243108(OverlayManager *appMan, int *state);

BOOL ov83_02243108(OverlayManager *appMan, int *state) {
    BattleCastleOpponentApp *app = OverlayManager_GetData(appMan, state);

    if (app->partnerIsExiting == 1) {
        switch (*state) {
        case STATE_MAIN_FLOW:
            app->partnerIsExiting = 0;
            ov83_02245074(app);
            ov83_022459A0(app);

            if (app->arrowSprite != 0L) {
                ov83_0224753C(app->arrowSprite);
                app->arrowSpriteVisible = 0;
            }

            ov83_02247944(&app->windows[OPPONENT_APP_WINDOW_MSG_BOX], Options_GetFrame(app->options));
            ov80_0222A7CC(app->strTemplate, 0);
            app->printerID = ov83_022448AC(app, 7, FONT_MESSAGE);
            ov83_02244CD4(app, state, STATE_SYNC_BEFORE_EXIT);
            break;
        }
    } else if (app->slotID != 0xff) {
        switch (*state) {
        case STATE_MAIN_FLOW:
        case STATE_SYNC_BEFORE_EXIT:
            app->partnerIsExiting = 0;
            ov83_02245074(app);
            ov83_022459A0(app);
            ov83_02244CD4(app, state, STATE_SYNC_PURCHASE);
            break;
        }
    }

    switch (*state) {
    case STATE_FADE_IN:
        if (ov83_022432B4(app) == 1) {
            ov83_02244CD4(app, state, STATE_MAIN_FLOW);
        }
        break;
    case STATE_MAIN_FLOW:
        if (ov83_022433F8(app) == 1) {
            if (app->syncPurchaseAction == 1) {
                ov83_02244CD4(app, state, STATE_SYNC_PURCHASE);
            } else {
                if (ov80_02237D8C(app->challengeType) == 1) {
                    ov83_02244CD4(app, state, STATE_SYNC_BEFORE_EXIT);
                } else {
                    ov83_02244CD4(app, state, STATE_FADE_OUT);
                }
            }
        }
        break;
    case STATE_SYNC_PURCHASE:
        if (ov83_02243C88(app) == 1) {
            ov83_02244CD4(app, state, STATE_MAIN_FLOW);
        }
        break;
    case STATE_SYNC_BEFORE_EXIT:
        if (ov83_02243D7C(app) == 1) {
            ov83_02244CD4(app, state, STATE_FADE_OUT);
        }
        break;
    case STATE_FADE_OUT:
        if (ov83_02243DE8(app) == 1) {
            return 1;
        }
        break;
    }

    ov83_022459AC(app);
    SpriteList_RenderAndAnimateSprites(app->spriteMan.spriteList);

    return 0;
}

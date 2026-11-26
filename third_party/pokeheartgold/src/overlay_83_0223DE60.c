#include "global.h"
#include "battle_castle_app_sprite_manager.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"
#include "list_menu.h"
#include "overlay_manager.h"
#include "battle_castle_save.h"
#include "save_frontier.h"
#include "message_printer.h"
#include "font.h"
#include "string_util.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

struct OverlayManager;

struct BattleCastleAppSprite;

struct BattleCastleSelfApp;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct ListMenuItem;

struct Window;

void *OverlayManager_GetData(OverlayManager *appMan);

void ov80_0222A7CC(MessageFormat *template, u32 idx);

void *ov83_0224753C(BattleCastleAppSprite *sprite);

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

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

BOOL ov83_0223E008(BattleCastleSelfApp *app);

BOOL ov83_0223E14C(BattleCastleSelfApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov83_0223EEA0(BattleCastleSelfApp *app);
#else
BOOL ov83_0223EEA0(BattleCastleSelfApp *app);
#endif

BOOL ov83_0223EFA4(BattleCastleSelfApp *app);

BOOL ov83_0223F010(BattleCastleSelfApp *app);

u8 ov83_0223FD14(BattleCastleSelfApp *app, int entryID, u8 font);

void ov83_02240DA8(BattleCastleSelfApp *app, int *state, enum BattleCastleAppState nextState);

void ov83_022412A0(BattleCastleSelfApp *app);

void ov83_02241B30(BattleCastleSelfApp *app);

void LoadMainBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadItemSelectBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleSelfApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void SetStringTemplateNumber(BattleCastleSelfApp *app, u32 idx, s32 num, u32 maxDigits, enum PrintingMode paddingMode);

BOOL ov83_0223DE60(OverlayManager *appMan, int *state);

BOOL ov83_0223DE60(OverlayManager *appMan, int *state) {
    BattleCastleSelfApp *app = OverlayManager_GetData(appMan);

    if (app->partnerIsExiting == 1) {
        switch (*state) {
        case STATE_MAIN_FLOW:
            app->partnerIsExiting = 0;
            ov83_022412A0(app);

            if (app->monSparklesSprite != 0L) {
                ov83_0224753C(app->monSparklesSprite);
                app->monSparklesVisible = 0;
            }

            ov83_02247944(&app->windows[SELF_APP_WINDOW_MSG_BOX], Options_GetFrame(app->options));
            ov80_0222A7CC(app->strTemplate, 0);

            app->printerID = ov83_0223FD14(app, 8, FONT_MESSAGE);

            ov83_02240DA8(app, state, STATE_SYNC_BEFORE_EXIT);
            break;
        }
    } else if (app->slotID != 0xff) {
        switch (*state) {
        case STATE_MAIN_FLOW:
        case STATE_SYNC_BEFORE_EXIT:
            app->partnerIsExiting = 0;
            ov83_022412A0(app);
            ov83_02240DA8(app, state, STATE_SYNC_PURCHASE);
            break;
        }
    }

    switch (*state) {
    case STATE_FADE_IN:
        if (ov83_0223E008(app) == 1) {
            ov83_02240DA8(app, state, STATE_MAIN_FLOW);
        }
        break;
    case STATE_MAIN_FLOW:
        if (ov83_0223E14C(app) == 1) {
            if (app->syncPurchaseAction == 1) {
                ov83_02240DA8(app, state, STATE_SYNC_PURCHASE);
            } else {
                if (ov80_02237D8C(app->challengeType) == 1) {
                    ov83_02240DA8(app, state, STATE_SYNC_BEFORE_EXIT);
                } else {
                    ov83_02240DA8(app, state, STATE_FADE_OUT);
                }
            }
        }
        break;
    case STATE_SYNC_PURCHASE:
        if (ov83_0223EEA0(app) == 1) {
            ov83_02240DA8(app, state, STATE_MAIN_FLOW);
        }
        break;
    case STATE_SYNC_BEFORE_EXIT:
        if (ov83_0223EFA4(app) == 1) {
            ov83_02240DA8(app, state, STATE_FADE_OUT);
        }
        break;
    case STATE_FADE_OUT:
        if (ov83_0223F010(app) == 1) {
            return 1;
        }
        break;
    }

    ov83_02241B30(app);
    SpriteList_RenderAndAnimateSprites(app->spriteMan.spriteList);

    return 0;
}

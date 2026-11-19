#include "global.h"
#include "battle_castle_app_sprite_manager.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "battle_castle_save.h"
#include "constants/heap.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "save_frontier.h"
#include "battle_castle_app_sprite.h"
#include "bg_window.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "message_printer.h"
#include "screen_fade.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

struct BattleCastleOpponentApp;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct ListMenuItem;

struct Window;

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

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

void sub_02037AC0(u8 syncNo);

BOOL sub_02037B38(u8 syncState);

void sub_02037BEC(void);

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

void ov83_022433B8(BattleCastleOpponentApp *app);

BOOL ov83_022450A8(BattleCastleOpponentApp *app, u16 cmd, u16 slot);

void LoadMainBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

u8 ov81_0224080C(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void ChangeState(BattleCastleOpponentApp *app, int *state, enum BattleCastleAppState newState);

BOOL ov83_022432B4(BattleCastleOpponentApp *app);

BOOL ov83_022432B4(BattleCastleOpponentApp *app) {
    switch (app->subState) {
    case 0:
        if (ov80_02237D8C(app->challengeType) == 1) {
            sub_02037BEC();
            sub_02037AC0(216);
        }

        app->subState++;
        break;
    case 1:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (sub_02037B38(216) == 1) {
                sub_02037BEC();
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 2:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (ov83_022450A8(app, 20, 0) == 1) {
                app->subState++;
            }
        } else {
            ov83_022433B8(app);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 3, HEAP_ID_107);
            app->subState++;
        }
        break;
    case 3:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (app->msgsReceived >= 2) {
                app->msgsReceived = 0;

                ov83_022433B8(app);
                BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 3, HEAP_ID_107);

                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }

    return 0;
}

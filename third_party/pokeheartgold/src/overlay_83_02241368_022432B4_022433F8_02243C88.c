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

void sub_020379A0(enum HeapID heapID);

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

BOOL ov83_02244E24(BattleCastleOpponentApp *param_1, u32 selectedSlot, u32 menuOption);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov83_02244F60(BattleCastleOpponentApp *app, u8 selectedSlot, u8 menuOption);
#else
BOOL ov83_02244F60(BattleCastleOpponentApp *app, u32 selectedSlot, u8 menuOption);
#endif

void ov83_0224563C(BattleCastleOpponentApp *app, u32 slotID, u32 menuOption);

void ov83_02245ACC(BattleCastleOpponentApp *app, u8 slot, u8 menuOption);

void LoadMainBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void ChangeState(BattleCastleOpponentApp *app, int *state, enum BattleCastleAppState newState);

u32 ov83_022450A8(void *param_1, u16 param_2, u16 arg2);

u32 ov83_0224776C(u32 param_1, u32 param_2);

BOOL ov83_02243C88(BattleCastleOpponentApp *app);

BOOL ov83_02243C88(BattleCastleOpponentApp *app) {
    s32 slot;

    switch (app->subState) {
    case 0:
        app->syncPurchaseState = 1;

        if (ov83_022450A8(app, 21, app->selectedMonSlot) == 1) {
            app->syncPurchaseAction = 0;
            app->subState++;
        }
        break;
    case 1:
        if (app->slotID == 0xff) {
            break;
        }

        app->msgsReceived = 0;

        if (app->selectedMenuEntry == 5) {
            ov83_02245ACC(app, app->slotID, 5);
        } else {
            ov83_0224563C(app, app->slotID, app->selectedMenuEntry);
        }

        app->subState++;
        break;
    case 2:
        slot = ov83_0224776C(app->exitSlot, app->slotID);

        if (ov83_02244E24(app, slot, app->selectedMenuEntry) == 1) {
            app->syncDelay = 30;
            app->subState++;
        }
        break;
    case 3:
        app->syncDelay--;

        if (app->syncDelay == 0) {
            sub_02037BEC();
            sub_02037AC0(133);
            app->subState++;
            break;
        }
        break;
    case 4:
        if (sub_02037B38(133) == 1) {
            app->subState++;
        }
        break;
    case 5:
        slot = ov83_0224776C(app->exitSlot, app->slotID);

        if (ov83_02244F60(app, slot, app->selectedMenuEntry) == 1) {
            sub_02037BEC();
            sub_020379A0(HEAP_ID_107);
            app->slotID = 0xff;
            app->partnerIsExiting = 0;
            return 1;
        }
        break;
    }

    return 0;
}

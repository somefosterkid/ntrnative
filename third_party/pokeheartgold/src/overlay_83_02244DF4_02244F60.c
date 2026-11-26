#include "global.h"
#include "battle_castle_app_sprite_manager.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "list_menu.h"
#include "overlay_manager.h"
#include "battle_castle_save.h"
#include "save_frontier.h"
#include "message_printer.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

struct BallSeal;

struct BattleCastleOpponentApp;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct Party;

struct Pokemon;

struct ListMenuItem;

struct Window;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

Pokemon *Party_GetMonByIndex(const Party *party, int slot);


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

u16 sub_0203769C(void);

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

void ShowMonSummary(BattleCastleOpponentApp *app, u8 slot);

void ShowMovesList(BattleCastleOpponentApp *app, u8 slot);

void LoadMainBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void ov83_02244704(BattleCastleOpponentApp *arg0, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void ov82_0223EE6C(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void BattleArcadeGameBoard_SetState(BattleCastleOpponentApp *app, int *state, enum BattleCastleAppState newState);

u8 ov83_02247768(u8 numSlots, u8 slot, u8, s32);

BOOL ov83_02244F60(BattleCastleOpponentApp *app, u8 selectedSlot, u8 menuOption, s32 arg3);

void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);

BOOL ov83_02244F60(BattleCastleOpponentApp *app, u8 selectedSlot, u8 menuOption, s32 arg3) {
    u8 exitSlot = app->exitSlot;
    u8 slot = ov83_02247768(app->numSlots, selectedSlot, menuOption, arg3);
    Pokemon *mon = Party_GetMonByIndex(app->opponentsParty, slot);

    switch (menuOption) {
    case 0:
    case 1:
        app->arrowSpriteVisible = 0;
        return 1;
    case 5:
        if (!app->arrowSpriteVisible) {
            ov83_02246114(app, 0);
        }
        app->arrowSpriteVisible = 0;
        return 1;
    case 3:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (sub_0203769C() == 0) {
                if (app->slotID >= exitSlot) {
                    app->arrowSpriteVisible = 0;
                    return 1;
                }
            } else {
                if (app->slotID < exitSlot) {
                    app->arrowSpriteVisible = 0;
                    return 1;
                }
            }
        }

        if (!app->arrowSpriteVisible) {
            app->syncPurchaseState = 3;
            app->arrowSpriteVisible = 0;
            return 1;
        }
        break;
    case 4:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (sub_0203769C() == 0) {
                if (app->slotID >= exitSlot) {
                    app->arrowSpriteVisible = 0;
                    return 1;
                }
            } else {
                if (app->slotID < exitSlot) {
                    app->arrowSpriteVisible = 0;
                    return 1;
                }
            }
        }

        if (!app->arrowSpriteVisible) {
            app->syncPurchaseState = 3;
            app->arrowSpriteVisible = 0;
            return 1;
        }
        break;
    }

    return 0;
}

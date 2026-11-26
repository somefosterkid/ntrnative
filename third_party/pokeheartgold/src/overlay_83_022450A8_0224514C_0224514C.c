#include "global.h"

#include "battle_castle_app_sprite_manager.h"
#include "unk_02035900.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "battle_castle_save.h"
#include "save_frontier.h"
#include "message_printer.h"

struct BattleCastleOpponentApp;

struct BattleCastlePersistentSave;

struct BattleCastleSave;

struct FrontierSave;

struct ListMenuCursor;

struct MessagePrinter;

struct G2dRenderer;

struct ListMenu2D;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct Window;

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

void LoadMainBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void ov83_02244704(BattleCastleOpponentApp *arg0, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void ov82_0223EE6C(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void BattleArcadeGameBoard_SetState(BattleCastleOpponentApp *app, int *state, enum BattleCastleAppState newState);

void ov83_0224514C(int netID, int unused, void *data, void *context);

void ov83_0224514C(int netID, int unused, void *data, void *context) {
    BattleCastleOpponentApp *app = context;
    const u16 *payload = data;

    int i = 0;
    app->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    i += 2;

    for (int j = 0; j < 3; j++) {
        app->partnersRanks[j] = payload[i + j];
    }

    i += 3;
}

#include "global.h"
#include "overlay_81_battle_factory_app.h"
#include "seal_case.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "bg_window.h"
#include "options.h"
#include "palette.h"
#include "pokemon_summary_app.h"
#include "overlay_manager.h"
#include "pokepic.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "gf_gfx_planes.h"

struct BallSeal;

struct BattleFactoryApp;

struct G3DPipelineBuffers;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct Party;

struct Pokemon;

struct Pokepic;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct ListMenuItem;

struct Window;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

u32 GetMonData(Pokemon *mon, int param, void *dest);

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

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

void BattleArcadeGameBoard_SetState(BattleFactoryApp *app, int *state, enum BattleFactoryAppState newState);

void UpdateBackgroundPalette(BattleFactoryApp *app, enum GFBgLayer bgLayer, u8 palette, u8 width, u8 height);

void LoadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadAppStartupBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadConveyorBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadWheelBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadNoScreensBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSelectionConfirmBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ov81_0224102C(BattleFactoryApp *app, u16 cmd, u16 unused);

void ov81_0224102C(BattleFactoryApp *app, u16 cmd, u16 unused) {
    int i;
    int offset = 0;

    for (i = 0; i < 60; i++) {
        app->commPayload[i] = 0;
    }

    app->commPayload[0] = app->numMonsSelected;
    offset += 1;

    for (i = 0; i < app->numMonsSelected; i++) {
        Pokemon *mon = Party_GetMonByIndex(app->personalParty, app->selectedIndices[i]);
        app->commPayload[i + offset] = GetMonData(mon, MON_DATA_SPECIES, 0L);
    }

    offset += app->numMonsSelected;

    for (i = 0; i < app->numMonsSelected; i++) {
        Pokemon *mon = Party_GetMonByIndex(app->personalParty, app->selectedIndices[i]);
        app->commPayload[i + offset] = GetMonData(mon, MON_DATA_GENDER, 0L);
    }
}

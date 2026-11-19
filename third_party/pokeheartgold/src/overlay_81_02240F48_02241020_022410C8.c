#include "global.h"

#include "unk_02035900.h"
#include "gf_gfx_planes.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokepic.h"
#include "pokemon_summary_app.h"
#include "overlay_81_battle_factory_app.h"

struct BattleFactoryApp;

struct BattleFactoryAppPanelSprite;

struct BattleFactoryAppPokeballSprite;

struct G3DPipelineBuffers;

struct ListMenu2D;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SpriteAnimFrame;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

u8 ov80_02236DD4(u8 challengeType);

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

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

void ov82_0223EE6C(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadNoScreensBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSelectionConfirmBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ov81_022410C8(int netID, int unused, void *data, void *context);

void ov81_022410C8(int netID, int unused, void *data, void *context) {
    int i;
    BattleFactoryApp *app = context;
    const u16 *payload = data;

    int offset = 0;
    ov80_02236DD4(app->challengeType);

    if (sub_0203769C() == netID) {
        return;
    }

    app->partnersNumSelectedMons = payload[0];
    offset += 1;

    for (i = 0; i < app->partnersNumSelectedMons; i++) {
        app->partnerMonSpecies[i] = payload[i + offset];
    }

    offset += app->partnersNumSelectedMons;

    for (i = 0; i < app->partnersNumSelectedMons; i++) {
        app->partnenMonGenders[i] = payload[i + offset];
    }
    app->unk_463 = TRUE;
}

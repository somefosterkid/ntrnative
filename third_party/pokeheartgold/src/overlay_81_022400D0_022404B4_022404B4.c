#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
#include "pm_string.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "options.h"
#include "palette.h"
#include "pokepic.h"
#include "pokemon_summary_app.h"
#include "list_menu_items.h"
#include "gf_gfx_planes.h"

struct BattleFactoryApp;

struct BattleFactoryAppPokeballSprite;

struct G3DPipelineBuffers;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct Pokepic;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct ListMenuItem;

struct Window;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

VecFx32 ov81_02242EC4(BattleFactoryAppPokeballSprite *ballSprite, int x, int y);

void ov81_02242F10(BattleFactoryAppPokeballSprite *ballSprite, int x, int y);

const VecFx32 *ov81_02242F30(BattleFactoryAppPokeballSprite *ballSprite);

int ov81_02242F8C(BattleFactoryAppPokeballSprite *ballSprite);

int ov81_02242F90(BattleFactoryAppPokeballSprite *ballSprite);

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

void ov81_022405F0(BattleFactoryApp *app);

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

BOOL ov81_022404B4(BattleFactoryApp *app) {
    int ballsReachedRestingPoint = 0;

    BgSetPosTextAndCommit(app->bgConfig, (enum GFBgLayer)6, BG_POS_OP_ADD_X, 8);
    app->conveyorXOffset = Bg_GetXpos(app->bgConfig, GF_BG_LYR_SUB_2);

    for (int i = 0; i < app->numPokeballs; i++) {
        const VecFx32 *ballPosition = ov81_02242F30(app->ballSprites[i]);

        if ((ballPosition->x / ((fx32)0x0000000000001000L)) - 8 <= ov81_02242F8C(app->ballSprites[i])) {
            int x = ov81_02242F8C(app->ballSprites[i]);
            int y = ov81_02242F90(app->ballSprites[i]);
            ov81_02242F10(app->ballSprites[i], x, y);
            ballsReachedRestingPoint = 1;
        } else {
            ov81_02242EC4(app->ballSprites[i], -8, 0);
        }
    }

    ov81_022405F0(app);

    if (ballsReachedRestingPoint == 1) {
        return 1;
    }

    return 0;
}

void ov81_02242EB8(BattleFactoryAppPokeballSprite *ballSprite, BOOL draw);

BOOL ov81_02240564(BattleFactoryApp *app) {
    int numBallsOffscreen = 0;

    BgSetPosTextAndCommit(app->bgConfig, (enum GFBgLayer)6, BG_POS_OP_ADD_X, 8);
    app->conveyorXOffset = Bg_GetXpos(app->bgConfig, GF_BG_LYR_SUB_2);

    for (int i = 0; i < app->numPokeballs; i++) {
        const VecFx32 *ballPosition = ov81_02242F30(app->ballSprites[i]);

        if ((ballPosition->x / ((fx32)0x0000000000001000L)) - 8 < -24) {
            ov81_02242EB8(app->ballSprites[i], 0);
            numBallsOffscreen++;
        } else {
            ov81_02242EC4(app->ballSprites[i], -8, 0);
        }
    }

    ov81_022405F0(app);

    if (numBallsOffscreen == app->numPokeballs) {
        return 1;
    }

    return 0;
}

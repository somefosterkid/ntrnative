#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
#include "gf_gfx_planes.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokepic.h"
#include "bg_window.h"
#include "options.h"
#include "msgdata.h"
#include "palette.h"
#include "pokemon_summary_app.h"
#include "list_menu_items.h"

struct BattleFactoryApp;

struct BattleFactoryAppPanelSprite;

struct BattleFactoryAppPokeballSprite;

struct G3DPipelineBuffers;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct Pokepic;

struct PokepicShadow;

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

BOOL ov80_02237254(u8 challengeType);

BOOL sub_02037030(int cmd, const void *data, int size);

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

void ov81_0224102C(BattleFactoryApp *app, u16 cmd, u16 unused);

void ChangeState(BattleFactoryApp *app, int *state, enum BattleFactoryAppState newState);

void UpdateBackgroundPalette(BattleFactoryApp *app, enum GFBgLayer bgLayer, u8 palette, u8 width, u8 height);

void LoadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadAppStartupBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadConveyorBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadWheelBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadNoScreensBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSelectionConfirmBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

BOOL ov81_02240FA4(BattleFactoryApp *app, u16 cmd, u16 arg) {
    int commCmd;

    if (!ov80_02237254(app->challengeType)) {
        return 0;
    }

    switch (cmd) {
    case 7:
        commCmd = 29;
        ov81_02241008(app, cmd);
        break;
    case 8:
        commCmd = 30;
        ov81_0224102C(app, cmd, arg);
        break;
    case 10:
        commCmd = 32;
        ov81_02241218(app, cmd, arg);
        break;
    }

    return sub_02037030(commCmd, (u8 *)&app->commPayload[0], 60) == 1;
}

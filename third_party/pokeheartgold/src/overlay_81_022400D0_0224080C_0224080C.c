#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "font_types_def.h"
#include "pokemon_summary_app.h"
#include "pokepic.h"
#include "msgdata.h"
#include "message_format.h"

struct BattleFactoryApp;

struct BattleFactoryAppPanelSprite;

struct BattleFactoryAppPokeballSprite;

struct G3DPipelineBuffers;

struct ListMenu2D;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct String;

struct Window;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

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

u8 ov81_0224080C(BattleFactoryApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font) {
    FillWindowPixelBuffer(window, bgColor);
    ReadMsgDataIntoString(app->msgLoader, entryID, app->fmtStr);
    StringExpandPlaceholders(app->strTemplate, app->displayStr, app->fmtStr);

    return AddTextPrinterParameterizedWithColor(window, font, app->displayStr, xOffset, yOffset, renderDelay, ((TextColor)(((fgColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
}

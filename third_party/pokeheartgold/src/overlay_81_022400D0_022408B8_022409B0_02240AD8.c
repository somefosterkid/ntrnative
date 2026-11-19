#include "global.h"
#include "overlay_81_battle_factory_app.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokepic.h"
#include "bg_window.h"
#include "palette.h"
#include "font_types_def.h"
#include "pokemon_summary_app.h"
#include "list_menu_items.h"

struct BattleFactoryApp;

struct BattleFactoryAppPanelSprite;

struct BattleFactoryAppPokeballSprite;

struct G3DPipelineBuffers;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MsgData;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct String;

struct ListMenuItem;

struct Window;

void String_Delete(String *string);

void String_SetEmpty(String *string);

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void FillWindowPixelBuffer(Window *window, u8 fillValue);

u8 GetWindowWidth(Window *window);

typedef struct MessageBank MessageBank;

enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

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

void ov82_0223EE6C(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadMonSelectionBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ReloadNoScreensBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void LoadSelectionConfirmBackground(BattleFactoryApp *app, enum GFBgLayer bgLayer);

void ov81_02240AD8(BattleFactoryApp *app, Window *window, u32 xOffset, u32 yOffset, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, u16 species, u8 gender);

void ov81_02240AD8(BattleFactoryApp *app, Window *window, u32 xOffset, u32 yOffset, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, u16 species, u8 gender) {
    unsigned char new_var;
    FillWindowPixelBuffer(window, bgColor);
    new_var = 237;
    MsgData *loader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (enum NarcId)27, new_var, (enum HeapID)100);
    String *string = NewString_ReadMsgData(loader, species);
    DestroyMsgData(loader);
    AddTextPrinterParameterizedWithColor(window, font, string, xOffset, yOffset, 0xFF, (TextColor)((((fgColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8)) | ((bgColor & 0xFF) << 0)), 0L);
    u8 width = GetWindowWidth(window) - 1;
    u32 symbol = (gender == GENDER_MALE) ? (26) : (27);
    TextColor color = (gender == GENDER_MALE) ? ((TextColor)((((7 & 0xFF) << 16) | ((8 & 0xFF) << 8)) | ((0 & 0xFF) << 0))) : ((TextColor)((((3 & 0xFF) << 16) | ((4 & 0xFF) << 8)) | ((0 & 0xFF) << 0)));
    String_SetEmpty(string);
    if (gender != GENDER_NONE) {
        ReadMsgDataIntoString(app->msgLoader, symbol, string);
        AddTextPrinterParameterizedWithColor(window, font, string, width * 8, yOffset, 0xFF, color, 0L);
    }
    String_Delete(string);
}

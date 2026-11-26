#include "global.h"
#include "overlay_82_battle_hall_app.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "font_types_def.h"
#include "pokemon_summary_app.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BattleHallApp;

struct BattleHallAppSprite;


struct BgConfig;

struct FontSpecialCharsContext;

struct G2dRenderer;

struct Menu;

struct MenuTemplate;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct PokemonSummary;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct StringList;

struct StringTemplate;

struct TextPrinterTemplate;

struct Window;

typedef struct StringTemplate StringTemplate;

void StringExpandPlaceholders(const StringTemplate *template, String *dst, const String *fmtString);

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

typedef struct BattleHallAppSprite BattleHallAppSprite;

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

void FillWindowPixelBuffer(Window *window, u8 val);

typedef struct FontSpecialCharsContext FontSpecialCharsContext;


void ReadMsgDataIntoString(const struct MsgData *loader, u32 entryID, String *string);

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, TextPrinterCallback callback);

typedef struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} StringList;

typedef struct MenuTemplate {
    StringList *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} MenuTemplate;

typedef struct Menu Menu;

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};

void ov82_0223EDF0(BattleHallApp *app, enum BgLayer bgLayer);

void LoadSubScreenBackground(BattleHallApp *app, enum BgLayer bgLayer);

void ChangeState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

u8 ov82_0223EF1C(BattleHallApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

u8 ov82_0223EF1C(BattleHallApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID) {
    FillWindowPixelBuffer(window, bgColor);
    ReadMsgDataIntoString(app->msgLoader, messageID, app->fmtStr);
    StringExpandPlaceholders(app->strTemplate, app->displayStr, app->fmtStr);

    return AddTextPrinterParameterizedWithColor(window, fontID, app->displayStr, x, y, renderDelay, ((TextColor)(((textColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
}

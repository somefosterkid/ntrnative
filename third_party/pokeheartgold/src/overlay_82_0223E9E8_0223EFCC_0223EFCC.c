#include "global.h"
#include "overlay_82_battle_hall_app.h"
#include "seal_case.h"
#include "pm_string.h"
#include "mail_message_types.h"
#include "pokemon_types_def.h"
#include "options.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "bg_window.h"
#include "message_printer.h"
#include "msgdata.h"
#include "font_types_def.h"
#include "pokemon_summary_app.h"
#include "list_menu_items.h"
#include "gf_gfx_planes.h"

struct BallSeal;

struct BattleHallApp;

struct BattleHallAppSprite;


struct G2dRenderer;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct Party;

struct Pokemon;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct ListMenuItem;

struct Window;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef u16 charcode_t;

void String_Delete(String *string);

void CopyU16ArrayToString(String *dst, const charcode_t *src);

Pokemon *Party_GetMonByIndex(const Party *party, int slot, u32);

u32 GetMonData(Pokemon *mon, int param, void *dest);

typedef struct BattleHallAppSprite BattleHallAppSprite;

void CopyWindowToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

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

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};

void ov82_0223EDF0(BattleHallApp *app, enum GFBgLayer bgLayer);

void ov82_0223EE6C(BattleHallApp *app, enum GFBgLayer bgLayer);

void BattleArcadeGameBoard_SetState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

void ov82_0223EFCC(BattleHallApp *app, Window *window, u32 x, u32 y, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

void ov82_0223EFCC(BattleHallApp *app, Window *window, u32 x, u32 y, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID) {
    u16 name[11];

    Pokemon *mon = Party_GetMonByIndex(app->party, 0, x);
    GetMonData(mon, MON_DATA_SPECIES_NAME, name);
    FillWindowPixelBuffer(window, bgColor);

    String *strBuf = String_New(11, HEAP_ID_105);
    CopyU16ArrayToString(strBuf, name);
    AddTextPrinterParameterizedWithColor(window, fontID, strBuf, x, y, 0, ((TextColor)(((textColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
    String_Delete(strBuf);
    CopyWindowToVram(window);
}

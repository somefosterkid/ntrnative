#include "global.h"
#include "pm_string.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "bg_window.h"
#include "options.h"
#include "palette.h"
#include "font_types_def.h"
#include "pokemon_summary_app.h"
#include "overlay_83_battle_castle_self_app.h"
#include "overlay_manager.h"
#include "overlay_81_battle_factory_app.h"
#include "pokepic.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "gf_gfx_planes.h"

struct G3DPipelineBuffers;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct Pokepic;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct String;

struct ListMenuItem;

struct Window;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

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

void ScheduleWindowCopyToVram(Window *window);

u8 ov83_0223FCB4(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);

u8 ov83_0223FCB4(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font) {
    ReadMsgDataIntoString(app->msgLoader, entryID, app->fmtStr);
    StringExpandPlaceholders(app->strTemplate, app->displayStr, app->fmtStr);

    u8 result = AddTextPrinterParameterizedWithColor(window, font, app->displayStr, xOffset, yOffset, renderDelay, ((TextColor)(((fgColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
    ScheduleWindowCopyToVram(window);
    return result;
}

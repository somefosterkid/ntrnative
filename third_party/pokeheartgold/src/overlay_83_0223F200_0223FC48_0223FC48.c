#include "global.h"
#include "overlay_83_battle_castle_self_app.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "font_types_def.h"
#include "pokemon_summary_app.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "gf_gfx_planes.h"

struct ListMenu2D;

struct ListMenu2DTemplate;

struct String;

struct ListMenuItem;

struct Window;

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

void ScheduleWindowCopyToVram(Window *window);

u8 ov83_0223FC48(BattleCastleSelfApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

u8 ov83_0223FC48(BattleCastleSelfApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID) {
    FillWindowPixelBuffer(window, bgColor);
    ReadMsgDataIntoString(app->msgLoader, messageID, app->fmtStr);
    StringExpandPlaceholders(app->strTemplate, app->displayStr, app->fmtStr);

    u8 result = AddTextPrinterParameterizedWithColor(window, fontID, app->displayStr, x, y, renderDelay, ((TextColor)(((textColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
    ScheduleWindowCopyToVram(window);
    return result;
}

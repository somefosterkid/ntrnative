#include "global.h"
#include "bg_window.h"
#include "pm_string.h"
#include "options.h"
#include "sprite.h"
#include "sprite_system.h"
#include "font_types_def.h"
#include "pokemon_summary_app.h"
#include "list_menu.h"
#include "overlay_68_move_relearner.h"
#include "font.h"
#include "overlay_manager.h"
#include "sav_chatot.h"

struct SOUND_CHATOT;

struct ListMenuCursor;

struct ListMenu2D;

struct MoveRelearner;

struct String;

struct Window;

u8 GetWindowWidth(Window *window);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

enum {
    ALIGN_LEFT = 0,
    ALIGN_RIGHT,
    ALIGN_CENTER,
};

void ov68_021E6234(MoveRelearner *controller, u32 windowID, u32 fontID, TextColor color, u32 align, u8 yOffset);

void ov68_021E6234(MoveRelearner *controller, u32 windowID, u32 fontID, TextColor color, u32 align, u8 yOffset) {
    u8 xOffset;
    u8 strWidth, winWidth;

    switch (align) {
    case ALIGN_LEFT:
        xOffset = 0;
        break;

    case ALIGN_RIGHT:
        strWidth = FontID_String_GetWidth((enum Font)fontID, controller->string, 0);
        winWidth = GetWindowWidth(&controller->windows[windowID]) * 8;
        xOffset = winWidth - strWidth;
        break;

    case ALIGN_CENTER:
        strWidth = FontID_String_GetWidth((enum Font)fontID, controller->string, 0);
        winWidth = GetWindowWidth(&controller->windows[windowID]) * 8;
        xOffset = (winWidth - strWidth) / 2;
        break;
    }

    AddTextPrinterParameterizedWithColor(&controller->windows[windowID], fontID, controller->string, xOffset, yOffset, 0xFF, color, 0L);
}

#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "text.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "msgdata.h"
#include "overlay_08_battle_party.h"
#include "message_printer.h"
#include "battle/battle_bag_context.h"

struct BattleBagContext;
struct BattleSubMenuCursor;
struct MessagePrinter;
struct Indicator;
struct MessageFormat;
typedef struct Indicator Indicator;
typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
typedef u32 TextColor;

void ov08_022234A4(BattleBag *battleBag, u8 windowIndex, u32 textID, enum Font font, u32 yOffset, TextColor color);

void ov08_022234A4(BattleBag *battleBag, u8 windowIndex, u32 textID, enum Font font, u32 yOffset, TextColor color) {
    Window *window = &battleBag->windows[windowIndex];
    String *string = NewString_ReadMsgData(battleBag->messageLoader, textID);
    u32 stringWidth = FontID_String_GetWidth(font, string, 0);
    u32 xOffset = (GetWindowWidth(window) * 8 - stringWidth) / 2;

    AddTextPrinterParameterizedWithColor(window, font, string, xOffset, yOffset, 0xFF, color, 0L);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

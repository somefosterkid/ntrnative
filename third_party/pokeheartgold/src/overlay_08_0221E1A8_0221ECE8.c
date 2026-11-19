#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "text.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "msgdata.h"
#include "message_printer.h"
#include "battle/battle_party_context.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct Pokemon;

struct MessageFormat;

typedef u32 TextColor;

void ov08_0221ECE8(BattleParty *battleParty, u32 windowIndex);

void ov08_0221ECE8(BattleParty *battleParty, u32 windowIndex) {
    Window *window = &battleParty->windows[windowIndex];
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 51);

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

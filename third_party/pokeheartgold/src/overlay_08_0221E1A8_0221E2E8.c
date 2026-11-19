#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "text.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "battle/battle_party_context.h"
#include "overlay_08_battle_party.h"
#include "msgdata.h"
#include "message_printer.h"
#include "render_window.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct Pokemon;

struct MessageFormat;

typedef u32 TextColor;

void ov08_0221E2E8(BattleParty *battleParty, u16 windowIndex, u8 xOffset, u8 yOffset);

void ov08_0221E2E8(BattleParty *battleParty, u16 windowIndex, u8 xOffset, u8 yOffset) {
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 14);

    AddTextPrinterParameterizedWithColor(&battleParty->windows[windowIndex], FONT_SYSTEM, string, xOffset, yOffset, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    ScheduleWindowCopyToVram(&battleParty->windows[windowIndex]);
}


enum BattlePartyMessageBox {
    BATTLE_PARTY_MESSAGE_BOX_SELECT_OPTION = 0,
    BATTLE_PARTY_MESSAGE_BOX_ERROR,
};

void ov08_0221E340(BattleParty *battleParty, u32 textID);

void ov08_0221E340(BattleParty *battleParty, u32 textID) {
    String *string;

    DrawFrameAndWindow2(&battleParty->messageBoxWindows[BATTLE_PARTY_MESSAGE_BOX_SELECT_OPTION], 1, 1, 14);
    FillWindowPixelBuffer(&battleParty->messageBoxWindows[BATTLE_PARTY_MESSAGE_BOX_SELECT_OPTION], 15);

    string = NewString_ReadMsgData(battleParty->messageLoader, textID);

    AddTextPrinterParameterizedWithColor(&battleParty->messageBoxWindows[BATTLE_PARTY_MESSAGE_BOX_SELECT_OPTION], FONT_MESSAGE, string, 0, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    ScheduleWindowCopyToVram(&battleParty->messageBoxWindows[BATTLE_PARTY_MESSAGE_BOX_SELECT_OPTION]);
}

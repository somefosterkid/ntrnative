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

enum BattlePartyScreen {
    BATTLE_PARTY_SCREEN_POKEMON_PARTY = 0,
    BATTLE_PARTY_SCREEN_SELECT_POKEMON,
    BATTLE_PARTY_SCREEN_POKEMON_SUMMARY,
    BATTLE_PARTY_SCREEN_POKEMON_MOVES,
    BATTLE_PARTY_SCREEN_MOVE_SUMMARY,
    BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP,
    BATTLE_PARTY_SCREEN_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE_CONTEST_STATS,
};

typedef u32 TextColor;

void ov08_0221F220(BattleParty *battleParty);

void ov08_0221F220(BattleParty *battleParty) {
    Window *window;
    String *string;

    if (battleParty->currentScreen == BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE) {
        window = &battleParty->windows[BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS];
    } else {
        window = &battleParty->windows[BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP];
    }

    FillWindowPixelBuffer(window, 0);

    string = NewString_ReadMsgData(battleParty->messageLoader, 60);

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

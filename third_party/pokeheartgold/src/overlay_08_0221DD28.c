#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "battle/battle_party_context.h"
#include "message_printer.h"
#include "msgdata.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct Pokemon;

struct MessageFormat;

void ov08_0221DD28(BattleParty *battleParty);

void ov08_0221F4A4(BattleParty *battleParty);
void ov08_0221F5D0(BattleParty *battleParty);
void ov08_0221F7C0(BattleParty *battleParty);
void ov08_0221F900(BattleParty *battleParty);
void ov08_0221FB18(BattleParty *battleParty);
void ov08_0221FC7C(BattleParty *battleParty);
void ov08_0221FDA4(BattleParty *battleParty);
void ov08_0221FF70(BattleParty *battleParty);
void ov08_02220084(BattleParty *battleParty);
void ov08_0221DD70(BattleParty *battleParty, u32 screen);

void ov08_0221DD28(BattleParty *battleParty) {
    WindowArray_Delete(battleParty->windows, battleParty->numWindows);
}

void ov08_0221DD40(BattleParty *battleParty);

void ov08_0221DD40(BattleParty *battleParty) {
    WindowArray_Delete(battleParty->windows, battleParty->numWindows);

    for (u32 i = 0; i < 2; i++) {
        RemoveWindow(&battleParty->messageBoxWindows[i]);
    }
}

void ov08_0221DD70(BattleParty *battleParty, u32 screen) {
    switch (screen) {
    case 0:
        ov08_0221F4A4(battleParty);
        return;
    case 1:
        ov08_0221F5D0(battleParty);
        return;
    case 2:
        ov08_0221F900(battleParty);
        return;
    case 3:
        ov08_0221F7C0(battleParty);
        return;
    case 4:
        ov08_0221FB18(battleParty);
        return;
    case 5:
        ov08_0221FF70(battleParty);
        return;
    case 6:
    case 8:
        ov08_0221FC7C(battleParty);
        return;
    case 7:
        ov08_0221FDA4(battleParty);
        return;
    case 9:
        ov08_02220084(battleParty);
    }
    return;
}

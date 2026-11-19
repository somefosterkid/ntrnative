#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "message_printer.h"
#include "msgdata.h"
#include "battle/battle_party_context.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct Pokemon;

struct MessageFormat;

BOOL ov08_0221DB04(BattleParty *battleParty);

BOOL ov08_0221DB24(BattleParty *battleParty, u8 partySlot);

BOOL ov08_0221DB24(BattleParty *battleParty, u8 partySlot) {
    if (ov08_0221DB04(battleParty) == 1) {
        if (battleParty->battlerSlot == 2) {
            if ((partySlot & 1) != 0) {
                return 1;
            }
        } else {
            if ((partySlot & 1) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

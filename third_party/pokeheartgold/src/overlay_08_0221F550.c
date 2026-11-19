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
#include "message_printer.h"
#include "msgdata.h"
#include "battle/battle_party_context.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct Pokemon;

struct MessageFormat;

void ov08_0221DFC8(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y);

void ov08_0221E048(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y);

void ov08_0221F550(BattleParty *battleParty, u8 windowIndex);

void ov08_0221DF80(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y);
void ov08_0221F5B0(BattleParty *battleParty, u8 partyIndex);

void ov08_0221F550(BattleParty *battleParty, u8 windowIndex) {
    FillWindowPixelRect(&battleParty->windows[windowIndex], 0, 56, 32, 24, 8);
    FillWindowPixelRect(&battleParty->windows[windowIndex], 0, 64, 24, 64, 8);

    ov08_0221DFC8(battleParty, windowIndex, windowIndex, 56, 32);
    ov08_0221E048(battleParty, windowIndex, windowIndex, 64, 24);
}

void ov08_0221F5B0(BattleParty *battleParty, u8 partyIndex) {
    if (battleParty->partyPokemon[partyIndex].isEgg == FALSE) {
        ov08_0221DF80(battleParty, partyIndex, partyIndex, 0, 32);
    }
}

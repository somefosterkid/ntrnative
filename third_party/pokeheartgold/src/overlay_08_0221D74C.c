#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_party.h"
#include "gf_gfx_planes.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "msgdata.h"
#include "battle/battle_party_context.h"
#include "message_printer.h"

struct BattlePartyContext;

struct MessagePrinter;

void ov08_0221D74C(BattleParty *battleParty, u16 fillVal, u16 x, u16 y);

void ov08_0221D74C(BattleParty *battleParty, u16 fillVal, u16 x, u16 y) {
    FillBgTilemapRect(battleParty->background, GF_BG_LYR_SUB_3, fillVal, x, y, 1, 1, 16);
}

void ov08_0221D77C(BattleParty *battleParty, u16 fillVal, u8 pointSlot);

void ov08_0221D77C(BattleParty *battleParty, u16 fillVal, u8 pointSlot) {
    FillBgTilemapRect(battleParty->background, GF_BG_LYR_SUB_3, fillVal, 2 + pointSlot * 2, 14, 1, 1, 16);
    FillBgTilemapRect(battleParty->background, GF_BG_LYR_SUB_3, fillVal + 1, 3 + pointSlot * 2, 14, 1, 1, 16);
    FillBgTilemapRect(battleParty->background, GF_BG_LYR_SUB_3, fillVal + (8 * (8 / 2)), 2 + pointSlot * 2, 15, 1, 1, 16);
    FillBgTilemapRect(battleParty->background, GF_BG_LYR_SUB_3, fillVal + (8 * (8 / 2)) + 1, 3 + pointSlot * 2, 15, 1, 1, 16);
}

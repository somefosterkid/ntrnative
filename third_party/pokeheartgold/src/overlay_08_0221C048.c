#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02020A0C.h"
#include "wild_encounter.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"

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

void ov08_0221D0F4(void *arg0);

void ov08_0221D184(BattleParty *battleParty);

void ov08_0221D8B0(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_0221DC00(BattleParty *battleParty);

void ov08_022205E0(void *arg0);

void ov08_0222171C(BattleParty *battleParty, enum BattlePartyScreen screen);

s32 ov08_0221CDF8(BattleParty *);

s32 ov08_0221CF38(BattleParty *);

s32 ov08_0221D6CC(BattleParty *, u8);

s32 ov08_0221DB24(BattleParty *, u8);

s32 ov08_0221DD70(BattleParty *, u32);

s32 ov08_02220C5C(BattleParty *, u8);

s32 ov08_022221CC(BattleParty *, u8);

s32 ov08_02222524(BattleParty *, u8);

BattleSubMenuCursor *ov08_02224B64(enum HeapID);

s32 ov08_02224B90(BattleSubMenuCursor *, s32);

u8 ov08_0221C048(BattleParty *battleParty);

u8 ov08_0221C048(BattleParty *battleParty) {
    u8 v0;

    *(s16 *)REG_DB_BLDCNT_ADDR = 0;
    if (battleParty->context->battlePartyMode == 3) {
        battleParty->currentScreen = 6;
        v0 = 0x13;
    } else {
        battleParty->currentScreen = 0;
        v0 = 1;
    }
    battleParty->cursor = ov08_02224B64(battleParty->context->heapID);
    ov08_0221D184(battleParty);
    ov08_0221CDF8(battleParty);
    ov08_0221CF38(battleParty);
    ov08_0221D0F4(battleParty);
    FontID_Alloc(FONT_UNK_4, battleParty->context->heapID);
    ov08_0221D8B0(battleParty, (enum BattlePartyScreen)battleParty->currentScreen);
    ov08_022221CC(battleParty, battleParty->currentScreen);
    ov08_02222524(battleParty, battleParty->currentScreen);
    ov08_022205E0((u8 *)battleParty);
    ov08_02220C5C(battleParty, battleParty->currentScreen);
    ov08_0221DC00(battleParty);
    ov08_0221DD70(battleParty, battleParty->currentScreen);
    if (battleParty->context->isCursorEnabled != 0) {
        ov08_02224B90(battleParty->cursor, 1);
    }
    if ((battleParty->currentScreen == 0) && (ov08_0221DB24(battleParty, 0) == 1)) {
        battleParty->context->selectedPartyIndex = 1;
    }
    ov08_0222171C(battleParty, (enum BattlePartyScreen)battleParty->currentScreen);
    ov08_0221D6CC(battleParty, battleParty->currentScreen);
    PaletteData_BeginPaletteFade(battleParty->palette, 0xAU, 0xFFFFU, -8, 0x10U, 0U, 0U);
    return v0;
}

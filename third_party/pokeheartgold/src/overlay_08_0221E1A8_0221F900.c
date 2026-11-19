#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

struct BattleSystem;


enum BattlePokemonSummaryScreenWindow {
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_POKEMON_NAME = 0,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ABILITY_NAME,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ABILITY_DESC,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HELD_ITEM,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HEALTH_BAR,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_CHECK_MOVES,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_ABILITY_NAME,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_ABILITY_DESC,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_HELD_ITEM,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_HP,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_ATTACK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_DEFENSE,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_SPEED,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_SP_ATK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_SP_DEF,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_HEALTH_BAR,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_LEVEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_EXP_TO_NEXT_LVL,
};

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void ov08_0221E048(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y);

void ov08_0221E120(BattleParty *battleParty, u32 windowIndex, u32 partyIndex);

void ov08_0221E1A8(BattleParty *battleParty, u32 windowIndex, u32 partyIndex);

void ov08_0221E3A4(BattleParty *battleParty, u32 windowIndex, u32 textID);

void ov08_0221E408(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E5DC(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E6D8(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E7D4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E8D4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E9D4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221EAD4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221EC6C(BattleParty *battleParty, u32 partyIndex);

void ov08_0221F900(BattleParty *battleParty);

void ov08_0221F900(BattleParty *battleParty) {
    u32 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;

    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_CHECK_MOVES], 0);

    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_POKEMON_NAME + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HEALTH_BAR + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ABILITY_NAME + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ABILITY_DESC + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HELD_ITEM + windowIndexOffset], 0);

    ov08_0221DDCC(battleParty, BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_POKEMON_NAME + windowIndexOffset, FONT_SYSTEM, battleParty->context->selectedPartyIndex, 0, 0);
    ov08_0221EAD4(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E048(battleParty, BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HEALTH_BAR + windowIndexOffset, battleParty->context->selectedPartyIndex, 0, 0);
    ov08_0221E408(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E5DC(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E6D8(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E7D4(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E8D4(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E9D4(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E120(battleParty, BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ABILITY_NAME + windowIndexOffset, battleParty->context->selectedPartyIndex);
    ov08_0221E1A8(battleParty, BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HELD_ITEM + windowIndexOffset, battleParty->context->selectedPartyIndex);
    ov08_0221EC6C(battleParty, battleParty->context->selectedPartyIndex);
    ov08_0221E3A4(battleParty, BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_CHECK_MOVES, 19);

    battleParty->useAltSummaryWindows ^= 1;
}

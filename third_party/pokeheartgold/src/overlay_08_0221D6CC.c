#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"

#include "bg_window.h"
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
#include "unk_0208805C.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"
#include "gf_gfx_planes.h"

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

void ov08_0221D74C(BattleParty *battleParty, u16 fillVal, u16 x, u16 y);

void ov08_0221D6CC(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_0221D6CC(BattleParty *battleParty, enum BattlePartyScreen screen) {
    BattlePartyPokemon *pokemon;
    u32 expFromCurrentToNextLevel;
    u32 expTowardsNextLevel;
    u16 fillVal;
    u8 expBarFilledPixels;
    u8 i;

    if (screen != BATTLE_PARTY_SCREEN_POKEMON_SUMMARY) {
        return;
    }

    pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    if (pokemon->level < 100) {
        expFromCurrentToNextLevel = pokemon->nextLevelExp - pokemon->currentLevelBaseExp;
        expTowardsNextLevel = pokemon->exp - pokemon->currentLevelBaseExp;
    } else {
        expFromCurrentToNextLevel = 0;
        expTowardsNextLevel = 0;
    }

    expBarFilledPixels = CalculateHpBarPixelsLength(expTowardsNextLevel, expFromCurrentToNextLevel, 8 * 8);

    for (i = 0; i < 8; i++) {
        if (expBarFilledPixels >= 8) {
            fillVal = 22 + 8;
        } else {
            fillVal = 22 + expBarFilledPixels;
        }

        ov08_0221D74C(battleParty, fillVal, 10 + i, 8);

        if (expBarFilledPixels < 8) {
            expBarFilledPixels = 0;
        } else {
            expBarFilledPixels -= 8;
        }
    }

    ScheduleBgTilemapBufferTransfer(battleParty->background, GF_BG_LYR_SUB_3);
}

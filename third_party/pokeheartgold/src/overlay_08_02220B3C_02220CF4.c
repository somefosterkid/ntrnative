#include "global.h"

#include "constants/species.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

s32 ov08_02220A8C(void *, s32, s32);

s32 ov08_02220B90(u32, void *, s32, s32);

s32 ov08_02220C08(u16, void *, s32, s32);

s32 ov08_02220C3C(u8, void *, s32, s32);

extern s32 ov08_02225534[6][2];

extern s32 ov08_02225564[6][2];

void ov08_02220CF4(BattleParty *battleParty);

void ov08_02220CF4(BattleParty *battleParty) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (battleParty->partyPokemon[i].species != SPECIES_NONE) {
            ov08_02220A8C(battleParty->sprites[7 + i], ov08_02225534[i][0], ov08_02225534[i][1]);
            ov08_02220B90(battleParty->partyPokemon[i].summaryStatus, battleParty->sprites[13 + i], ov08_02225564[i][0], ov08_02225564[i][1]);
            ov08_02220C08(battleParty->partyPokemon[i].heldItem, battleParty->sprites[i], ov08_02225534[i][0] + 8, ov08_02225534[i][1] + 8);
            ov08_02220C3C(battleParty->partyPokemon[i].mail, battleParty->sprites[32 + i], ov08_02225534[i][0] + 16, ov08_02225534[i][1] + 8);
        }
    }
}

#include "global.h"

#include "constants/moves.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

s32 ov08_02220A8C(void *, s32, s32);

s32 ov08_02220AEC(BattleParty *, void *, s32, u8);

s32 ov08_02220B90(u32, void *, s32, s32);

s32 ov08_02220BB4(BattleParty *, void *, s32 *);

s32 ov08_02220C08(u16, void *, s32, s32);

s32 ov08_02220C3C(u8, void *, s32, s32);

extern s32 ov08_022253D4;

extern s32 ov08_02225454[4][2];

void ov08_02220E80(BattleParty *battleParty);

void ov08_02220E80(BattleParty *battleParty) {
    u32 i;
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedPartyIndex + 7], 0x18, 0xC);
    ov08_02220B90(pokemon->summaryStatus, battleParty->sprites[battleParty->context->selectedPartyIndex + 13], 0xC6, 0x14);
    ov08_02220BB4(battleParty, pokemon, &ov08_022253D4);
    ov08_02220C08(pokemon->heldItem, battleParty->sprites[battleParty->context->selectedPartyIndex], 0x20, 0x14);
    ov08_02220C3C(pokemon->mail, battleParty->sprites[battleParty->context->selectedPartyIndex + 32], 0x28, 0x14);
    for (i = 0; i < 4; i++) {
        if (pokemon->moves[i].move != MOVE_NONE) {
            ov08_02220AEC(battleParty, battleParty->sprites[21 + i], i + 0xB010, pokemon->moves[i].type);
            ov08_02220A8C(battleParty->sprites[21 + i], ov08_02225454[i][0], ov08_02225454[i][1]);
        }
    }
}

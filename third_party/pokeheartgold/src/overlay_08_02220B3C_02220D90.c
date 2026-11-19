#include "global.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

s32 ov08_02220A8C(void *, s32, s32);

s32 ov08_02220C08(u16, void *, s32, s32);

s32 ov08_02220C3C(u8, void *, s32, s32);

void ov08_02220D90(BattleParty *battleParty);

void ov08_02220D90(BattleParty *battleParty) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedPartyIndex + 7], 0x80, 0x48);
    ov08_02220C08(pokemon->heldItem, battleParty->sprites[battleParty->context->selectedPartyIndex], 0x88, 0x50);
    ov08_02220C3C(pokemon->mail, battleParty->sprites[battleParty->context->selectedPartyIndex + 32], 0x90, 0x50);
}

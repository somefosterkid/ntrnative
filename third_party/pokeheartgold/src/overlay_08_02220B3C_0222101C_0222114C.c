#include "global.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

s32 ov08_02220A8C(void *, s32, s32);

s32 ov08_02220BB4(BattleParty *, void *, s32 *);

s32 ov08_02220C08(u16, void *, s32, s32);

s32 ov08_02220C3C(u8, void *, s32, s32);

s32 ov08_022213C8(BattleParty *);

extern s32 ov08_022253C4;

void ov08_0222114C(BattleParty *battleParty);

void ov08_0222114C(BattleParty *battleParty) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedPartyIndex + 7], 0x18, 0xC);
    ov08_02220BB4(battleParty, pokemon, &ov08_022253C4);
    ov08_02220C08(pokemon->heldItem, battleParty->sprites[battleParty->context->selectedPartyIndex], 0x20, 0x14);
    ov08_02220C3C(pokemon->mail, battleParty->sprites[battleParty->context->selectedPartyIndex + 32], 0x28, 0x14);
    ov08_022213C8(battleParty);
}

s32 ov08_02221500(BattleParty *);

void ov08_022211B8(BattleParty *battleParty);

void ov08_022211B8(BattleParty *battleParty) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedPartyIndex + 7], 0x18, 0xC);
    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedMoveSlot + 21], 0x88, 0x48);
    ov08_02220C08(pokemon->heldItem, battleParty->sprites[battleParty->context->selectedPartyIndex], 0x20, 0x14);
    ov08_02220C3C(pokemon->mail, battleParty->sprites[battleParty->context->selectedPartyIndex + 32], 0x28, 0x14);
    ov08_02221500(battleParty);
}

void ov08_02221230(BattleParty *battleParty);

void ov08_02221230(BattleParty *battleParty) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedPartyIndex + 7], 0x18, 0xC);
    ov08_02220C08(pokemon->heldItem, battleParty->sprites[battleParty->context->selectedPartyIndex], 0x20, 0x14);
    ov08_02220C3C(pokemon->mail, battleParty->sprites[battleParty->context->selectedPartyIndex + 32], 0x28, 0x14);
    ov08_022213C8(battleParty);
    ov08_02221500(battleParty);
}

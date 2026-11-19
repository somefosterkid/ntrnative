#include "global.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

s32 ov08_02220A8C(void *, s32, s32);

s32 ov08_02220B3C(void *, void *, u16);

s32 ov08_02220B90(u32, void *, s32, s32);

s32 ov08_02220BB4(void *, void *, s32 *);

s32 ov08_02220C08(u16, void *, s32, s32);

s32 ov08_02220C3C(u8, void *, s32, s32);

extern s32 ov08_022253E4;

void ov08_02220F58(BattleParty *battleParty);

void ov08_02220F58(BattleParty *battleParty) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedPartyIndex + 7], 0x18, 0xC);
    ov08_02220B90(pokemon->summaryStatus, battleParty->sprites[battleParty->context->selectedPartyIndex + 13], 0xC6, 0x14);
    ov08_02220BB4(battleParty, pokemon, &ov08_022253E4);
    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedMoveSlot + 21], 0x88, 0x30);
    ov08_02220C08(pokemon->heldItem, battleParty->sprites[battleParty->context->selectedPartyIndex], 0x20, 0x14);
    ov08_02220C3C(pokemon->mail, battleParty->sprites[battleParty->context->selectedPartyIndex + 32], 0x28, 0x14);
    ov08_02220B3C(battleParty, battleParty->sprites[26], pokemon->moves[battleParty->context->selectedMoveSlot].class);
    ov08_02220A8C(battleParty->sprites[26], 0x18, 0x58);
}

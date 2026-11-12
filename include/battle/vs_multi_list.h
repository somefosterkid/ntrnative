// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/vs_multi_list.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_VS_MULTI_LIST_H
#define BWPORT_BATTLE_VS_MULTI_LIST_H

#include "types.h"
#include "struct_decls.h"

struct VsMultiListParam {
    // The teams, each side's three first Pokémon showing
    PokeParty *party0;
    PokeParty *party1;
    // Whether the teams are in the opposite places, which also sends the lists in from the opposite sides
    u32 flipped;
};

#endif // BWPORT_BATTLE_VS_MULTI_LIST_H

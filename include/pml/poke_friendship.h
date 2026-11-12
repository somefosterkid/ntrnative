// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/poke_friendship.h: the lines in use so
// far.
#ifndef BWPORT_PML_POKE_FRIENDSHIP_H
#define BWPORT_PML_POKE_FRIENDSHIP_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The reasons of FriendshipManagerCalc, rows of its table. Only those the code shows are named
#define FRIENDSHIP_REASON_LEVEL_UP_PARTY 2

#define FRIENDSHIP_REASON_WALK 3

// Black 2's, not checked against Black
#define FRIENDSHIP_REASON_COUNT 8

// The friendship of walking, which a Pokémon gets half the times
void happinessAdd(PartyPkm *pkm, u16 zoneId, HeapID heapId);

#endif // BWPORT_PML_POKE_FRIENDSHIP_H

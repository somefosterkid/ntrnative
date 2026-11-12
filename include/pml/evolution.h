// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/evolution.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_PML_EVOLUTION_H
#define BWPORT_PML_EVOLUTION_H

#include "types.h"
#include "stddef.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// What the check is made for, and so which of the evolution methods it looks at
#define EVO_CHECK_LEVEL_UP 0

#define EVO_CHECK_TRADE 1
#define EVO_CHECK_ITEM_UNUSED 2
#define EVO_CHECK_ITEM 3

// The evolutions of a species, and one of them: a method, EVO_METHOD_*, its parameter and the species it gives
#define EVOLUTION_COUNT 7

typedef struct {
    u16 method;
    u16 param;
    u16 species;
} Evolution;

// The fields of an evolution, for getEvolutionInfoParam
#define EVOLUTION_FIELD_METHOD 0

#define EVOLUTION_FIELD_PARAM 1
#define EVOLUTION_FIELD_SPECIES 2

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// a3 depends on the mode a2: the zone for 0 (level-up), the partner PartyPkm (or 0) for 1 (trade), the item for 3; a
// uintptr_t, as it carries the partner's address
u16 CheckEvolveSpecies(PokeParty *party, PartyPkm *pkm, u32 a2, uintptr_t a3, u8 season, u32 *method, HeapID heapId);

#endif // BWPORT_PML_EVOLUTION_H

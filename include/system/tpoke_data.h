// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/tpoke_data.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_TPOKE_DATA_H
#define BWPORT_SYSTEM_TPOKE_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The index of the entry that fits the Pokémon, or TPOKE_INDEX_NONE
int GetFieldPokemonMMdlLUTIndex_(const TPokeData *data, u16 species, u16 sex, u16 form);

// What GetFieldPokemonMMdlLUTIndex_ returns when no entry fits
#define TPOKE_INDEX_NONE 0xFFFF

// The object code of the entry that fits the Pokémon, or of entry 0 if none does
u16 GetPokemonFieldOBJCODE(TPokeData *data, u16 species, u16 sex, u16 form);
BOOL IsFieldPokemonSpriteHugeBillboard(void *unused, TPokeData *data, u16 species, u16 sex, u16 form);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
TPokeData *LoadTPokeData(HeapID heapId);
void FreeTPokeData(TPokeData *data);

#endif // BWPORT_SYSTEM_TPOKE_DATA_H

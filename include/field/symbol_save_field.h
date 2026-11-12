// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/symbol_save_field.h, with Black's
// names.
#ifndef BWPORT_FIELD_SYMBOL_SAVE_FIELD_H
#define BWPORT_FIELD_SYMBOL_SAVE_FIELD_H

// Overlay 10's symbol_save_field.c (a descriptive name): what the field reads of the symbol encounters of the Entree
// Forest, which the save keeps in its area NPC data, encrypted

#include "types.h"
#include "field/entree_forest.h"
#include "struct_decls.h"

// The area NPC data of the save: the forest's Pokémon, ten in the first area, ten more in the second, then ten for
// each of three areas and twenty for each of the deeper ones
struct AreaNPCSave {
    EntreeForestPokemon pokemon[530];
    u8 unk848;
    u8 unk849;
    u8 unk84a;
    u8 unk84b;
    u16 unk84c;
    u16 unk84e;
};

u32 func_ov010_021673a8(AreaNPCSave *npcData);
u32 func_ov010_021673c4(AreaNPCSave *npcData);
// The Pokémon of an area, and how many there are
u32 func_ov010_0216749c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int area, u8 *count);

#endif // BWPORT_FIELD_SYMBOL_SAVE_FIELD_H

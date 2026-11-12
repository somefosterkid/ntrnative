// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/symbol_save.h: the lines in use so far.
#ifndef BWPORT_SAVE_SYMBOL_SAVE_H
#define BWPORT_SAVE_SYMBOL_SAVE_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the values of SYMBOL_KIND_KEEP_LARGE, SYMBOL_KIND_FREE_LARGE and
// SYMBOL_KIND_FREE_SMALL in Black
// The forest's Pokémon are kept in four ranges of slots, by kind: large and small ones, of the areas the player
// keeps them in and of the areas they arrive in
typedef enum {
    SYMBOL_KIND_KEEP_LARGE,
    SYMBOL_KIND_KEEP_SMALL,
    SYMBOL_KIND_FREE_LARGE,
    SYMBOL_KIND_FREE_SMALL,
    SYMBOL_KIND_COUNT,
} SymbolKind;

#define SYMBOL_POKEMON_MAX 530

// The block's size and its initial state
u32 getSizeofNpcData(void);
void initNpcData(AreaNPCSave *npcData);

#endif // BWPORT_SAVE_SYMBOL_SAVE_H

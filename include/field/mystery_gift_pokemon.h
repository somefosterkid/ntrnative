#ifndef BWPORT_FIELD_MYSTERY_GIFT_POKEMON_H
#define BWPORT_FIELD_MYSTERY_GIFT_POKEMON_H

// Overlay 10's mystery_gift_pokemon.c (pokebw2's ov012 file): makes the Pokemon of a Pokemon gift

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "save/mystery_gift.h"

// pokebw2's CreateMysteryGiftPokemon
PartyPkm *func_ov010_02157e9c(const MysteryGift *gift, HeapID heapId, GameData *gameData);

#endif // BWPORT_FIELD_MYSTERY_GIFT_POKEMON_H

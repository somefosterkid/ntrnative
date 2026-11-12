// Black's code at ov010 0x02157e9c-0x02158318. pokebw2's ov012 mystery_gift_pokemon.c (CreateMysteryGiftPokemon), which
// sits between delivery_irc.c and intrude_work.c there as here; its .rodata is the inlined maker's ribbon table

#include "types.h"
#include "app/mystery/mystery_gift_data.h"
#include "field/mystery_gift_pokemon.h"


// The body is the maker that overlay 121 also inlines
PartyPkm *func_ov010_02157e9c(const MysteryGift *gift, HeapID heapId, GameData *gameData) {
    return Mystery_CreateGiftPokemon(gift, heapId, gameData);
}


#include "global.h"

#include "constants/heap.h"

#include "frontier/overlay_80_02229EE0.h"

#include "heap.h"
#include "overlay_80_02229EE0.h"
#include "overlay_80_02236B78.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_80_battle_factory.h"

void ov80_0222A140(const FrontierPokemon *, Pokemon *, u32);

s32 ov80_02237120(BattleFactory *factory);

void ov80_022371B0(BattleFactory *factory) {
    s32 count;
    Pokemon *ptr;
    u32 setId;
    FrontierPokemon *puVar3;
    s32 i;
    FrontierPokemon auStack_84[2];

    for (count = Party_GetCount(factory->playersParty); 2 < count; count--) {
        Party_RemoveMon(factory->playersParty, count - 1);
    }
    ov80_0222A52C(auStack_84, factory->partnerRentalSetIDs, factory->partnerRentalIVs, factory->partnerRentalPersonalities, NULL, 2, 0xb, 0xcd);
    ptr = AllocMonZeroed(HEAP_ID_FIELD2);
    i = 0;
    puVar3 = auStack_84;
    do {
        setId = ov80_02237120(factory);
        ov80_0222A140(puVar3, ptr, setId);
        ov80_0222A3BC(factory->saveData, factory->playersParty, ptr);
        puVar3 = puVar3 + 1;
        factory->playerMonSetIDs[2 + i] = factory->partnerRentalSetIDs[i];
        i++;
    } while (i < 2);
    Heap_Free(ptr);
}

#include "global.h"

#include "overlay_80_02236B78.h"
#include "frontier/overlay_80_02229EE0.h"
#include "heap.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_80_battle_factory.h"


s32 ov80_02237120(BattleFactory *factory);
s32 ov80_0222A140(void *, Pokemon *, s32);
s32 ov80_0222A3BC(void *, Party *, Pokemon *);

u32 ov80_022370F4(u8 challengeType) {
    switch (challengeType) {
    case 0:
        return (1 << 7) | ((0 << 0) | (1 << 0));
    case 1:
        return (1 << 7) | ((1 << 1) | (1 << 0));
    case 2:
        return ((1 << 2) | (1 << 7)) | ((1 << 1) | (1 << 0)) | (1 << 3);
    case 3:
        return ((1 << 2) | (1 << 7)) | ((1 << 1) | (1 << 0)) | (1 << 3);
    }

    return (1 << 7) | ((0 << 0) | (1 << 0));
}

s32 ov80_02237120(BattleFactory *factory) {
    if (factory->isOpenLevel == 0) {
        return 0x32;
    }
    return 0x64;
}

void ov80_02237130(BattleFactory *factory) {
    Pokemon *ptr;
    s32 v1;
    FrontierPokemon *v0;

    ov80_0222A52C(factory->initialRentalMons, factory->initialRentalSetIDs, factory->initialRentalIVs, factory->initialRentalPersonalities, NULL, 6U, 0xBU, 0xCDU);
    SaveArray_Party_Init(factory->playersParty);
    ptr = AllocMonZeroed(HEAP_ID_FIELD2);
    v1 = 0;
    v0 = factory->initialRentalMons;
    do {
        ov80_0222A140(v0, ptr, ov80_02237120(factory));
        ov80_0222A3BC(factory->saveData, factory->playersParty, ptr);
        v1 += 1;
        v0 += 1;
    } while (v1 < 6);
    Heap_Free(ptr);
}

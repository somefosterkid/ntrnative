#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "overlay_70_gtsapplication_state.h"

Pokemon *ov70_02241868(void *, s32);

void ov70_022418A4(void *arg0);

void ov70_022418A4(void *arg0) {
    s32 v0;
    s32 slotno_p;
    Pokemon *pokemon;

    pokemon = ov70_02241868(arg0, *((s32 *)&((GTSApplicationState *)arg0)->screenArgument));
    if (*((u32 *)&((GTSApplicationState *)arg0)->tradedPokemonLocation.boxIndex) == 0x12) {
        CopyPokemonToPokemon(pokemon, Party_GetMonByIndex(*(Party **)((u8 *)(*(void **)arg0) + 8), *((s32 *)&((GTSApplicationState *)arg0)->tradedPokemonLocation.index)));
        return;
    }
    v0 = 0;
    slotno_p = 0;
    PCStorage_DeleteBoxMonByIndexPair(*(PCStorage **)((u8 *)(*(void **)arg0) + 0xC), *((u32 *)&((GTSApplicationState *)arg0)->tradedPokemonLocation.boxIndex), (u32) *((s32 *)&((GTSApplicationState *)arg0)->tradedPokemonLocation.index));
    PCStorage_FindFirstEmptySlot(*(PCStorage **)((u8 *)(*(void **)arg0) + 0xC), &v0, &slotno_p);
    PCStorage_PlaceMonInBoxFirstEmptySlot(*(PCStorage **)((u8 *)(*(void **)arg0) + 0xC), (u32)v0, Mon_GetBoxMon(pokemon));
}

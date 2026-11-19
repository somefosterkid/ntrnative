#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "party.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "save_special_ribbons.h"
#include "unk_02088288.h"

extern u8 _020FC7CC;

void sub_02058AEC(PokemonSummaryArgs **arg0, FieldSystem *arg1, Party *arg2, s32 arg3, s32 arg4, enum HeapID arg5);

void sub_02058AEC(PokemonSummaryArgs **arg0, FieldSystem *fieldSystem, Party *arg2, s32 arg3, s32 arg4, enum HeapID heapID) {
    s32 v1;
    Party *v0;
    PokemonSummaryArgs *pokemonSummaryArgs;
    SaveData *v2;

    v0 = arg2;
    v1 = arg3;
    v2 = fieldSystem->saveData;
    pokemonSummaryArgs = Heap_AllocAtEnd(heapID, sizeof(PokemonSummaryArgs));
    MI_CpuFill8(pokemonSummaryArgs, 0U, sizeof(PokemonSummaryArgs));
    sub_0208AD34(pokemonSummaryArgs, Save_PlayerData_GetProfile(fieldSystem->saveData));
    pokemonSummaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(v2);
    pokemonSummaryArgs->unk2C = (s32)sub_02088288(v2);
    pokemonSummaryArgs->options = Save_PlayerData_GetOptionsAddr(v2);
    pokemonSummaryArgs->party = v0;
    pokemonSummaryArgs->unk11 = 1;
    pokemonSummaryArgs->partySlot = (u8)v1;
    pokemonSummaryArgs->partyCount = (u8)Party_GetCount(pokemonSummaryArgs->party);
    pokemonSummaryArgs->moveToLearn = 0;
    pokemonSummaryArgs->unk12 = (u8)arg4;
    pokemonSummaryArgs->ribbons = Save_SpecialRibbons_Get(v2);
    pokemonSummaryArgs->menuInputStatePtr = NULL;
    pokemonSummaryArgs->isFlag982Set = sub_0208828C(v2);
    sub_02089D40(pokemonSummaryArgs, &_020FC7CC);
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PokemonSummary, pokemonSummaryArgs);
    *arg0 = pokemonSummaryArgs;
}

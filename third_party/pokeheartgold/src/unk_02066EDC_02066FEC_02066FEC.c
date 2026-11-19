#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "party.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "save.h"
#include "save_special_ribbons.h"
#include "unk_02088288.h"
#include "battle_tower_select_pokemon_task.h"

extern u8 _020FE4B0;


int sub_02066FEC(BattleTowerSelectPokemonTask *arg0, FieldSystem *arg1, enum HeapID arg2);

int sub_02066FEC(BattleTowerSelectPokemonTask *arg0, FieldSystem *arg1, enum HeapID arg2) {
    PokemonSummaryArgs *temp_r0;
    SaveData *temp_r5;
    temp_r5 = arg1->saveData;
    temp_r0 = Heap_AllocAtEnd(arg2, sizeof(PokemonSummaryArgs));
    MI_CpuFill8(temp_r0, 0U, sizeof(PokemonSummaryArgs));
    temp_r0->options = Save_PlayerData_GetOptionsAddr(temp_r5);
    temp_r0->party = SaveArray_Party_Get(temp_r5);
    temp_r0->natDexEnabled = SaveArray_IsNatDexEnabled(temp_r5);
    temp_r0->unk2C = sub_02088288(temp_r5);
    temp_r0->unk11 = 1;
    temp_r0->partySlot = arg0->selectedMonSlot;
    temp_r0->partyCount = (u8)Party_GetCount(temp_r0->party);
    temp_r0->moveToLearn = 0;
    temp_r0->unk12 = arg0->summaryMode;
    temp_r0->ribbons = Save_SpecialRibbons_Get(temp_r5);
    temp_r0->menuInputStatePtr = &arg1->menuInputState;
    temp_r0->isFlag982Set = sub_0208828C(temp_r5);
    sub_02089D40(temp_r0, &_020FE4B0);
    sub_0208AD34(temp_r0, Save_PlayerData_GetProfile(temp_r5));
    FieldSystem_LaunchApplication(arg1, &gOverlayTemplate_PokemonSummary, temp_r0);
    *arg0->partyMenu = temp_r0;
    return 3;
}

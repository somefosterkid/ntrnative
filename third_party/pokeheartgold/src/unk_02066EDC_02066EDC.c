#include "global.h"

#include "constants/heap.h"

#include "bag.h"
#include "bag_types_def.h"
#include "battle_tower_select_pokemon_task.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "options.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pokemon_types_def.h"

s32 sub_02066EDC(BattleTowerSelectPokemonTask *arg0, FieldSystem *arg1, enum HeapID arg2);

s32 sub_02066EDC(BattleTowerSelectPokemonTask *arg0, FieldSystem *arg1, enum HeapID arg2) {
    SaveData *temp_r7;
    u8 var_r2;
    PartyMenuArgs *temp_r0;

    temp_r0 = Heap_AllocAtEnd(arg2, sizeof(PartyMenuArgs));
    temp_r7 = arg1->saveData;
    MI_CpuFill8(temp_r0, 0U, sizeof(PartyMenuArgs));
    temp_r0->options = Save_PlayerData_GetOptionsAddr(temp_r7);
    temp_r0->party = SaveArray_Party_Get(temp_r7);
    temp_r0->bag = Save_Bag_Get(temp_r7);
    var_r2 = 0;
    temp_r0->unk_25 = 0;
    temp_r0->context = arg0->partyMenuMode;
    temp_r0->minMonsToSelect = arg0->minSelectionSpots;
    temp_r0->maxMonsToSelect = arg0->maxSelectionSpots;
    temp_r0->maxLevel = arg0->reqLevel;
    temp_r0->partySlot = arg0->selectedMonSlot;
    temp_r0->menuInputStatePtr = &arg1->menuInputState;
    do {
        temp_r0->selectedOrder[var_r2] = arg0->selectedSlots[var_r2];
        var_r2 += 1;
    } while ((u32)var_r2 < 6U);
    FieldSystem_LaunchApplication(arg1, &gOverlayTemplate_PartyMenu, temp_r0);
    *arg0->partyMenu = temp_r0;
    return 1;
}

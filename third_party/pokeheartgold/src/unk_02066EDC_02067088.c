#include "global.h"

#include "heap.h"
#include "task.h"
#include "battle_tower_select_pokemon_task.h"
#include "pokemon_summary_args.h"


int sub_02067088(BattleTowerSelectPokemonTask *arg0, FieldSystem *fieldSystem);

int sub_02067088(BattleTowerSelectPokemonTask *arg0, FieldSystem *fieldSystem) {
    PokemonSummaryArgs *ptr;

    if (FieldSystem_ApplicationIsRunning(fieldSystem) != 0) {
        return 3;
    }
    ptr = *arg0->partyMenu;
    arg0->selectedMonSlot = ptr->partySlot;
    Heap_Free(ptr);
    *arg0->partyMenu = NULL;
    return 0;
}

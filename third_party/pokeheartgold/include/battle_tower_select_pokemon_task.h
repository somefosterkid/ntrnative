#ifndef POKEHEARTGOLD_BATTLE_TOWER_SELECT_POKEMON_TASK_H
#define POKEHEARTGOLD_BATTLE_TOWER_SELECT_POKEMON_TASK_H

#include "global.h"

typedef struct BattleTowerSelectPokemonTask {
    int success;
    int subTask;
    u8 partyMenuMode;
    u8 summaryMode;
    u8 minSelectionSpots;
    u8 maxSelectionSpots;
    u8 reqLevel;
    u8 selectedMonSlot;
    u8 selectedSlots[6];
    void **partyMenu;
} BattleTowerSelectPokemonTask;

#endif // POKEHEARTGOLD_BATTLE_TOWER_SELECT_POKEMON_TASK_H

#ifndef POKEHEARTGOLD_MON_RIDE_TASK_H
#define POKEHEARTGOLD_MON_RIDE_TASK_H

#include "pokemon_types_def.h"
#include "sys_task.h"

typedef struct MonRideTask {
    BOOL playCutIn;
    int playerGender;
    Pokemon *partyMon;
    SysTask *HMCutInTask;
} MonRideTask;

#endif // POKEHEARTGOLD_MON_RIDE_TASK_H

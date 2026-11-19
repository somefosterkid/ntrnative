#ifndef POKEHEARTGOLD_FRONTIER_DATA_H
#define POKEHEARTGOLD_FRONTIER_DATA_H

#include "save.h"

// The Wi-Fi Battle Tower record of the Battle Frontier save block (struct in save_frontier.h).
struct WifiBattleTowerRecord;
typedef struct WifiBattleTowerRecord FrontierData;

FrontierData *Save_FrontierData_Get(SaveData *saveData);
u16 FrontierData_BattlePointAction(FrontierData *frontierData, u16 value, int func);

#endif // POKEHEARTGOLD_FRONTIER_DATA_H

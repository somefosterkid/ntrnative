#ifndef POKEHEARTGOLD_UNK_0203BA5C_H
#define POKEHEARTGOLD_UNK_0203BA5C_H

#include "script.h"

BOOL GetMomSpawnId(void);
void GetDeathWarpData(u16 spawnId, Location *dest);
void GetSpecialSpawnWarpData(u16 spawnId, Location *dest);
u32 MapHeader_GetSpawnIdForDeathWarp(u32 mapId);

s32 SpawnIdToTableIndex(s32 arg0);

#endif // POKEHEARTGOLD_UNK_0203BA5C_H

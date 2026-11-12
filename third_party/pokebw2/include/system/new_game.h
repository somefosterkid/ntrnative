#ifndef POKEBW2_SYSTEM_NEW_GAME_H
#define POKEBW2_SYSTEM_NEW_GAME_H

// Overlay 279, which sets up the save data of a new game

#include "types.h"
#include "gfl/overlay.h"
#include "struct_decls.h"

#define OVERLAY_NEW_GAME OVERLAY_ID(279)

void InitItemBag(GameData *gameData, u32 heapId);
void InitDreamRadarFlagSave(GameData *gameData, u32 heapId);

#endif // POKEBW2_SYSTEM_NEW_GAME_H

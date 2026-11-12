// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/season.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SYSTEM_SEASON_H
#define BWPORT_SYSTEM_SEASON_H

#include "types.h"
#include "struct_decls.h"

// The season the game data has and the season of the date, which are the same while the season is kept in sync with
// another player; returns whether they differ
BOOL GameData_GetSeasons(GameData *gameData, u16 *prevSeason, u16 *season);
u8 Season_GetNext(u8 season);
u8 Season_GetPrevious(u8 season);
// The season of the date: spring in January, May and September, and so on
u8 Season_GetRealTime(void);
void Season_Set(GameData *gameData, u16 season);

#define SEASON_COUNT 4

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The season of the field, from the game data that the communication system takes as the base player's
u8 GameSystem_GetSeason(GameSystem *gsys);

#endif // BWPORT_SYSTEM_SEASON_H

// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_open.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKAN_OPEN_H
#define BWPORT_APP_ZUKAN_OPEN_H

#include "types.h"
#include "struct_decls.h"

// What the Pokédex ends the opening with: B, X or the end of the opening. The Pokédex tells by the result whether to
// go on
#define ZUKAN_OPEN_RESULT_BACK 0

#define ZUKAN_OPEN_RESULT_CLOSE 1
#define ZUKAN_OPEN_RESULT_DONE 2

// Black 2's; twinned code confirms the offsets of gameData and result in Black
typedef struct {
    GameData *gameData;
    PlayerInfo *playerInfo;
    u32 result;
} ZukanOpenParam;

extern const struct GameProcFunctions data_ov211_021bc8bc;

#endif // BWPORT_APP_ZUKAN_OPEN_H

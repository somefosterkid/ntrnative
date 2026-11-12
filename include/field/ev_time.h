// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/ev_time.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_FIELD_EV_TIME_H
#define BWPORT_FIELD_EV_TIME_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/rtc.h"
#include "struct_decls.h"

// The date the OS_GetTick was last checked, followed by the time
RTCDate *getAddressAdventureTimeBlk(GameData *gameData);

// Saves the time in seconds
void setCurrentSeconds(GameData *gameData);

#endif // BWPORT_FIELD_EV_TIME_H

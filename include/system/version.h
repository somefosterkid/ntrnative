// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/version.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_VERSION_H
#define BWPORT_SYSTEM_VERSION_H

#include "types.h"

extern const u8 region;
// The version that made the save's Pokémon, as their origin game
extern const u8 game_version;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// VERSION_BLACK2 or VERSION_WHITE2
u32 getGameVersion(void);

#endif // BWPORT_SYSTEM_VERSION_H

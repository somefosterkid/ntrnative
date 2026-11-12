#ifndef POKEBW2_SYSTEM_SAVE_ERROR_H
#define POKEBW2_SYSTEM_SAVE_ERROR_H

#include "types.h"

// The screen of a save data error, which stops the game until it is reset. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// The backup system's callbacks for an error in reading and in writing the save data
void showSavegameMainError(void);
void showSavegameAltError(BOOL noResponse);

#endif // POKEBW2_SYSTEM_SAVE_ERROR_H

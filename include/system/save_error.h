// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/save_error.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_SAVE_ERROR_H
#define BWPORT_SYSTEM_SAVE_ERROR_H

#include "types.h"

void showSavegameAltError(BOOL noResponse);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The backup system's callbacks for an error in reading and in writing the save data
void showSavegameMainError(void);

#endif // BWPORT_SYSTEM_SAVE_ERROR_H

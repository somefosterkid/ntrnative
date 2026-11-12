// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/dsi.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_DSI_H
#define BWPORT_SYSTEM_DSI_H

#include "types.h"

// The DSi's settings, with what a DS answers in their place

// FALSE when the DSi's wireless communications are turned off (pokebw2's isWirelessEnabled)
BOOL func_0202eb44(void);

// TRUE when the DSi's parental controls restrict exchanging photos, despite swan's name
BOOL canPlayerExchangePhotos(void);
// The birthday in the DS's owner settings
void getBirthdayMonthDay(u8 *month, u8 *day);

// Whether the DSi's user agreed to the EULA, TRUE on a DS
BOOL hasLicenseBeenAccepted(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's isRunningOnDSi
// Whether the game runs on a DSi, through OS_IsRunOnTwl
BOOL func_0202eb0c(void);

#endif // BWPORT_SYSTEM_DSI_H

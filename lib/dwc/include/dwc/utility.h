// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/utility.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_UTILITY_H
#define BWPORT_DWC_UTILITY_H

#include "types.h"

// The work that DWC_StartUtility needs, as the caller allocates it
#define DWC_UTILITY_WORK_SIZE 0x40000

// Languages and starting screens of the settings
#define DWC_LANGUAGE_ENGLISH 1

#define DWC_UTILITY_TOP_MENU_FOR_USA 0

// pokebw2's DWC_StartUtility
// Runs the settings until the player leaves them; negative when the game runs on a DSi or the parameters are wrong
int DWC_StartUtility(void *work, int language, int param);

#endif // BWPORT_DWC_UTILITY_H

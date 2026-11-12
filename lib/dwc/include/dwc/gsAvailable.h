// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gsAvailable.h, with Black's names.
#ifndef BWPORT_DWC_GSAVAILABLE_H
#define BWPORT_DWC_GSAVAILABLE_H

#include "types.h"

// GameSpy's availability check (gsAvailable.c), in overlay 11: asks the server "<game>.available.gs.nintendowifi.net"
// whether the game's services are up, with one UDP packet that it sends again once after two seconds

// The result of the check; the services count as available when no answer came
typedef enum { GSIACWaiting, GSIACAvailable, GSIACUnavailable, GSIACTemporarilyUnavailable } GSIACResult;

extern GSIACResult data_ov009_021acef8;
// The game's name, which the check copies, and the host to ask instead, unless it is empty
extern char data_ov009_021acefc[0x40];
extern char data_ov009_021acf3c[0x40];

void func_ov009_0217c750(const char *gamename);
// Waits for the answer; returns the result once the check is done, and GSIACWaiting before
GSIACResult GSIAvailableCheckThink(void);

#endif // BWPORT_DWC_GSAVAILABLE_H

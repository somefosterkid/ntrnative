// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/screen.h, with Black's names.
#ifndef BWPORT_DWC_SCREEN_H
#define BWPORT_DWC_SCREEN_H

#include "types.h"
#include <stdarg.h>
#include <time.h>

// The screen that GameSpy's debug output prints to (screen.c, GameSpy's common/nitro/screen.c), in overlay 11, and the
// clock in seconds

// Prints a formatted text to the screen when printing is on, which it is not in the ROM
void VPrintf(const char *format, va_list args);
// The time in seconds, which it also writes to *seconds unless that is NULL
time_t func_ov009_02184098(time_t *seconds);

#endif // BWPORT_DWC_SCREEN_H

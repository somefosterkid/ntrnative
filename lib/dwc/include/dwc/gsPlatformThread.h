// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gsPlatformThread.h, with Black's names.
#ifndef BWPORT_DWC_GSPLATFORMTHREAD_H
#define BWPORT_DWC_GSPLATFORMTHREAD_H

#include "dwc/nonport.h"
#include "nitro/os.h"

// GameSpy's threads (gsPlatformThread.c), in overlay 11: counters changed with the interrupts off, and the critical
// sections, which are NitroSDK's mutexes

typedef OSMutex GSICriticalSection;

// Adds one to or takes one from a counter, and returns its new value
gsi_u32 func_ov009_0217f0b4(gsi_u32 *value);
gsi_u32 func_ov009_0217f0d8(gsi_u32 *value);
void func_ov009_0217f0fc(GSICriticalSection *section);
void func_ov009_0217f108(GSICriticalSection *section);
void func_ov009_0217f114(GSICriticalSection *section);
// Does nothing: a mutex needs no freeing
void func_ov009_0217f120(GSICriticalSection *section);

#endif // BWPORT_DWC_GSPLATFORMTHREAD_H

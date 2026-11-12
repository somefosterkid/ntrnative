// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/machine_use.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_MACHINE_USE_H
#define BWPORT_SYSTEM_MACHINE_USE_H

#include "types.h"

void SystemInit(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Does the VRAM transfers queued during the frame
void SystemUpdateVBlank(void);

// Black's dsd name; the file that defines it is not ported yet. A tail call of func_02005438
void func_020911a0(void);
void func_02005438(void);

#endif // BWPORT_SYSTEM_MACHINE_USE_H

// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/screentex.h.
#ifndef BWPORT_SYSTEM_SCREENTEX_H
#define BWPORT_SYSTEM_SCREENTEX_H

#include "types.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

// A texture of the screen: the display capture writes the main engine's output to one of VRAM banks A to D, and a
// texture resource points at it so that 3D models can draw it. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), though the file is not part of GFL

// The capture banks, 0 to 3 for VRAM banks A to D
#define SCREENTEX_BANK_A 0
#define SCREENTEX_BANK_B 1
#define SCREENTEX_BANK_C 2
#define SCREENTEX_BANK_D 3

// A texture resource of one 256x192 direct color texture, at the capture bank in the texture VRAM
void *GFL_G3DScreenTexCreate(HeapID heapId, u32 bank);
// Captures the screen to the bank over the next frames, then copies it to destBank if that is another bank
void GFL_G3DScreenTexCapture(u32 bank, u32 destBank);
// Draws the actor with a fixed camera that looks at it from the front, turned a quarter around the X axis
void GFL_G3DAnmMdlDrawHeadless(G3DActor *actor);

#endif // BWPORT_SYSTEM_SCREENTEX_H

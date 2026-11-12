// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pdc_main.h: the lines in use so far.
#ifndef BWPORT_APP_PDC_MAIN_H
#define BWPORT_APP_PDC_MAIN_H

#include "types.h"
#include "app/pdc.h"
#include "gfl/heap.h"

typedef struct PdcMain PdcMain;

// pokebw2's PdcMain_Create
PdcMain *func_ov098_021b61e8(PdcWork *pdc, HeapID heapId);
// pokebw2's PdcMain_Update
// Runs a frame: 0 while it goes on, then nonzero (1 once the Pokémon is caught, 2 when the player leaves)
u32 func_ov098_021b6308(PdcMain *work);
// pokebw2's PdcMain_Free
void func_ov098_021b632c(PdcMain *work);

#endif // BWPORT_APP_PDC_MAIN_H

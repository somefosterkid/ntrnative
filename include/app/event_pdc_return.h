// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/event_pdc_return.h: the lines in use so
// far.
#ifndef BWPORT_APP_EVENT_PDC_RETURN_H
#define BWPORT_APP_EVENT_PDC_RETURN_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// How the capture ended
#define PDC_RESULT_CAUGHT 1

// Black 2's, not checked against Black
// The Pokémon was seen but not caught
#define PDC_RESULT_SEEN 2

typedef struct {
    GameData *gameData;
    // PDC_RESULT_*
    u32 result;
    PartyPkm *pkm;
} EventPdcReturnParam;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's EventPdcReturn_CreateParam
EventPdcReturnParam *func_ov227_021b95a0(GameData *gameData, u32 result, PartyPkm *pkm, HeapID heapId);
// pokebw2's EventPdcReturn_FreeParam
void func_ov227_021b95c8(EventPdcReturnParam *param);

extern const GameProcFunctions data_ov227_021b98a4;

#endif // BWPORT_APP_EVENT_PDC_RETURN_H

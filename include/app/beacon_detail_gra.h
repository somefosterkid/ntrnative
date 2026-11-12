// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/beacon_detail_gra.h: the lines in use so
// far.
#ifndef BWPORT_APP_BEACON_DETAIL_GRA_H
#define BWPORT_APP_BEACON_DETAIL_GRA_H

#include "types.h"

typedef struct BeaconDetailGraphic BeaconDetailGraphic;
#include "gfl/clact.h"
#include "gfl/heap.h"


// pokebw2's BeaconDetailGraphic_Create
BeaconDetailGraphic *func_ov216_021bc930(u32 layout, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BeaconDetailGraphic_GetClActUnit
ClActUnit *func_ov216_021bca38(BeaconDetailGraphic *graphic);
// pokebw2's BeaconDetailGraphic_Free
void func_ov216_021bc9d0(BeaconDetailGraphic *graphic);
// pokebw2's BeaconDetailGraphic_Update
void func_ov216_021bca24(BeaconDetailGraphic *graphic);

#endif // BWPORT_APP_BEACON_DETAIL_GRA_H

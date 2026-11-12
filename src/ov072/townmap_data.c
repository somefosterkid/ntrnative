// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov144/townmap_data.c, which is the same code
// in Black 2/White 2's overlay 144 with Black's archive and number of places. The names are Black's dsd names;
// pokebw2's guesses are in the comments.
#include "app/townmap/townmap_data.h"
#include "types.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"

// The town map's table of places. The ROM doesn't name the file; the name is pokebw2's guess. It is its own file at
// the end of overlay 72: the function before it reads another table, and these are the only ones overlays 208 and 216
// call

#define ARCID_TOWNMAP_DATA 86

// pokebw2's TownMapData_Load
void *func_ov072_021bbb24(HeapID heapId) {
    return GFL_ArcSysReadHeapNewLZ(ARCID_TOWNMAP_DATA, 0, FALSE, heapId);
}

// pokebw2's TownMapData_Free
void func_ov072_021bbb34(void *data) {
    GFL_HeapFree(data);
}

// pokebw2's TownMapData_GetParam
u16 func_ov072_021bbb3c(void *data, u16 place, u32 param) {
    u16(*places)[TOWNMAP_PARAM_COUNT] = data;

    return places[place][param];
}

// pokebw2's TownMapData_GetPlaceByZone
u16 func_ov072_021bbb48(void *data, u16 zone) {
    int i;

    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        if (func_ov072_021bbb3c(data, i, TOWNMAP_PARAM_ZONE) == zone) {
            return i;
        }
    }
    return TOWNMAP_PLACE_NONE;
}

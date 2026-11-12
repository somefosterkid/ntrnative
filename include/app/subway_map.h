// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/subway_map.h: the lines in use so far.
#ifndef BWPORT_APP_SUBWAY_MAP_H
#define BWPORT_APP_SUBWAY_MAP_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"

// Black 2's, not checked against Black
typedef struct {
    PlayerInfo *playerInfo;
} SubwayMapParam;

typedef struct SubwayMapGraphic SubwayMapGraphic;

// Black 2's, not checked against Black
#define OVERLAY_SUBWAY_MAP OVERLAY_ID(317)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's SubwayMapGraphic_Free
void func_ov224_021b99e8(SubwayMapGraphic *graphic);
// pokebw2's SubwayMapGraphic_Update
void func_ov224_021b9a44(SubwayMapGraphic *graphic);
// pokebw2's SubwayMapGraphic_Create
// subway_map_graphic.c
SubwayMapGraphic *func_ov224_021b9940(u32 layout, HeapID heapId);

#endif // BWPORT_APP_SUBWAY_MAP_H

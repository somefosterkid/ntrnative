// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/townmap/townmap_data.h, the lines in
// use so far.
#ifndef BWPORT_APP_TOWNMAP_TOWNMAP_DATA_H
#define BWPORT_APP_TOWNMAP_TOWNMAP_DATA_H

#include "types.h"
#include "gfl/heap.h"

// townmap_data.c: the town map's table of places, which overlays 72, 208 and 216 read. pokebw2's guessed names are
// in the comments

#define TOWNMAP_PLACE_COUNT 61

// What func_ov072_021bbb48 returns for a zone that is no place on the map
#define TOWNMAP_PLACE_NONE 0xffff

// A place's parameters, as func_ov072_021bbb3c reads them: the first is its zone, and each place is
// TOWNMAP_PARAM_COUNT u16s
#define TOWNMAP_PARAM_ZONE 0
#define TOWNMAP_PARAM_X 2
#define TOWNMAP_PARAM_Y 3
#define TOWNMAP_PARAM_CURSOR_X 4
#define TOWNMAP_PARAM_CURSOR_Y 5
#define TOWNMAP_PARAM_HIT_START_X 6
#define TOWNMAP_PARAM_HIT_START_Y 7
#define TOWNMAP_PARAM_HIT_END_X 8
#define TOWNMAP_PARAM_HIT_END_Y 9
#define TOWNMAP_PARAM_HIT_RADIUS 10
// TOWNMAP_PLACE_TYPE_*
#define TOWNMAP_PARAM_TYPE 11
#define TOWNMAP_PARAM_AREA_ANIM 24
#define TOWNMAP_PARAM_AREA_X 25
#define TOWNMAP_PARAM_AREA_Y 26
#define TOWNMAP_PARAM_FLAG 0x10
#define TOWNMAP_PARAM_COUNT 27

// TownMapData_Load and TownMapData_Free: read the table of places, and free it
void *func_ov072_021bbb24(HeapID heapId);
void func_ov072_021bbb34(void *data);
// TownMapData_GetParam: a TOWNMAP_PARAM_* of a place
u16 func_ov072_021bbb3c(void *data, u16 place, u32 param);
// TownMapData_GetPlaceByZone: the place whose TOWNMAP_PARAM_ZONE is zone, or TOWNMAP_PLACE_NONE
u16 func_ov072_021bbb48(void *data, u16 zone);

// The Pokédex's map gives this type its own marker; in the data it is N's Castle's, the Plasma Frigate's and the
// Abyssal Ruins', not the towns' (0)
#define TOWNMAP_PLACE_TYPE_TOWN 4

#define TOWNMAP_NO_FLAG 0xffff

#endif // BWPORT_APP_TOWNMAP_TOWNMAP_DATA_H

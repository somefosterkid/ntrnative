// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_p2pmatchroom_map.h: the lines in
// use so far.
#ifndef BWPORT_APP_WIFI_P2PMATCHROOM_MAP_H
#define BWPORT_APP_WIFI_P2PMATCHROOM_MAP_H

#include "types.h"
#include "struct_decls.h"

// How many maps there are: mapNo is the number of blocks of friends, minus 1
#define WIFI_MCR_MAP_NUM 4

// A map's size in squares
typedef struct {
    u16 x;
    u16 y;
} WIFI_MCR_MAPSIZE;

// pokebw2's WIFI_MCR_MapSizeGet
WIFI_MCR_MAPSIZE func_ov099_021ba810(u32 mapNo);
// pokebw2's WIFI_MCR_MapDataGet
const WF2DMAP_MAP *func_ov099_021ba81c(u32 mapNo);

#endif // BWPORT_APP_WIFI_P2PMATCHROOM_MAP_H

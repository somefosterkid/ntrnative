// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_map.h: the lines in use
// so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_MAP_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_MAP_H

#include "types.h"
#include "struct_decls.h"

typedef struct WF2DMAP_MAPSYS WF2DMAP_MAPSYS;

// pokebw2's WF2DMAP_MAPSysInit
WF2DMAP_MAPSYS *func_ov127_021c47a0(u16 gridX, u16 gridY, u32 heapId);
// pokebw2's WF2DMAP_MAPSysDataSet
// Copies gridX * gridY squares
void func_ov127_021c4800(WF2DMAP_MAPSYS *sys, const WF2DMAP_MAP *buff);
// pokebw2's WF2DMAP_MAPSysParamGet
u32 func_ov127_021c4864(const WF2DMAP_MAPSYS *sys, u16 x, u16 y);

// What a square without data reads as: a wall
#define WF2DMAP_MAPDATA_NONE 0xffffffff

// A square of the map: its word, or its bits
union WF2DMAP_MAP {
    u32 data;
    struct {
        // Whether objects cannot enter it
        u32 hit : 1;
        u32 param : 31;
    } val;
};

// pokebw2's WF2DMAP_MAPSysDataGet
WF2DMAP_MAP func_ov127_021c4818(const WF2DMAP_MAPSYS *sys, u16 x, u16 y);
// pokebw2's WF2DMAP_MAPSysHitGet
BOOL func_ov127_021c4844(const WF2DMAP_MAPSYS *sys, u16 x, u16 y);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WF2DMAP_MAPSysExit
void func_ov127_021c47e4(WF2DMAP_MAPSYS *sys);
// pokebw2's WF2DMAP_MAPSysGridXGet
u16 func_ov127_021c47f8(const WF2DMAP_MAPSYS *sys);
// pokebw2's WF2DMAP_MAPSysGridYGet
u16 func_ov127_021c47fc(const WF2DMAP_MAPSYS *sys);

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_MAP_H

// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wf2dmap_map.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wf2dmap_map.h"
#include "gfl/heap.h"
#include "gfl/std.h"

// The grid of the Wi-Fi Club's 2D map, named after the ROM's "wf2dmap_map.c"

struct WF2DMAP_MAPSYS {
    u16 gridX;
    u16 gridY;
    WF2DMAP_MAP *buff;
};

// pokebw2's WF2DMAP_MAPSysInit
WF2DMAP_MAPSYS *func_ov127_021c47a0(u16 gridX, u16 gridY, u32 heapId) {
    WF2DMAP_MAPSYS *sys;

    sys = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_MAPSYS), TRUE, "wf2dmap_map.c", 76);
    sys->gridX = gridX;
    sys->gridY = gridY;
    sys->buff = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_MAP) * (sys->gridX * sys->gridY), TRUE, "wf2dmap_map.c", 83);
    return sys;
}

// pokebw2's WF2DMAP_MAPSysExit
void func_ov127_021c47e4(WF2DMAP_MAPSYS *sys) {
    GFL_HeapFree(sys->buff);
    GFL_HeapFree(sys);
}

// pokebw2's WF2DMAP_MAPSysGridXGet
u16 func_ov127_021c47f8(const WF2DMAP_MAPSYS *sys) {
    return sys->gridX;
}

// pokebw2's WF2DMAP_MAPSysGridYGet
u16 func_ov127_021c47fc(const WF2DMAP_MAPSYS *sys) {
    return sys->gridY;
}

// pokebw2's WF2DMAP_MAPSysDataSet
void func_ov127_021c4800(WF2DMAP_MAPSYS *sys, const WF2DMAP_MAP *buff) {
    MI_CpuCopy8(buff, sys->buff, sizeof(WF2DMAP_MAP) * (sys->gridX * sys->gridY));
}

// pokebw2's WF2DMAP_MAPSysDataGet
WF2DMAP_MAP func_ov127_021c4818(const WF2DMAP_MAPSYS *sys, u16 x, u16 y) {
    WF2DMAP_MAP map = { WF2DMAP_MAPDATA_NONE };

    if (sys->gridX > x && sys->gridY > y) {
        map = sys->buff[sys->gridX * y + x];
    }
    return map;
}

// pokebw2's WF2DMAP_MAPSysHitGet
BOOL func_ov127_021c4844(const WF2DMAP_MAPSYS *sys, u16 x, u16 y) {
    WF2DMAP_MAP map;

    map = func_ov127_021c4818(sys, x, y);
    if (map.data == WF2DMAP_MAPDATA_NONE) {
        return TRUE;
    }
    return map.val.hit;
}

// pokebw2's WF2DMAP_MAPSysParamGet
u32 func_ov127_021c4864(const WF2DMAP_MAPSYS *sys, u16 x, u16 y) {
    WF2DMAP_MAP map;

    map = func_ov127_021c4818(sys, x, y);
    if (map.data == WF2DMAP_MAPDATA_NONE) {
        return 0;
    }
    return map.val.param;
}

// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/rail_attr.c, ported by tools/port_file.py.
// The tile attributes of the rail maps: a tilemap per rail line, from archive 78, and the tile classes of the rails'
// special tiles. The name is the ROM's own, from GFL_HeapAllocate's file argument. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field_map.h"
#include "field/field_rail.h"
#include "field/zone.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"

#define ARC_RAIL_ATTR 79
#define RAIL_ATTR_NONE 0xffffffff

// A rail line's tiles, across it and along it
typedef struct {
    u16 width;
    u16 length;
    u32 tiles[];
} RailTilemap;

struct RailAttr {
    u8 *data;
    u32 size;
    u32 count;
    RailTilemap **tilemaps;
};

RailAttr *AllocateRailAttrBlock(u32 heapId) {
    return GFL_HeapAllocate(heapId, sizeof(RailAttr), TRUE, "rail_attr.c", 92);
}

void func_ov021_021bb0a4(RailAttr *attr) {
    func_ov021_021bb11c(attr);
    GFL_HeapFree(attr);
}

void FieldRailTilemap_Load(RailAttr *attr, u32 fileId, u32 heapId) {
    u32 i;
    u32 offset;

    attr->data = GFL_ArcSysReadHeapNewLZGetLen(ARC_RAIL_ATTR, fileId, FALSE, heapId, &attr->size);
    attr->count = *(u32 *)attr->data;
    attr->tilemaps = GFL_HeapAllocate(heapId, attr->count * sizeof(RailTilemap *), TRUE, "rail_attr.c", 134);
    offset = sizeof(u32);
    for (i = 0; i < attr->count; i++) {
        attr->tilemaps[i] = (RailTilemap *)(attr->data + offset);
        offset += attr->tilemaps[i]->width * attr->tilemaps[i]->length * sizeof(u32) + sizeof(RailTilemap);
    }
}

void func_ov021_021bb11c(RailAttr *attr) {
    if (attr->data != NULL) {
        GFL_HeapFree(attr->tilemaps);
        GFL_HeapFree(attr->data);
        attr->count = 0;
        attr->tilemaps = NULL;
        attr->data = NULL;
    }
}

// The attributes of the tile at a position on a line, or RAIL_ATTR_NONE
u32 FieldRailTilemap_GetTileAtPos(RailAttr *attr, const RailPosition *pos) {
    u32 tile = RAIL_ATTR_NONE;

    if (pos->componentIsLine == TRUE && attr->count > pos->componentId) {
        RailTilemap *tilemap = attr->tilemaps[pos->componentId];
        int x = pos->posSide + tilemap->width / 2;
        int z = pos->posFront;

        if (tilemap->width > x && tilemap->length > z && x >= 0 && z >= 0) {
            tile = tilemap->tiles[x + tilemap->width * z];
        }
    }
    return tile;
}

BOOL func_ov021_021bb188(u16 tileClass) {
    if (tileClass == 0xa0 || tileClass == 0xa1) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021bb1a0(u32 tileClass) {
    if (tileClass == 0xa3) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021bb1ac(u32 tileClass) {
    if (tileClass == 0xa5) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021bb1b8(u32 tileClass) {
    if (tileClass == 0xa4) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021bb1c4(u32 tileClass) {
    if (tileClass == 0xa7) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021bb1d0(u32 tileClass) {
    if (tileClass == 0xa6) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021bb1dc(u32 tileClass) {
    if (tileClass == 0xa8) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021bb1e8(u32 tileClass) {
    if (tileClass == 0xb0) {
        return TRUE;
    }
    return FALSE;
}

#include "types.h"
#include "field/field_map.h"
#include "field/field_map_chunk.h"
#include "nitro/fx.h"

// The height and tile type of a position in a chunk's terrain. The ROM doesn't name the file; the name is a guess.
// GetTerrainCore's name is swan's (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the types are ours

// A tile is 16 units wide
#define TILE_SIZE (16 * FX32_ONE)

enum {
    // One plane
    TILE_SHAPE_PLANE,
    TILE_SHAPE_UNUSED,
    // Two triangles, a MapHeightSplit
    TILE_SHAPE_SPLIT,
};

typedef struct {
    u16 shape : 2;
    // Index into MAP_HEIGHT_SLOPE_TABLE
    u16 slope : 14;
    // Index into MAP_HEIGHT_OFFSET_TABLE, or into the splits for TILE_SHAPE_SPLIT
    u16 offset;
    u32 tileType : 31;
    // How a split tile is cut: 0 along x + z = 16, 1 along x = z
    u32 diagonal : 1;
} MapHeightTile;

// The two planes of a tile cut along a diagonal. Plane 0 is the half nearer the origin, or the half with x > z
typedef struct {
    u16 unused : 2;
    u16 slope0 : 14;
    u16 slope1;
    u16 offset0;
    u16 offset1;
} MapHeightSplit;

// A terrain section of a chunk file: width * height tiles, then the splits
struct MapHeightData {
    u16 width;
    u16 height;
    MapHeightTile tiles[];
};

// clang-format off
static const fx16 MAP_HEIGHT_SLOPE_TABLE[] = {
#include "asset/field_map_chunk_height_MAP_HEIGHT_SLOPE_TABLE.inc"
};

// clang-format on

// clang-format off
static const fx32 MAP_HEIGHT_OFFSET_TABLE[] = {
#include "asset/field_map_chunk_height_MAP_HEIGHT_OFFSET_TABLE.inc"
};

// clang-format on

static inline u8 MapHeightTile_GetShape(const MapHeightTile *tile) {
    return tile->shape;
}

// Writes the plane under pos to a layer of out: its normal, its tile type and the height on it, above baseY. pos is
// relative to the chunk's center and size is the chunk's width
void FieldChunkAccessor_GetTerrainCore(MapTerrainSamplerOutput *out, u32 layer, MapHeightData *data, const VecFx32 *pos,
                                       fx32 size, fx32 baseY) {
    fx32 offset;
    const fx16 *slopes = MAP_HEIGHT_SLOPE_TABLE;
    fx32 half = size / 2;
    fx32 z = pos->z + half;
    fx32 x = pos->x + half;
    fx32 tileX = x % TILE_SIZE;
    fx32 tileZ = z % TILE_SIZE;
    MapHeightTile *tile = &data->tiles[x / TILE_SIZE + (FX_Whole(size) / 16) * (z / TILE_SIZE)];
    u8 shape = MapHeightTile_GetShape(tile);
    fx32 ny;
    fx32 dz;
    fx32 dx;

    if (shape == TILE_SHAPE_PLANE) {
        unsigned int slope = tile->slope * 3;
        u16 offsetId = tile->offset;

        VEC_Fx16Set(&out->layers[layer].normal, slopes[slope], slopes[slope + 1], -slopes[slope + 2]);
        offset = MAP_HEIGHT_OFFSET_TABLE[offsetId];
    } else if (shape == TILE_SHAPE_SPLIT) {
        MapHeightSplit *split = (MapHeightSplit *)&data->tiles[data->width * data->height] + tile->offset;
        unsigned int slope;
        u16 offsetId;

        if (tile->diagonal == 0) {
            if (tileX + tileZ < TILE_SIZE) {
                slope = split->slope0 * 3;
                offsetId = split->offset0;
            } else {
                slope = split->slope1 * 3;
                offsetId = split->offset1;
            }
        } else {
            if (tileX > tileZ) {
                slope = split->slope0 * 3;
                offsetId = split->offset0;
            } else {
                slope = split->slope1 * 3;
                offsetId = split->offset1;
            }
        }
        VEC_Fx16Set(&out->layers[layer].normal, slopes[slope], slopes[slope + 1], -slopes[slope + 2]);
        offset = MAP_HEIGHT_OFFSET_TABLE[offsetId];
    } else {
        VEC_Fx16Set(&out->layers[layer].normal, 0, FX16_ONE, 0);
        offset = 0;
    }
    ny = out->layers[layer].normal.y;
    dz = FX_Mul(out->layers[layer].normal.z, pos->z);
    dx = FX_Mul(out->layers[layer].normal.x, pos->x);
    out->layers[layer].tileType = tile->tileType;
    out->layers[layer].height = FX_Div(-(offset + (dx + dz)), ny) + baseY;
}

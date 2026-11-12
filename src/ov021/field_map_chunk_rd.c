// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/field_map_chunk_rd.c. Its
// FieldChunkAccessor_RD_GetTerrain is the same code in Black 2/White 2's overlay 36; FieldChunkAccessor_RD_Update is
// adapted to Black, which builds the White Forest/Black City map where Black 2 builds its Join Avenue.
#include "field/field_map_chunk.h"
#include "types.h"
#include "field/field_g3d_mapper.h"
#include "field/field_wfbc.h"
#include "field/field_map.h"
#include "field/field_prop.h"
#include "field/field_terrain_animator.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"

// An 'RD' chunk file, whose terrain holds its planes itself and which the White Forest/Black City map is built into:
// a model, terrain and props. The ROM doesn't name the file; the name is a guess. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the types are pokebw2's

// A tile is 16 units wide
#define TILE_SIZE (16 * FX32_ONE)

// The start of the file. Each offset is from the start of the file
typedef struct {
    // 'RD'
    u16 magic;
    u16 sectionCount;
    u32 modelOffset;
    u32 terrainOffset;
    u32 propsOffset;
    u32 endOffset;
} RDChunkFile;

// A tile's two planes, cut along a diagonal. Plane 0 is the half nearer the origin, or the half with x > z
typedef struct {
    VecFx16 normal0;
    VecFx16 normal1;
    fx32 offset0;
    fx32 offset1;
    u32 tileType : 31;
    // 0 if the tile is cut along x + z = 16, 1 if along x = z
    u32 diagonal : 1;
} RDTile;

// The terrain section
typedef struct {
    u16 width;
    u16 height;
    RDTile tiles[];
} RDTerrain;

// Loads the chunk a step at a time: reads the file, then binds its model, places its props and the White
// Forest/Black City map's 4x4 blocks and starts its texture animation. Returns FALSE once the chunk is loaded
BOOL FieldChunkAccessor_RD_Update(FieldChunk *chunk, FieldChunkContext *context) {
    FieldChunkLoader *loader;
    u32 datId;
    void *container;
    u32 propCount;

    FieldChunkContext_GetHeapID(context);
    FieldChunk_GetLoaderHandle(chunk, &loader);
    switch (loader->state) {
    case FIELD_CHUNK_LOAD_START:
        FieldChunk_ResetStreamer(chunk);
        FieldChunk_GetDatID(chunk, &datId);
        FieldChunk_BeginLoad(chunk, datId);
        loader->state = FIELD_CHUNK_LOAD_READ;
        break;
    case FIELD_CHUNK_LOAD_READ:
        if (FieldChunk_UpdateStreamLoad(chunk)) {
            break;
        }
        loader->unk18 = TRUE;
        loader->unk1c = TRUE;
        loader->terrainLoadDone = TRUE;
        loader->state = FIELD_CHUNK_LOAD_SETUP;
        break;
    case FIELD_CHUNK_LOAD_SETUP: {
        s32 x;
        s32 z;
        s32 blockX;
        s32 blockZ;
        RDChunkFile *file;
        FieldPropSystem *propSystem;
        void *terrain;
        s32 chunkIndex;
        FieldWFBC *wfbc;
        HeapID heapId;

        GetChunkRawDataContainer(chunk, &container);
        file = container;
        FieldChunk_BindModel(chunk, (u8 *)file + file->modelOffset);
        // BUG: propCount is not set if the file has no props, and it is passed on below
#ifdef BUGFIX
        propCount = 0;
#endif
        if (file->propsOffset != file->endOffset) {
            FieldChunkProps *props = (FieldChunkProps *)((u8 *)container + file->propsOffset);

            propCount = FieldPropSystem_InstantiateProps(FieldChunkContext_GetPropSystem(context), chunk,
                                                         (const FieldPropSourceInfo *)(props + 1), props->count);
        }
        propSystem = FieldChunkContext_GetPropSystem(context);
        terrain = (u8 *)container + file->terrainOffset;
        chunkIndex = FieldChunkContext_GetChunkIndex(context);
        wfbc = FieldChunkContext_GetResortMap(context);
        heapId = FieldChunkContext_GetHeapID(context);
        blockX = (chunkIndex % 2) * 4;
        blockZ = (chunkIndex / 2) * 4;
        for (z = 0; z < 4; z++) {
            for (x = 0; x < 4; x++) {
                propCount = func_ov021_02193924(wfbc, terrain, propSystem, chunk, propCount, blockX + x, blockZ + z,
                                                heapId);
            }
        }
        FieldChunk_SetupModel(chunk);
        if (FieldChunkContext_HasAnimator(context)) {
            FieldTerrainSRTAnimatorChunkState_Bind(FieldChunkContext_GetSRTAnimatorState(context),
                                                   FieldChunk_GetModelResource(chunk),
                                                   FieldChunk_GetUsedTexRsc(chunk), FieldChunk_GetModel(chunk));
        }
        loader->state = FIELD_CHUNK_LOAD_END;
        break;
    }
    case FIELD_CHUNK_LOAD_END:
        loader->state = FIELD_CHUNK_LOAD_NONE;
        return FALSE;
    }
    return TRUE;
}

void FieldChunkAccessor_RD_GetTerrain(MapTerrainSamplerOutput *out, const void *container, const VecFx32 *pos,
                                      fx32 size, fx32 baseY) {
    const RDChunkFile *file = container;
    fx32 half = size / 2;
    fx32 z = pos->z + half;
    fx32 x = pos->x + half;
    fx32 tileX = x % TILE_SIZE;
    fx32 tileZ = z % TILE_SIZE;
    const RDTerrain *terrain = (const RDTerrain *)((u8 *)file + file->terrainOffset);
    const RDTile *tile = &terrain->tiles[x / TILE_SIZE + (FX_Whole(size) / 16) * (z / TILE_SIZE)];
    fx32 offset;
    fx32 ny;
    fx32 dz;
    fx32 dx;

    if (tile->diagonal == 0) {
        if (tileX + tileZ < TILE_SIZE) {
            VEC_Fx16Set(&out->layers[0].normal, tile->normal0.x, tile->normal0.y, -tile->normal0.z);
            offset = tile->offset0;
        } else {
            VEC_Fx16Set(&out->layers[0].normal, tile->normal1.x, tile->normal1.y, -tile->normal1.z);
            offset = tile->offset1;
        }
    } else {
        if (tileX > tileZ) {
            VEC_Fx16Set(&out->layers[0].normal, tile->normal0.x, tile->normal0.y, -tile->normal0.z);
            offset = tile->offset0;
        } else {
            VEC_Fx16Set(&out->layers[0].normal, tile->normal1.x, tile->normal1.y, -tile->normal1.z);
            offset = tile->offset1;
        }
    }
    ny = out->layers[0].normal.y;
    dz = FX_MUL(out->layers[0].normal.z, pos->z);
    dx = FX_MUL(out->layers[0].normal.x, pos->x);
    out->layers[0].tileType = tile->tileType;
    out->layers[0].height = FX_Div(-(offset + (dx + dz)), ny) + baseY;
    out->layerCount = 1;
}

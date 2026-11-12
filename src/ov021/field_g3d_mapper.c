// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/field_g3d_mapper.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field.h"
#include "field/field_g3d_mapper.h"
#include "field/field_map.h"
#include "field/field_map_chunk.h"
#include "field/field_prop.h"
#include "field/field_terrain_animator.h"
#include "field/field_wfbc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "system/palanm.h"
#include "system/game_data.h"
#include "system/game_system.h"


void func_ov010_021693cc(FieldChunk *chunk);
void func_ov010_021693bc(FieldChunk *chunk);
void func_ov010_021693b4(FieldChunk *chunk, void *textures);
void func_ov010_021693c4(FieldChunk *chunk, FieldPropResBank *bank);
void func_ov021_0218d104(FieldG3DMapper *mapper, const FieldG3DMapperConfig *config, const u8 *lut);
BOOL FieldChunkLocator_IsActive(FieldChunkLocator *locator);
void func_ov010_02159c00(MapMatrix *matrix, u8 width, u8 height);
void func_ov021_021c0674(void *resortMap);
void func_ov021_0218def0(FieldG3DMapper *mapper);
void func_ov021_0218d05c(FieldG3DMapper *mapper);
void FieldChunkLocator_Disable(FieldChunkLocator *locator);
void *func_ov021_021c05d4(u32 count, u8 heapId);

const FieldChunkVTable FIELD_CHUNK_VTABLES[5] = {
    // "WB", "GC", "NG" and "RD" read as little-endian 16-bit numbers
    {0x4257, FieldChunkAccessor_WB_Update, FieldChunkAccessor_WB_GetTerrain, FieldChunkAccessor_WB_GetTerrain},
    {0x4347, FieldChunkAccessor_GC_Update, FieldChunkAccessor_GC_GetTerrain, FieldChunkAccessor_GC_GetTerrainBaseLayer},
    {0x474e, FieldChunkAccessor_NG_Update, FieldChunkAccessor_NG_GetTerrain, FieldChunkAccessor_NG_GetTerrain},
    {0x4452, FieldChunkAccessor_RD_Update, FieldChunkAccessor_RD_GetTerrain, FieldChunkAccessor_RD_GetTerrain},
    {0xffffffff, FieldChunkAccessor_GC_Update, FieldChunkAccessor_GC_GetTerrain, FieldChunkAccessor_GC_GetTerrain},
};

// The terrain animations of the next mapper that FieldG3DMapper_CreateTerrainAnimator makes
FieldTerrainAnimatorSetup GND_ANIME_WK = {0, 0, 0, 0x45, 0, 0x46, 0};

void FieldG3DMapper_FreeMapTextures(FieldG3DMapper *mapper) {
    if (mapper->mapTextures != NULL) {
        GFL_G3DResFreeTexData(mapper->mapTextures);
        GFL_G3DResFree(mapper->mapTextures);
        mapper->mapTextures = NULL;
    }
}

void FieldG3DMapper_SetPlayerPos(FieldG3DMapper *mapper, const VecFx32 *position) {
    VEC_Set(&mapper->playerPos, position->x, position->y, position->z);
}

// The chunk handle that the player is in
u32 FieldG3DMapper_GetBasePosChunkHandleIdx(FieldG3DMapper *mapper) {
    u32 i;
    VecFx32 pos;

    for (i = 0; i < mapper->chunkCapacity; i++) {
        if (FieldChunk_IsActive(mapper->chunkHandles[i].chunk)) {
            fx32 half;
            fx32 minX;
            fx32 maxX;
            fx32 minZ;
            fx32 maxZ;

            FieldChunk_GetWorldPos(mapper->chunkHandles[i].chunk, &pos);
            half = mapper->chunkSpan / 2;
            minX = pos.x - half;
            minZ = pos.z - half;
            maxX = pos.x + half;
            maxZ = pos.z + half;
            if (mapper->playerPos.x >= minX && mapper->playerPos.x < maxX && mapper->playerPos.z >= minZ
                && mapper->playerPos.z < maxZ) {
                return i;
            }
        }
    }
    return 0;
}

// Every chunk of the matrix, in layers
void FieldG3DMapper_GenLocators0(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators) {
    u32 layers;
    u32 matrixSize;
    fx32 halfSpan;
    u32 layer;
    fx32 y;
    fx32 layerHeight;
    u32 span;
    u32 chunkId;
    s32 i;
    FieldChunkLocator *locator;

    matrixSize = mapper->matrixWidth * mapper->matrixHeight;
    layers = mapper->chunkIdCount / matrixSize;
    if (mapper->chunkIdCount % matrixSize != 0) {
        layers++;
    }
    span = mapper->chunkSpan;
    halfSpan = FX_Div(span, FX32_CONST(2));
    chunkId = 0;
    layerHeight = mapper->unk08;
    for (layer = 0; layer < layers; layer++) {
        for (i = 0; i < matrixSize; i++, chunkId++) {
            s32 x = i % mapper->matrixWidth;
            s32 z = i / mapper->matrixWidth;

            y = FX_Mul(layer << FX32_SHIFT, layerHeight);
            if (chunkId >= mapper->chunkIdCount) {
                chunkId = 0xffffffff;
            }
            locator = &locators[chunkId];
            FieldChunkLocator_SetChunkID(locator, chunkId);
            FieldChunkLocator_SetPos(locator, halfSpan + FX_Mul(x << FX32_SHIFT, span), y,
                                     halfSpan + FX_Mul(z << FX32_SHIFT, span));
        }
    }
}

// The chunks around the player, within the matrix
void FieldG3DMapper_GenLocators1(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators) {
    s32 z;
    u16 width = mapper->matrixWidth;
    s32 height = mapper->matrixHeight;
    u32 span = mapper->chunkSpan;
    fx32 halfSpan = FX_Div(span, FX32_CONST(2));
    s32 startX;
    s32 startZ;
    s32 x;

    if (mapper->chunkLoadDiameterX % 2 == 0) {
        startX = func_ov021_0218d934(mapper->chunkLoadDiameterX, span, playerPos->x);
    } else {
        startX = func_ov021_0218d970(mapper->chunkLoadDiameterX, span, playerPos->x);
    }
    if (mapper->chunkLoadDiameterZ % 2 == 0) {
        startZ = func_ov021_0218d934(mapper->chunkLoadDiameterZ, span, playerPos->z);
    } else {
        startZ = func_ov021_0218d970(mapper->chunkLoadDiameterZ, span, playerPos->z);
    }
    for (z = 0; z < mapper->chunkLoadDiameterZ; z++) {
        s32 chunkZ = startZ + z;

        if (chunkZ < 0 || chunkZ >= height) {
            continue;
        }
        for (x = 0; x < mapper->chunkLoadDiameterX; x++) {
            s32 chunkX = startX + x;
            FieldChunkLocator *locator;

            if (chunkX < 0 || chunkX >= width) {
                continue;
            }
            locator = &locators[x + z * mapper->chunkLoadDiameterX];
            FieldChunkLocator_SetChunkID(locator, chunkX + chunkZ * width);
            FieldChunkLocator_SetPos(locator, halfSpan + FX_Mul(chunkX << FX32_SHIFT, span), 0,
                                     halfSpan + FX_Mul(chunkZ << FX32_SHIFT, span));
        }
    }
}

// The chunks around the player, wrapping around the matrix
void FieldG3DMapper_GenLocators2(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators) {
    u16 width = mapper->matrixWidth;
    u16 height = mapper->matrixHeight;
    u32 span = mapper->chunkSpan;
    fx32 halfSpan = FX_Div(span, FX32_CONST(2));
    s32 z;
    s32 startX = FieldG3DMapper_CalcChunkLoadAreaStart(mapper->chunkLoadDiameterX, span, playerPos->x);
    s32 startZ = FieldG3DMapper_CalcChunkLoadAreaStart(mapper->chunkLoadDiameterZ, span, playerPos->z);
    s32 x;
    s32 chunkX;

    for (z = 0; z < mapper->chunkLoadDiameterZ; z++) {
        s32 chunkZ = startZ + z;
        s32 wrappedZ = chunkZ % height;

        if (wrappedZ < 0) {
            wrappedZ += height;
        }
        for (x = 0; x < mapper->chunkLoadDiameterX; x++) {
            chunkX = startX + x;
            s32 wrappedX = chunkX % width;
            fx32 posZ = halfSpan + FX_Mul(chunkZ << FX32_SHIFT, span);
            FieldChunkLocator *locator;

            if (wrappedX < 0) {
                wrappedX += width;
            }
            locator = &locators[x + z * mapper->chunkLoadDiameterX];
            FieldChunkLocator_SetChunkID(locator, wrappedX + wrappedZ * width);
            FieldChunkLocator_SetPos(locator, halfSpan + FX_Mul(chunkX << FX32_SHIFT, span), 0, posZ);
        }
    }
}

// The player's layer of the matrix and the nearer of the layers above and below it
void FieldG3DMapper_GenLocators3(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators) {
    u32 layer;
    u32 chunksPerLayer;
    fx32 layerHeight;
    fx32 halfSpan;
    u16 width;
    fx32 span;
    s32 adjacentLayerOffset;
    u32 chunkId;
    u32 locatorIndex;
    s32 chunkIndex;
    fx32 layerY;
    int chunkX;
    int chunkZ;

    width = mapper->matrixWidth;
    chunksPerLayer = width * mapper->matrixHeight;
    span = mapper->chunkSpan;
    halfSpan = FX_Div(mapper->chunkSpan, FX32_CONST(2));
    layerHeight = mapper->unk08;
    layer = FX_Div(playerPos->y, layerHeight) >> FX32_SHIFT;
    if (playerPos->y - layer * layerHeight > layerHeight / 2) {
        adjacentLayerOffset = 1;
    } else {
        adjacentLayerOffset = -1;
    }
    locatorIndex = 0;
    for (chunkIndex = 0; chunkIndex < chunksPerLayer; chunkIndex++, locatorIndex++) {
        layerY = FX_Mul(layer << FX32_SHIFT, layerHeight);
        chunkX = chunkIndex % mapper->matrixWidth;
        chunkZ = chunkIndex / mapper->matrixWidth;
        chunkId = layer * chunksPerLayer + chunkZ * width + chunkX;
        if (chunkId >= mapper->chunkIdCount) {
            chunkId = 0xffffffff;
        }
        FieldChunkLocator_SetChunkID(&locators[locatorIndex], chunkId);
        FieldChunkLocator_SetPos(&locators[locatorIndex], halfSpan + FX_Mul(chunkX << FX32_SHIFT, span), layerY,
                                 halfSpan + FX_Mul(chunkZ << FX32_SHIFT, span));
    }
    for (chunkIndex = 0; chunkIndex < chunksPerLayer; chunkIndex++, locatorIndex++) {
        layerY = FX_Mul((layer + adjacentLayerOffset) << FX32_SHIFT, layerHeight);
        chunkX = chunkIndex % mapper->matrixWidth;
        chunkZ = chunkIndex / mapper->matrixWidth;
        chunkId = (layer + adjacentLayerOffset) * chunksPerLayer + chunkZ * width + chunkX;
        if (chunkId >= mapper->chunkIdCount) {
            chunkId = 0xffffffff;
        }
        FieldChunkLocator_SetChunkID(&locators[locatorIndex], chunkId);
        FieldChunkLocator_SetPos(&locators[locatorIndex], halfSpan + FX_Mul(chunkX << FX32_SHIFT, span), layerY,
                                 halfSpan + FX_Mul(chunkZ << FX32_SHIFT, span));
    }
}

// Keeps the loaded chunks that are still wanted, moved to their new places, frees the others, and loads the new ones
// into them. TRUE if a chunk was loaded
BOOL FieldG3DMapper_UpdateChunkLoad(FieldG3DMapper *mapper, FieldChunkLocator *locators) {
    VecFx32 pos;
    VecFx32 newPos;
    BOOL placed;
    BOOL found;
    BOOL loaded;
    BOOL result = FALSE;
    u32 chunkId;
    u32 datId;
    int i;
    int j;

    for (i = 0; i < mapper->chunkCapacity; i++) {
        if (FieldChunkLocator_IsActive(&mapper->chunkHandles[i].locator)) {
            found = FALSE;
            chunkId = FieldChunkLocator_GetChunkID(&mapper->chunkHandles[i].locator);
            for (j = 0; j < mapper->chunkCapacity; j++) {
                if (chunkId == FieldChunkLocator_GetChunkID(&locators[j]) && !found) {
                    FieldChunkLocator_GetPos(&locators[j], &pos);
                    FieldChunk_SetWorldPos(mapper->chunkHandles[i].chunk, &pos);
                    FieldChunkLocator_Disable(&locators[j]);
                    found = TRUE;
                }
            }
            if (!found) {
                FieldChunkLocator_Disable(&mapper->chunkHandles[i].locator);
                FieldChunk_SetActive(mapper->chunkHandles[i].chunk, FALSE);
                FieldPropSystem_UnlinkChunk(mapper->propSystem, mapper->chunkHandles[i].chunk);
                func_ov021_0218dd4c(&mapper->chunkHandles[i].context);
                FieldChunk_Disable(mapper->chunkHandles[i].chunk);
            }
        }
    }
    loaded = FALSE;
    for (i = 0; i < mapper->chunkCapacity; i++) {
        if (FieldChunkLocator_IsActive(&locators[i])) {
            placed = FALSE;
            for (j = 0; j < mapper->chunkCapacity; j++) {
                if (!FieldChunkLocator_IsActive(&mapper->chunkHandles[j].locator) && !placed) {
                    datId = mapper->chunkDatIds[FieldChunkLocator_GetChunkID(&locators[i])];
                    FieldChunkLocator_GetPos(&locators[i], &newPos);
                    if (datId != 0xffffffff) {
                        FieldChunk_ChangeDatID(mapper->chunkHandles[j].chunk, datId);
                        FieldChunk_SetWorldPos(mapper->chunkHandles[j].chunk, &newPos);
                        FieldChunk_SetActive(mapper->chunkHandles[j].chunk, TRUE);
                    }
                    mapper->chunkHandles[j].locator = locators[i];
                    placed = TRUE;
                    loaded = TRUE;
                }
            }
        }
    }
    if (loaded == TRUE) {
        result = TRUE;
    }
    return result;
}

s32 func_ov021_0218d934(u32 diameter, fx32 chunkSpan, fx32 pos) {
    s32 start = -(diameter / 2 - 1);
    s32 chunk = pos / chunkSpan;

    if (pos % chunkSpan < FX_Div(chunkSpan, FX32_CONST(2))) {
        start += chunk - 1;
    } else {
        start += chunk;
    }
    return start;
}

s32 func_ov021_0218d970(u32 diameter, fx32 chunkSpan, fx32 pos) {
    s32 start = -(diameter / 2);

    start += pos / chunkSpan;
    return start;
}

s32 FieldG3DMapper_CalcChunkLoadAreaStart(u32 diameter, fx32 chunkSpan, fx32 pos) {
    s32 start = (FX_Div(pos, chunkSpan) >> FX32_SHIFT) - diameter / 2;

    if (!(diameter & 1)) {
        fx32 remainder = FX_ModS32(pos, chunkSpan);

        if (FX_Div(chunkSpan, FX32_CONST(2)) < remainder) {
            start++;
        }
    }
    return start;
}

void ClearMapTerrainBuf(MapTerrainBuf *terrain) {
    terrain->normal.x = 0;
    terrain->normal.y = 0;
    terrain->normal.z = 0;
    terrain->tileType = 0;
    terrain->height = 0;
}

void ClearMapTerrainSamplerOutput(FieldG3DMapperTerrain *output) {
    int i;

    for (i = 0; i < 16; i++) {
        ClearMapTerrainBuf(&output->layers[i]);
    }
    output->layerCount = 0;
}

// The terrain layers of every chunk at the position
BOOL FieldG3DMapper_GetTerrainAll(FieldG3DMapper *mapper, const VecFx32 *position, FieldG3DMapperTerrain *output) {
    MapTerrainSamplerOutput chunkTerrain;
    VecFx32 chunkPos;
    fx32 half;
    int i;
    u32 count;

    if (mapper->chunkDatIds == NULL) {
        return FALSE;
    }
    ClearMapTerrainSamplerOutput(output);
    count = 0;
    half = mapper->chunkSpan >> 1;
    for (i = 0; i < mapper->chunkCapacity; i++) {
        if (FieldChunk_IsActive(mapper->chunkHandles[i].chunk) == TRUE) {
            fx32 minX;
            fx32 maxX;
            fx32 minZ;
            fx32 maxZ;

            FieldChunk_GetWorldPos(mapper->chunkHandles[i].chunk, &chunkPos);
            minZ = chunkPos.z - half;
            maxX = chunkPos.x + half;
            maxZ = chunkPos.z + half;
            minX = chunkPos.x - half;
            if (position->x >= minX && position->x < maxX && position->z >= minZ && position->z < maxZ) {
                FieldChunk_GetTerrain(&chunkTerrain, mapper->chunkHandles[i].chunk, position, mapper->chunkSpan);
                if (chunkTerrain.layerCount != 0) {
                    u32 j;

                    if (count + chunkTerrain.layerCount >= 16) {
                        break;
                    }
                    for (j = 0; j < chunkTerrain.layerCount; j++) {
                        output->layers[count + j].normal = chunkTerrain.layers[j].normal;
                        output->layers[count + j].tileType = chunkTerrain.layers[j].tileType;
                        output->layers[count + j].height = chunkTerrain.layers[j].height;
                    }
                    count += chunkTerrain.layerCount;
                }
            }
        }
    }
    output->layerCount = count;
    func_ov021_0218ddb4(mapper, position, output);
    if (output->layerCount != 0) {
        return TRUE;
    }
    return FALSE;
}

// The terrain layer nearest to the position's height
BOOL FieldG3DMapper_GetTerrain(FieldG3DMapper *mapper, const VecFx32 *position, MapTerrainBuf *terrain) {
    FieldG3DMapperTerrain all;
    int i;
    int nearest;
    fx32 nearestDistance;

    ClearMapTerrainBuf(terrain);
    if (!FieldG3DMapper_GetTerrainAll(mapper, position, &all)) {
        return FALSE;
    }
    if (all.layerCount == 1) {
        *terrain = all.layers[0];
        return TRUE;
    }
    nearest = 0;
    nearestDistance = 0xfff000;
    for (i = 0; i < all.layerCount; i++) {
        MapTerrainBuf *layer = &all.layers[i];

        if (MapTile_IsValid(layer->tileType)) {
            fx32 distance = layer->height - position->y;

            if (distance < 0) {
                distance = FX_Mul(distance, FX32_CONST(-1));
            }
            if (distance < nearestDistance) {
                nearest = i;
                nearestDistance = distance;
            }
        }
    }
    *terrain = all.layers[nearest];
    return TRUE;
}

BOOL func_ov021_0218dbac(FieldG3DMapper *mapper, u32 index, const VecFx32 *position, MapTerrainBuf *terrain) {
    MapTerrainSamplerOutput chunkTerrain;

    FieldChunk_GetTerrainBaseLayer(&chunkTerrain, mapper->chunkHandles[index].chunk, position, mapper->chunkSpan);
    terrain->tileType = chunkTerrain.layers[0].tileType;
    terrain->height = chunkTerrain.layers[0].height;
    return TRUE;
}

BOOL FieldG3DMapper_CheckChunkHandleTerrainReady(FieldG3DMapper *mapper, u32 index) {
    if (mapper->chunkDatIds == NULL) {
        return FALSE;
    }
    if (FieldChunk_IsActive(mapper->chunkHandles[index].chunk) == FALSE) {
        return FALSE;
    }
    return FieldChunk_IsTerrainReady(mapper->chunkHandles[index].chunk);
}

BOOL FieldG3DMapper_IsPosOutOfBounds(FieldG3DMapper *mapper, const VecFx32 *position) {
    fx32 maxX;
    fx32 maxZ;

    if (mapper->chunkDatIds == NULL) {
        return TRUE;
    }
    if (mapper->locatorGenType == 2) {
        return FALSE;
    }
    maxX = mapper->matrixWidth * mapper->chunkSpan;
    maxZ = mapper->matrixHeight * mapper->chunkSpan;
    if (position->x >= 0 && position->x < maxX && position->z >= 0 && position->z < maxZ) {
        return FALSE;
    }
    return TRUE;
}

void func_ov021_0218dc4c(FieldG3DMapper *mapper, const VecFx32 *position) {
    mapper->chunkBasePos = *position;
}

void func_ov021_0218dc5c(FieldG3DMapper *mapper, VecFx32 *out) {
    *out = mapper->chunkBasePos;
}

// Function name from swan.
FieldPropSystem *FieldG3DMapper_GetBMSystem(FieldG3DMapper *mapper) {
    return mapper->propSystem;
}

FieldWFBC *func_ov021_0218dc70(FieldG3DMapper *mapper) {
    return mapper->wfbc;
}


void func_ov021_0218dc74(FieldG3DMapper *mapper, CityState *city, BOOL isOther) {
    func_ov021_02193b04(mapper->wfbc, city, isOther, mapper->heapId);
}


FieldG3DMapper *func_ov021_0218c9e8(HeapID heapId, u8 season) {
    FieldG3DMapper *mapper = GFL_HeapAllocate(heapId, sizeof(FieldG3DMapper), TRUE, "field_g3d_mapper.c", 0xf9);

    mapper->heapId = heapId;
    mapper->playerPos.x = 0;
    mapper->playerPos.y = 0;
    mapper->playerPos.z = 0;
    mapper->matrixWidth = 0;
    mapper->matrixHeight = 0;
    mapper->chunkIdCount = 0;
    mapper->chunkDatIds = NULL;
    mapper->chunkSpan = 0;
    mapper->unk08 = 0;
    mapper->locatorGenType = 1;
    mapper->chunkArcId = -1;
    mapper->phase1MaxChunkCount = 0;
    mapper->terrainAnimator = NULL;
    mapper->propSystem = FieldPropSystem_Create(mapper->heapId, mapper, season);
    mapper->wfbc = func_ov021_021938ac(heapId);
    mapper->resortMap = func_ov021_021c05d4(0x10, heapId);
    return mapper;
}

void func_ov021_0218ca4c(FieldG3DMapper *mapper) {
    func_ov021_021c0674(mapper->resortMap);
    func_ov021_0218d05c(mapper);
    func_ov021_021938d4(mapper->wfbc);
    if (mapper->propSystem != NULL) {
        FieldPropSystem_Free(mapper->propSystem);
        mapper->propSystem = NULL;
    }
    GFL_HeapFree(mapper);
}

void func_ov021_0218ca78(FieldG3DMapper *mapper) {
    if (mapper->chunkDatIds != NULL) {
        s32 i;

        for (i = 0; i < mapper->chunkCapacity; i++) {
            FieldChunkLocator_Disable(&mapper->chunkLocators[i]);
        }
        switch (mapper->locatorGenType) {
        case 0:
            FieldG3DMapper_GenLocators0(mapper, &mapper->playerPos, mapper->chunkLocators);
            break;
        default:
        case 1:
            FieldG3DMapper_GenLocators1(mapper, &mapper->playerPos, mapper->chunkLocators);
            break;
        case 2:
            FieldG3DMapper_GenLocators2(mapper, &mapper->playerPos, mapper->chunkLocators);
            break;
        case 3:
            FieldG3DMapper_GenLocators3(mapper, &mapper->playerPos, mapper->chunkLocators);
            break;
        }
        FieldG3DMapper_UpdateChunkLoad(mapper, mapper->chunkLocators);
        for (i = 0; i < mapper->chunkCapacity; i++) {
            FieldChunk_LoadFullSync(mapper->chunkHandles[i].chunk);
        }
        {
            fx32 x = FX_Div(mapper->playerPos.x, mapper->chunkSpan);
            fx32 z = FX_Div(mapper->playerPos.z, mapper->chunkSpan);
            mapper->chunkIdOfPlayer = (x >> 12) + (z >> 12) * mapper->matrixWidth;
        }
    }
}

BOOL func_ov021_0218cb34(FieldG3DMapper *mapper) {
    BOOL found;
    s32 i;
    FieldChunkLoader *loader;

    if (mapper->chunkDatIds == NULL) {
        return FALSE;
    }
    for (i = 0; i < mapper->chunkCapacity; i++) {
        FieldChunkLocator_Disable(&mapper->chunkLocators[i]);
    }
    switch (mapper->locatorGenType) {
    case 0:
        FieldG3DMapper_GenLocators0(mapper, &mapper->playerPos, mapper->chunkLocators);
        break;
    default:
    case 1:
        FieldG3DMapper_GenLocators1(mapper, &mapper->playerPos, mapper->chunkLocators);
        break;
    case 2:
        FieldG3DMapper_GenLocators2(mapper, &mapper->playerPos, mapper->chunkLocators);
        break;
    case 3:
        FieldG3DMapper_GenLocators3(mapper, &mapper->playerPos, mapper->chunkLocators);
        break;
    }
    FieldG3DMapper_UpdateChunkLoad(mapper, mapper->chunkLocators);
    found = FALSE;
    FieldG3DMapper_ResetDrawCalcState(mapper);
    for (i = 0; i < mapper->chunkCapacity; i++) {
        FieldChunk_GetLoaderHandle(mapper->chunkHandles[i].chunk, &loader);
        if (loader->state == 1) {
            found = TRUE;
        }
        FieldChunk_CallLoaderUpdate(mapper->chunkHandles[i].chunk);
        FieldG3DMapper_AddDrawableChunk(mapper, mapper->chunkHandles[i].chunk, i);
    }
    FieldG3DMapper_CalcRenderPassSplit(mapper);
    {
        fx32 x = FX_Div(mapper->playerPos.x, mapper->chunkSpan);
        fx32 z = FX_Div(mapper->playerPos.z, mapper->chunkSpan);
        mapper->chunkIdOfPlayer = (x >> 12) + (z >> 12) * mapper->matrixWidth;
    }
    FieldPropSystem_Update(mapper->propSystem);
    if (mapper->terrainAnimator != NULL) {
        FieldTerrainAnimator_Update(mapper->terrainAnimator);
    }
    return found;
}


void func_ov021_0218cc34(FieldG3DMapper *mapper) {
    if (mapper->chunkDatIds != NULL) {
        s32 i;

        for (i = 0; i < mapper->chunkCapacity; i++) {
            FieldChunk_CallLoaderUpdate(mapper->chunkHandles[i].chunk);
        }
        func_ov021_0218def0(mapper);
    }
}

void func_ov021_0218cc64(FieldG3DMapper *mapper, void *camera, s32 pass) {
    if (pass != 0 || mapper->phase1MaxChunkCount != 0) {
        s32 i;

        for (i = 0; i < mapper->chunkCapacity; i++) {
            u32 chunkIdx;

            if (FieldG3DMapper_CheckChunkDraw(mapper, i, pass, &chunkIdx)) {
                VecFx32 pos;
                VecFx32 drawPos;
                BOOL drawn;

                FieldChunk_GetWorldPos(mapper->chunkHandles[chunkIdx].chunk, &pos);
                drawPos.x = pos.x + mapper->chunkBasePos.x;
                drawPos.y = pos.y + mapper->chunkBasePos.y;
                drawPos.z = pos.z + mapper->chunkBasePos.z;
                FieldChunk_SetWorldPos(mapper->chunkHandles[chunkIdx].chunk, &drawPos);
                drawn = FieldChunk_Draw(mapper->chunkHandles[chunkIdx].chunk, camera);
                FieldChunk_SetWorldPos(mapper->chunkHandles[chunkIdx].chunk, &pos);
                if (drawn && mapper->nowDrawnCount < mapper->chunkCapacity) {
                    mapper->nowDrawnCount++;
                }
            }
        }
        if (pass == 1) {
            FieldPropSystem_DrawAllHandles(mapper->propSystem);
        }
    }
}

BOOL func_ov021_0218cd20(Field *field, FieldG3DMapper *mapper, MapMatrix *matrix) {
    u32 *chunkIds;
    u32 newChunkIdCount;
    u32 *ptr;
    u32 chunkId;
    u32 matrixWidth;
    u32 newHeight;
    s32 height;
    u16 newWidth;
    u32 x;
    u32 y;
    u32 idCount;
    s32 width;
    s32 row;
    s32 col;
    s32 i;

    width = GetMapMatrixWidth(matrix);
    height = GetMapMatrixHeight(matrix);
    idCount = GetMapMatrixChunkIDCount(matrix);
    chunkIds = GetMapMatrixChunkIDs(matrix);
    newWidth = mapper->matrixWidth + width;
    newHeight = mapper->matrixHeight;
    newChunkIdCount = mapper->chunkIdCount + idCount;
    if (newHeight != height) {
        return FALSE;
    }
    if (newChunkIdCount > 900 || newWidth > 30 || newHeight > 30) {
        return FALSE;
    }
    ptr = GFL_HeapAllocate((mapper->heapId & 0x7fff) | 0x8000, newChunkIdCount * 4, FALSE, "field_g3d_mapper.c", 0x25f);

    for (row = 0; row < mapper->matrixHeight; row++) {
        for (col = 0; col < mapper->matrixWidth; col++) {
            ptr[row * newWidth + col] = mapper->chunkDatIds[mapper->matrixWidth * row + col];
        }
    }
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            ptr[row * newWidth + mapper->matrixWidth + col] = chunkIds[row * width + col];
        }
    }
    for (i = 0; i < newChunkIdCount; i++) {
        chunkId = ptr[i];
        mapper->chunkDatIds[i] = chunkId;
    }

    matrixWidth = mapper->matrixWidth;
    x = mapper->chunkIdOfPlayer % matrixWidth;
    y = mapper->chunkIdOfPlayer / matrixWidth;
    mapper->chunkIdOfPlayer = x + newWidth * y;
    for (i = 0; i < mapper->chunkCapacity; i++) {
        if (FieldChunkLocator_IsActive(&mapper->chunkHandles[i].locator)) {
            matrixWidth = mapper->matrixWidth;
            chunkId = mapper->chunkHandles[i].locator.chunkId;
            x = chunkId % matrixWidth;
            y = chunkId / matrixWidth;
            mapper->chunkHandles[i].locator.chunkId = x + newWidth * y;
        }
    }
    mapper->matrixWidth = newWidth;
    mapper->matrixHeight = newHeight;
    mapper->chunkIdCount = newChunkIdCount;
    func_ov010_02159c00(GetMapMatrixSystem(GSYS_GetGameData(Field_GetGameSystem(field))), newWidth, newHeight);
    GFL_HeapFree(ptr);
    return TRUE;
}


ResortMapCreateWork *func_ov021_0218cee0(FieldG3DMapper *mapper) {
    return mapper->resortMap;
}

void func_ov021_0218cee4(FieldG3DMapper *mapper, const FieldG3DMapperConfig *config, const u8 *lut) {
    FieldChunkConfig chunkConfig;
    u32 capacity;
    s32 i;

    mapper->chunkSpan = config->chunkSpan;
    mapper->unk08 = config->unk04;
    mapper->chunkArcId = config->chunkArcId;
    mapper->matrixWidth = config->matrixWidth;
    mapper->chunkIdCount = config->chunkIdCount;
    mapper->locatorGenType = config->locatorGenType;
    mapper->matrixHeight = config->matrixHeight;
    mapper->chunkDatIds = config->chunkDatIds;
    mapper->chunkLoadDiameterX = config->chunkLoadDiameterX;
    mapper->chunkLoadDiameterZ = config->chunkLoadDiameterZ;
    mapper->chunkCapacity = config->chunkLoadDiameterX * config->chunkLoadDiameterZ;
    mapper->phase1MaxChunkCount = config->renderPhase1MaxChunkCount;
    capacity = mapper->chunkCapacity;
    mapper->lastDrawnCount = (u8)capacity;
    mapper->nowDrawnCount = (u8)capacity;
    switch (config->locatorGenType) {
    case 0:
        if (config->chunkIdCount > capacity) {
            mapper->locatorGenType = 1;
        }
        break;
    case 3: {
        s32 half = capacity >> 1;

        if (mapper->matrixWidth * mapper->matrixHeight > half) {
            mapper->locatorGenType = 1;
        }
        break;
    }
    }
    mapper->isSharedTexResource = config->isSharedTexResource;
    func_ov021_0218d104(mapper, config, lut);
    mapper->terrainAnimator = FieldG3DMapper_CreateTerrainAnimator(mapper->chunkCapacity, mapper->mapTextures,
        &config->terrainAnmInfo, mapper->heapId);

    chunkConfig.bufferSize = config->chunkBufferSize;
    chunkConfig.textureVRAMSize = 0;
    chunkConfig.vtable = FIELD_CHUNK_VTABLES;
    chunkConfig.context = NULL;
    chunkConfig.streamerBufSize = 0x800;
    chunkConfig.propCapacity1 = 0x20;
    chunkConfig.propCapacity2 = 0;
    chunkConfig.unkA8 = 0x1000000;
    chunkConfig.unkAC = 0x400000;

    mapper->chunkHandles = GFL_HeapAllocate(mapper->heapId, mapper->chunkCapacity * sizeof(FieldChunkHandle), TRUE,
        "field_g3d_mapper.c", 0x306);
    mapper->chunkLocators = GFL_HeapAllocate(mapper->heapId, mapper->chunkCapacity * sizeof(FieldChunkLocator), TRUE,
        "field_g3d_mapper.c", 0x307);
    mapper->drawnChunkIndices = GFL_HeapAllocate(mapper->heapId, mapper->chunkCapacity, TRUE, "field_g3d_mapper.c", 0x308);

    for (i = 0; i < mapper->chunkCapacity; i++) {
        FieldChunk *chunk;

        FieldChunkLocator_Disable(&mapper->chunkHandles[i].locator);
        FieldChunkContext_Init(&mapper->chunkHandles[i].context, mapper, i, mapper->heapId);
        chunkConfig.context = &mapper->chunkHandles[i].context;
        chunk = FieldChunk_Create(&chunkConfig, mapper->heapId);
        mapper->chunkHandles[i].chunk = chunk;
        FieldChunk_BindArc(chunk, mapper->chunkArcId, mapper->heapId);
        if (mapper->mapTextures != NULL) {
            func_ov010_021693b4(chunk, mapper->mapTextures);
        }
        func_ov010_021693c4(chunk, FieldPropSystem_GetResBank(mapper->propSystem));
    }
}


void func_ov021_0218d05c(FieldG3DMapper *mapper) {
    if (mapper->chunkHandles != NULL) {
        s32 i;

        for (i = 0; i < mapper->chunkCapacity; i++) {
            func_ov010_021693cc(mapper->chunkHandles[i].chunk);
            func_ov010_021693bc(mapper->chunkHandles[i].chunk);
            FieldChunk_UnbindArc(mapper->chunkHandles[i].chunk);
            FieldChunk_Free(mapper->chunkHandles[i].chunk);
            FieldChunkContext_FreeAnimator(&mapper->chunkHandles[i].context);
        }
        GFL_HeapFree(mapper->chunkHandles);
        GFL_HeapFree(mapper->chunkLocators);
        GFL_HeapFree(mapper->drawnChunkIndices);
        mapper->chunkLocators = NULL;
        mapper->chunkHandles = NULL;
        mapper->drawnChunkIndices = NULL;
        mapper->chunkCapacity = 0;
    }
    if (mapper->terrainAnimator != NULL) {
        FieldTerrainAnimator_Free(mapper->terrainAnimator);
        mapper->terrainAnimator = NULL;
    }
    FieldG3DMapper_FreeMapTextures(mapper);
}


BOOL func_ov021_0218d0d8(void *resource, const u8 *lut) {
    NNSG3dResFileHeader *header = GFL_G3DResGetResData(resource);
    NNSG3dResTex *tex = NNS_G3dGetTex(header);

    if (tex != NULL) {
        u32 size = tex->plttInfo.sizePltt << 3;
        u32 offset = tex->plttInfo.ofsPlttData;

        if (lut != NULL) {
            ColorFilter_ApplyLUT((u16 *)((u8 *)tex + offset), size / 2, lut);
        }
        return TRUE;
    }
    return FALSE;
}

void func_ov021_0218d104(FieldG3DMapper *mapper, const FieldG3DMapperConfig *config, const u8 *lut) {
    if (config->isSharedTexResource != 0) {
        if (config->isSharedTexResource == 1) {
            const u32 *ids = &config->textures.arcId;
            mapper->mapTextures = GFL_G3DSysReadArcSysResource(ids[0], ids[1]);
            if (lut != NULL) {
                func_ov021_0218d0d8(mapper->mapTextures, lut);
            }
            GFL_G3DResUploadAndReleaseTexData(mapper->mapTextures);
        }
    } else {
        mapper->mapTextures = NULL;
    }
}


FieldTerrainAnimator *FieldG3DMapper_CreateTerrainAnimator(u16 chunkCapacity, void *mapTextures,
                                                           const FieldTerrainAnmInfo *anmInfo, u32 heapId) {
    GND_ANIME_WK.chunkCapacity = chunkCapacity;
    if (anmInfo->srtAnimeId == 0xffffffff) {
        GND_ANIME_WK.hasSRT = FALSE;
    } else {
        GND_ANIME_WK.hasSRT = TRUE;
        GND_ANIME_WK.idSRT = anmInfo->srtAnimeId;
    }
    if (anmInfo->patAnimeId == 0xffffffff) {
        GND_ANIME_WK.hasPat = FALSE;
    } else {
        GND_ANIME_WK.hasPat = TRUE;
        GND_ANIME_WK.idPat = anmInfo->patAnimeId;
    }
    return FieldTerrainAnimator_Create(&GND_ANIME_WK, mapTextures, heapId);
}

BOOL FieldChunkContext_HasAnimator(FieldChunkContext *context) {
    return FieldChunkContext_HasAnimatorCore(context);
}

FieldTerrainSRTAnimatorChunkState *FieldChunkContext_GetSRTAnimatorState(FieldChunkContext *context) {
    return FieldChunkContext_GetSRTAnimatorStateCore(context);
}

FieldPropSystem *FieldChunkContext_GetPropSystem(FieldChunkContext *context) {
    return context->propSystem;
}

FieldWFBC *FieldChunkContext_GetResortMap(FieldChunkContext *context) {
    return context->wfbc;
}

u32 FieldChunkContext_GetChunkIndex(FieldChunkContext *context) {
    return context->chunkIndex;
}

HeapID FieldChunkContext_GetHeapID(FieldChunkContext *context) {
    return context->heapId;
}

void FieldChunkContext_Init(FieldChunkContext *context, FieldG3DMapper *mapper, u32 chunkIndex, HeapID heapId) {
    context->srtAnimatorState = FieldTerrainAnimator_GetChunkState(mapper->terrainAnimator, chunkIndex);
    context->propSystem = mapper->propSystem;
    context->wfbc = mapper->wfbc;
    context->chunkIndex = chunkIndex;
    context->heapId = heapId;
}

void FieldChunkContext_FreeAnimator(FieldChunkContext *context) {
    if (context->srtAnimatorState != NULL) {
        FieldTerrainSRTAnimatorChunkState_ClearUsedFlag(context->srtAnimatorState);
        context->srtAnimatorState = NULL;
    }
}

void func_ov021_0218dd4c(FieldChunkContext *context) {
    if (context->srtAnimatorState != NULL) {
        FieldTerrainSRTAnimatorChunkState_ClearUsedFlag(context->srtAnimatorState);
    }
}

BOOL FieldChunkContext_HasAnimatorCore(FieldChunkContext *context) {
    if (context->srtAnimatorState != NULL) {
        return TRUE;
    }
    return FALSE;
}

FieldTerrainSRTAnimatorChunkState *FieldChunkContext_GetSRTAnimatorStateCore(FieldChunkContext *context) {
    return context->srtAnimatorState;
}

void FieldChunkLocator_Disable(FieldChunkLocator *locator) {
    locator->chunkId = 0xffffffff;
    VEC_Set(&locator->position, 0, 0, 0);
}

void FieldChunkLocator_SetChunkID(FieldChunkLocator *locator, u32 chunkId) {
    locator->chunkId = chunkId;
}

BOOL FieldChunkLocator_IsActive(FieldChunkLocator *locator) {
    if (locator->chunkId != 0xffffffff) {
        return TRUE;
    }
    return FALSE;
}

u32 FieldChunkLocator_GetChunkID(FieldChunkLocator *locator) {
    return locator->chunkId;
}

void FieldChunkLocator_SetPos(FieldChunkLocator *locator, fx32 x, fx32 y, fx32 z) {
    VEC_Set(&locator->position, x, y, z);
}

void FieldChunkLocator_GetPos(FieldChunkLocator *locator, VecFx32 *position) {
    *position = locator->position;
}

// Adds the terrain layers of the map's height areas at the position
void func_ov021_0218ddb4(const FieldG3DMapper *mapper, const VecFx32 *position, FieldG3DMapperTerrain *output) {
    VecFx16 normal = {0, FX16_ONE, 0};
    s32 added = 0;
    u32 count = output->layerCount;
    s32 areaCount = func_ov021_021c0688(mapper->resortMap);
    s32 z;
    s32 x;
    s32 i;

    x = position->x / FX32_CONST(16);
    z = position->z / FX32_CONST(16);

    for (i = 0; i < areaCount; i++) {
        if (func_ov021_021c068c(x, z, mapper->resortMap, i)) {
            s32 index;
            MapTerrainBuf *layer;

            added++;
            index = count + added - 1;
            if (index >= 16) {
                break;
            }
            layer = &output->layers[index];
            layer->normal = normal;
            layer->tileType = func_ov021_021c06b4(i, mapper->resortMap);
            layer->height = func_ov021_021c06a8(i, mapper->resortMap);
        }
    }
    output->layerCount += (u16)added;
}

static const u16 data_ov021_021d3dd4[4] = { 0, 0x1000, 0, 0 };

void FieldG3DMapper_ResetDrawCalcState(FieldG3DMapper *mapper) {
    s32 elapsed;
    s32 vcount;

    MI_CpuFill8(mapper->drawnChunkIndices, 0xff, mapper->chunkCapacity);
    mapper->drawnChunkCount = 0;
    mapper->lastDrawnCount = mapper->nowDrawnCount;
    mapper->nowDrawnCount = 0;
    mapper->chunkSizeCapPassingChunkEndIdx = 0;
    elapsed = OS_GetVBlankCount() - mapper->unk50;
    if (elapsed >= 2) {
        mapper->nowDrawScanline = 0xff;
        mapper->maxConcurrentDrawChunkByteSize = 0;
        return;
    }
    vcount = GX_GetVCount();
    mapper->nowDrawScanline = vcount;
    if (vcount > 192 || vcount <= 10) {
        mapper->maxConcurrentDrawChunkByteSize = 0xf000;
        return;
    }
    mapper->maxConcurrentDrawChunkByteSize = 0xf000 - ((vcount - 10) << 10);
}

void func_ov021_0218def0(FieldG3DMapper *mapper) {
    mapper->unk50 = OS_GetVBlankCount();
}

// Inserts the chunk in the drawing order by the room its data leaves
void FieldG3DMapper_AddDrawableChunk(FieldG3DMapper *mapper, FieldChunk *chunk, u32 index) {
    u32 size;
    u32 otherSize;
    s32 margin;
    int i;

    if (FieldChunk_GetModel(chunk) != NULL && FieldChunk_IsActive(chunk)) {
        FieldChunk_GetRawDataLength(chunk, &size);
        margin = mapper->maxConcurrentDrawChunkByteSize - size;
        for (i = mapper->drawnChunkCount; i > 0; i--) {
            s32 otherMargin;

            FieldChunk_GetRawDataLength(mapper->chunkHandles[mapper->drawnChunkIndices[i - 1]].chunk, &otherSize);
            otherMargin = mapper->maxConcurrentDrawChunkByteSize - otherSize;
            if (otherMargin >= 0 && margin >= otherMargin) {
                break;
            }
            if (margin < 0) {
                break;
            }
            mapper->drawnChunkIndices[i] = mapper->drawnChunkIndices[i - 1];
        }
        mapper->drawnChunkIndices[i] = index;
        mapper->drawnChunkCount++;
    }
}

void FieldG3DMapper_CalcRenderPassSplit(FieldG3DMapper *mapper) {
    s32 total = 0;
    int count;
    int i;

    mapper->chunkSizeCapPassingChunkEndIdx = 0;
    count = mapper->lastDrawnCount - (mapper->chunkCapacity - mapper->phase1MaxChunkCount);
    if (count > 0) {
        for (i = 0; i < count; i++) {
            u8 index = mapper->drawnChunkIndices[i];
            u32 size;

            if (index == 0xff) {
                break;
            }
            FieldChunk_GetRawDataLength(mapper->chunkHandles[index].chunk, &size);
            total += size;
            if (mapper->maxConcurrentDrawChunkByteSize < total) {
                break;
            }
            mapper->chunkSizeCapPassingChunkEndIdx++;
        }
    }
}

BOOL FieldG3DMapper_CheckChunkDraw(FieldG3DMapper *mapper, u32 order, u32 pass, u32 *index) {
    u8 chunkIndex = mapper->drawnChunkIndices[order];

    if (chunkIndex == 0xff) {
        return FALSE;
    }
    *index = chunkIndex;
    if (pass == 0) {
        if (order < mapper->chunkSizeCapPassingChunkEndIdx) {
            return TRUE;
        }
    } else {
        if (order >= mapper->chunkSizeCapPassingChunkEndIdx) {
            return TRUE;
        }
    }
    return FALSE;
}

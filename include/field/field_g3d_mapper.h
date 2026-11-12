// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_g3d_mapper.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_FIELD_G3D_MAPPER_H
#define BWPORT_FIELD_FIELD_G3D_MAPPER_H

// Overlay 36's field_g3d_mapper.c (overlay 21's in Black), which streams the map chunks around the player and draws
// them. Names and layouts from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "field/field_terrain_animator.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/field_map.h"
#include "field/field_map_chunk.h"

BOOL FieldChunkContext_HasAnimator(FieldChunkContext *context);
FieldTerrainSRTAnimatorChunkState *FieldChunkContext_GetSRTAnimatorState(FieldChunkContext *context);
FieldPropSystem *FieldChunkContext_GetPropSystem(FieldChunkContext *context);
// The mapper's White Forest/Black City map (Black 2 has its Join Avenue's map in this place)
FieldWFBC *FieldChunkContext_GetResortMap(FieldChunkContext *context);
u32 FieldChunkContext_GetChunkIndex(FieldChunkContext *context);
HeapID FieldChunkContext_GetHeapID(FieldChunkContext *context);

FieldPropSystem *FieldG3DMapper_GetBMSystem(FieldG3DMapper *mapper);
// The mapper's White Forest/Black City map
FieldWFBC *func_ov021_0218dc70(FieldG3DMapper *mapper);
BOOL FieldG3DMapper_GetTerrain(FieldG3DMapper *mapper, const VecFx32 *position, MapTerrainBuf *terrain);

void ClearMapTerrainBuf(MapTerrainBuf *terrain);
BOOL FieldG3DMapper_IsPosOutOfBounds(FieldG3DMapper *mapper, const VecFx32 *position);

void func_ov021_0218dc4c(FieldG3DMapper *mapper, const VecFx32 *position);

// An archive and a file in it
struct FieldG3DMapperTextureFile {
    u32 arcId;
    u32 fileId;
};

// Black 2's; twinned code confirms the offsets of matrixWidth, matrixHeight, chunkIdCount, chunkDatIds,
// isSharedTexResource, textures and terrainAnmInfo in Black
struct FieldG3DMapperConfig {
    fx32 chunkSpan;
    u32 unk04;
    u16 chunkLoadDiameterX;
    u16 chunkLoadDiameterZ;
    u32 locatorGenType;
    u32 chunkArcId;
    u16 matrixWidth;
    u16 matrixHeight;
    u32 chunkIdCount;
    u32 *chunkDatIds;
    BOOL isSharedTexResource;
    // The textures that all the chunks use, if isSharedTexResource is 1
    FieldG3DMapperTextureFile textures;
    FieldTerrainAnmInfo terrainAnmInfo;
    u32 chunkBufferSize;
    u8 renderPhase1MaxChunkCount;
};

// The terrain layers at a position, which the mapper gathers from its chunks
struct FieldG3DMapperTerrain {
    MapTerrainBuf layers[16];
    u16 layerCount;
};

struct FieldChunkLocator {
    u32 chunkId;
    VecFx32 position;
};

// Black 2's, not checked against Black
struct FieldChunkHandle {
    FieldChunk *chunk;
    FieldChunkContext context;
    FieldChunkLocator locator;
};

// Black 2's; twinned code confirms the offsets of chunkSpan, locatorGenType, matrixWidth, matrixHeight, chunkDatIds,
// chunkCapacity, drawnChunkCount, lastDrawnCount, nowDrawnCount, drawnChunkIndices, maxConcurrentDrawChunkByteSize,
// chunkSizeCapPassingChunkEndIdx, nowDrawScanline, unk50, chunkBasePos, mapTextures and propSystem in Black
struct FieldG3DMapper {
    u16 heapId;
    fx32 chunkSpan;
    u32 unk08;
    u32 locatorGenType;
    u32 chunkArcId;
    u16 matrixWidth;
    u16 matrixHeight;
    u32 chunkIdCount;
    u32 *chunkDatIds;
    BOOL isSharedTexResource;
    FieldChunkHandle *chunkHandles;
    FieldChunkLocator *chunkLocators;
    u8 chunkLoadDiameterX;
    u8 chunkLoadDiameterZ;
    u16 chunkCapacity;
    u32 chunkIdOfPlayer;
    VecFx32 playerPos;
    u8 phase1MaxChunkCount;
    u8 drawnChunkCount;
    u8 lastDrawnCount;
    u8 nowDrawnCount;
    // The chunk handles in drawing order, 0xff after the last
    u8 *drawnChunkIndices;
    s32 maxConcurrentDrawChunkByteSize;
    u8 chunkSizeCapPassingChunkEndIdx;
    u8 nowDrawScanline;
    u16 unk4E;
    u32 unk50;
    VecFx32 chunkBasePos;
    void *mapTextures;
    FieldPropSystem *propSystem;
    FieldTerrainAnimator *terrainAnimator;
    FieldWFBC *wfbc;
    ResortMapCreateWork *resortMap;
};

void FieldG3DMapper_FreeMapTextures(FieldG3DMapper *mapper);
void FieldG3DMapper_SetPlayerPos(FieldG3DMapper *mapper, const VecFx32 *position);
s32 FieldG3DMapper_CalcChunkLoadAreaStart(u32 diameter, fx32 chunkSpan, fx32 pos);
void ClearMapTerrainSamplerOutput(FieldG3DMapperTerrain *output);
FieldTerrainAnimator *FieldG3DMapper_CreateTerrainAnimator(u16 chunkCapacity, void *mapTextures,
                                                           const FieldTerrainAnmInfo *anmInfo, u32 heapId);
BOOL FieldChunkContext_HasAnimatorCore(FieldChunkContext *context);
void FieldChunkLocator_Disable(FieldChunkLocator *locator);
BOOL FieldChunkLocator_IsActive(FieldChunkLocator *locator);
void FieldChunkLocator_SetPos(FieldChunkLocator *locator, fx32 x, fx32 y, fx32 z);
void FieldG3DMapper_ResetDrawCalcState(FieldG3DMapper *mapper);
BOOL FieldG3DMapper_CheckChunkDraw(FieldG3DMapper *mapper, u32 order, u32 pass, u32 *index);
s32 func_ov021_0218d934(u32 diameter, fx32 chunkSpan, fx32 pos);
s32 func_ov021_0218d970(u32 diameter, fx32 chunkSpan, fx32 pos);
void func_ov021_0218dc5c(FieldG3DMapper *mapper, VecFx32 *out);
void func_ov021_0218dc74(FieldG3DMapper *mapper, CityState *city, BOOL isOther);
ResortMapCreateWork *func_ov021_0218cee0(FieldG3DMapper *mapper);
void func_ov021_0218dd4c(FieldChunkContext *context);
void func_ov021_0218def0(FieldG3DMapper *mapper);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL FieldG3DMapper_GetTerrainAll(FieldG3DMapper *mapper, const VecFx32 *position, FieldG3DMapperTerrain *output);
u32 FieldG3DMapper_GetBasePosChunkHandleIdx(FieldG3DMapper *mapper);
void FieldG3DMapper_GenLocators0(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators);
void FieldG3DMapper_GenLocators1(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators);
void FieldG3DMapper_GenLocators2(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators);
void FieldG3DMapper_GenLocators3(FieldG3DMapper *mapper, const VecFx32 *playerPos, FieldChunkLocator *locators);
BOOL FieldG3DMapper_UpdateChunkLoad(FieldG3DMapper *mapper, FieldChunkLocator *locators);
BOOL FieldG3DMapper_CheckChunkHandleTerrainReady(FieldG3DMapper *mapper, u32 index);
void FieldChunkContext_Init(FieldChunkContext *context, FieldG3DMapper *mapper, u32 chunkIndex, HeapID heapId);
void FieldChunkContext_FreeAnimator(FieldChunkContext *context);
FieldTerrainSRTAnimatorChunkState *FieldChunkContext_GetSRTAnimatorStateCore(FieldChunkContext *context);
void FieldChunkLocator_SetChunkID(FieldChunkLocator *locator, u32 chunkId);
u32 FieldChunkLocator_GetChunkID(FieldChunkLocator *locator);
void FieldChunkLocator_GetPos(FieldChunkLocator *locator, VecFx32 *position);
void FieldG3DMapper_AddDrawableChunk(FieldG3DMapper *mapper, FieldChunk *chunk, u32 index);
void FieldG3DMapper_CalcRenderPassSplit(FieldG3DMapper *mapper);
BOOL func_ov021_0218dbac(FieldG3DMapper *mapper, u32 index, const VecFx32 *position, MapTerrainBuf *terrain);
void func_ov021_0218ddb4(const FieldG3DMapper *mapper, const VecFx32 *position, FieldG3DMapperTerrain *output);

BOOL func_ov021_0218cd20(Field *field, FieldG3DMapper *mapper, MapMatrix *matrix);

#endif // BWPORT_FIELD_FIELD_G3D_MAPPER_H

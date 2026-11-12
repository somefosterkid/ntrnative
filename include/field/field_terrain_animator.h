// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_terrain_animator.h, the lines
// in use so far.
#ifndef BWPORT_FIELD_FIELD_TERRAIN_ANIMATOR_H
#define BWPORT_FIELD_FIELD_TERRAIN_ANIMATOR_H

// Overlay 36's animations of the map chunks' textures (overlay 21's in Black). Names and layouts from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

typedef struct FieldTerrainSRTAnimatorChunkState FieldTerrainSRTAnimatorChunkState;

// Binds the chunk's SRT animation to its model and textures, in the render object
void FieldTerrainSRTAnimatorChunkState_Bind(FieldTerrainSRTAnimatorChunkState *state, void *model, void *textures,
                                            NNSG3dRenderObj *renderObj);

typedef struct FieldTerrainAnimator FieldTerrainAnimator;

FieldTerrainAnimator *FieldTerrainAnimator_Create(const FieldTerrainAnimatorSetup *setup, void *mapTextures,
                                                  HeapID heapId);

struct FieldTerrainAnimatorSetup {
    u16 hasSRT : 8;
    u16 hasPat : 8;
    u16 chunkCapacity;
    u16 arcIdSRT;
    u16 idSRT;
    u16 arcIdPat;
    u16 idPat;
};

void FieldTerrainAnimator_Free(FieldTerrainAnimator *animator);
void FieldTerrainAnimator_Update(FieldTerrainAnimator *animator);
FieldTerrainSRTAnimatorChunkState *FieldTerrainAnimator_GetChunkState(FieldTerrainAnimator *animator, u32 chunkIndex);

struct FieldTerrainAnmInfo {
    u32 srtAnimeId;
    u32 patAnimeId;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldTerrainSRTAnimatorChunkState_ClearUsedFlag(FieldTerrainSRTAnimatorChunkState *state);

#endif // BWPORT_FIELD_FIELD_TERRAIN_ANIMATOR_H

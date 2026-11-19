#ifndef POKEHEARTGOLD_OVERLAY_01_021FD1B8_H
#define POKEHEARTGOLD_OVERLAY_01_021FD1B8_H

#include "global.h"
#include "simple3dmodel.h"

struct FieldEffectManager;

void *ov01_021FD1B8(struct FieldEffectManager *fieldEffMan);

// Renderer 0x14's cache slot: the model of one id (unk_00 == 0xFFFF: free).
typedef struct UnkStruct_021F1118 {
    u32 unk_00;
    Simple3DModel unk_04;
    Simple3DRenderObj unk_18;
} UnkStruct_021F1118;

// Renderer 0x14's context (ov01_021FD1B8): `unk_04` slots in the heap array `unk_08`.
typedef struct UnkStruct_021F114C {
    struct FieldEffectManager *unk_00;
    u32 unk_04;
    UnkStruct_021F1118 *unk_08;
} UnkStruct_021F114C;

void ov01_021FD1CC(void *param0);

void ov01_021FD378(void);

#endif // POKEHEARTGOLD_OVERLAY_01_021FD1B8_H

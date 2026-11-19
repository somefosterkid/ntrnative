#ifndef POKEHEARTGOLD_UNK_STRUCT_OV59_0223C5B0_H
#define POKEHEARTGOLD_UNK_STRUCT_OV59_0223C5B0_H

#include "global.h"
#include "camera.h"
#include "gf_3d_vramman.h"
#include "library/spl_emitter.h"
#include "particle_system.h"
#include "pokepic.h"

// UnkStruct_ov59_0223C5B0: recovered by tools/hg/asmcluster.py (hk120) from the accesses of every function the object reaches
// (asm union-find with conflict cuts); DS offsets, pads elsewhere.
typedef struct UnkStruct_ov59_0223C5B0 {
    u8 unk_0[0x4];
    GF3DVramMan *unk_4;
    Camera *unk_8;
    PokepicManager *unk_C;
    u8 unk_10[0x10];
    Pokepic *unk_20;
    ParticleSystem *unk_24;
} UnkStruct_ov59_0223C5B0;

#endif // POKEHEARTGOLD_UNK_STRUCT_OV59_0223C5B0_H

#ifndef POKEHEARTGOLD_OVERLAY_07_UNK_STRUCT_OV07_02232D20_H
#define POKEHEARTGOLD_OVERLAY_07_UNK_STRUCT_OV07_02232D20_H

#include "constants/heap.h"

#include "particle_system.h"

// The seal / ball particle effect work: ov07_02232D20 allocates it, ov07_02232D80 starts the emitters, ov07_02232DF4 polls
// them and ov07_02232E10 frees it. sizeof == 0x24 on the DS; the pointer is 8 bytes on the host, so every access is by field.
typedef struct UnkStruct_ov07_02232D20_in {
    s16 unk0;
    s16 unk2;
    int unk4;
    enum HeapID unk8;
    int unkC;  // the resource to start, 0xFF = the whole file
    int unk10; // nonzero: the fixed file 0x403 and every resource
} UnkStruct_ov07_02232D20_in;

typedef struct UnkStruct_ov07_02232D20 {
    UnkStruct_ov07_02232D20_in unk0;
    u32 unk14;
    ParticleSystem *particleSystem; // DS 0x18
    int resourceCount;              // DS 0x1C
    int fileId;                     // DS 0x20 the particle file's member index
} UnkStruct_ov07_02232D20;

#endif // POKEHEARTGOLD_OVERLAY_07_UNK_STRUCT_OV07_02232D20_H

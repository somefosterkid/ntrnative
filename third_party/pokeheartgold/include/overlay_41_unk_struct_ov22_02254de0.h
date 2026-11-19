#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02254DE0_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02254DE0_H

#include "constants/heap.h"

struct SoftwareSprite;

typedef struct UnkStruct_ov22_02255040 {
    int unk_00;
    struct SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;

// Heap object allocated by ov41_02245EA0: a counted array of UnkStruct_ov22_02255040.
typedef struct UnkStruct_ov22_02254DE0 {
    UnkStruct_ov22_02255040 *unk_00;
    int unk_04;
} UnkStruct_ov22_02254DE0;

// sizeof(UnkStruct_ov22_02254DE0) == 0x8 on DS

UnkStruct_ov22_02254DE0 *ov41_02245EA0(s32 count, enum HeapID heapID);
void ov41_02245ECC(UnkStruct_ov22_02254DE0 *list);

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02254DE0_H

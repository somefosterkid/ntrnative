#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02259560_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02259560_H

// UnkStruct_ov22_02259560: the 0x10-byte node of ov41's sprite lists (merged by tools/hg/promote.py from 41 m2c local
// copies, 1 variant; the _t twin header is gone: tag and typedef are one type). Alloc: Heap_Alloc(sizeof(T) * count)
// in ov41_02248A28 / ov41_022499B4.

typedef struct UnkStruct_ov22_02259560_t {
    void *unk_00;
    int unk_04;
    struct UnkStruct_ov22_02259560_t *unk_08;
    struct UnkStruct_ov22_02259560_t *unk_0C;
} UnkStruct_ov22_02259560;

// sizeof(UnkStruct_ov22_02259560) == 0x10 on the DS (0x20 with 8-byte pointers)

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02259560_H

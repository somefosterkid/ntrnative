#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F74A4_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F74A4_H

// The 0x330-byte list of ov96 (hk191): 29 records of 0x1C plus a countdown, allocated by ov96_021F74A4,
// filled by ov96_021F74D0, freed by ov96_021F74C8.
typedef struct UnkStruct_ov96_021F74A4_Entry {
    s32 active;    // 0x00
    VecFx32 pos;   // 0x04
    void *unk_10;  // 0x10 ov96_021EB3E4 sprite handles
    void *unk_14;  // 0x14
    void *unk_18;  // 0x18
} UnkStruct_ov96_021F74A4_Entry;

// sizeof(UnkStruct_ov96_021F74A4_Entry) == 0x1c

typedef struct UnkStruct_ov96_021F74A4 {
    UnkStruct_ov96_021F74A4_Entry entries[29]; // 0x000
    s32 timer;                                 // 0x32C frames until the next ov96_021F7620 pick
} UnkStruct_ov96_021F74A4;

// sizeof(UnkStruct_ov96_021F74A4) == 0x330

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F74A4_H

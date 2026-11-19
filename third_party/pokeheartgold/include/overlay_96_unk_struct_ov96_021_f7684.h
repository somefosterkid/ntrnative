#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F7684_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F7684_H

// The 0x24-byte CPU pick object of ov96: built by ov96_021F7684, stepped by ov96_021F7740 / ov96_021F77EC,
// freed by ov96_021F7738 (hk191).
typedef struct UnkStruct_ov96_021F7684 {
    u32 active;         // 0x00 a pick is running
    u8 *dataCopyArea;   // 0x04 PokeathlonCourse data copy area (participant records at +0x50, 0x28 each)
    u8 unk_8[4];        // 0x08 per participant: sum of the members' byte 2
    u8 unk_C[4];        // 0x0C per participant: sum of the members' byte 4
    u8 unk_10[4][3];    // 0x10 per participant: running sums of unk_8, one per member
    u8 count;           // 0x1C CPU participants
    u8 cursor;          // 0x1D
    u8 timer;           // 0x1E
    u8 interval;        // 0x1F
    u8 first;           // 0x20 first CPU participant (4 - count)
    u8 unk_21;          // 0x21 == first; 4 = no CPU participant
    u8 pad_22[0x2];
} UnkStruct_ov96_021F7684;

// sizeof(UnkStruct_ov96_021F7684) == 0x24

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F7684_H

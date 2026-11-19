#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_FAB24_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_FAB24_H

// UnkStruct_ov96_021FAB24: one 0x6C-byte lane row of the ov96 0x3C8 work object (UnkStruct_ov96_021F94A8.rows[3] at +0xE0).
// Split (hk191) from the old promote.py merge of the work object and this row.

typedef struct UnkStruct_ov96_021FAB24 {
    u16 timer;   // 0x00 lane progress, the lane finishes at 0x1000
    u16 unk_2;   // 0x02
    u16 unk_4;   // 0x04 sub-screen BG y offset
    u16 unk_6;   // 0x06 main-screen BG y offset
    u8 unk_8;    // 0x08 finished
    u8 unk_9;    // 0x09 jump stage 0..4
    u8 pad_A[0x2];
    f32 unk_C;   // 0x0C speed
    f32 unk_10;  // 0x10 fraction carried to the next frame
    u16 bg0;     // 0x14
    u16 bg1;     // 0x16
    u8 unk_18;   // 0x18 lane index
    u8 pad19;
    u8 unk_1A;
    u8 unk_1B;
    s32 state;   // 0x1C tilemap state
    u8 pad_20[0x4];
    void *unk_24;
    s32 unk_28;  // 0x28 lane state (4 = done)
    s32 unk_2C;
    u8 unk_30;
    u8 unk_31;
    u8 unk_32;
    u8 pad_33[0x9];
    s32 unk_3C;
    u8 pad_40[0x4];
    void *unk_44;
    void *unk_48;
    void *unk_4C;
    f32 unk_50;  // 0x50 top speed
    f32 unk_54;  // 0x54 acceleration
    u8 unk_58;
    u8 pad_59[0x7];
    u16 unk_60;
    u16 unk_62;
    f32 unk_64;  // 0x64 sprite scale
    s16 unk_68;
    u16 unk_6A;
} UnkStruct_ov96_021FAB24;

// sizeof(UnkStruct_ov96_021FAB24) == 0x6c

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_FAB24_H

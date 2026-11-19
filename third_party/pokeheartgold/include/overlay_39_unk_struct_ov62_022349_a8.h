#ifndef POKEHEARTGOLD_OVERLAY_39_UNK_STRUCT_OV62_022349_A8_H
#define POKEHEARTGOLD_OVERLAY_39_UNK_STRUCT_OV62_022349_A8_H

// UnkStruct_ov62_022349A8: merged by tools/hg/promote.py from 11 m2c local copies (1 variants, base
// src/overlay_39_thumb_02227208.c) and 14 accesses; fields at every accessed offset, pads elsewhere.

struct MsgData;
struct SaveData;
struct String;
struct StringTemplate;

typedef struct UnkStruct_ov62_022349A8 {
    struct SaveData *saveData;
    void *unk_04;
    void *unk_08;
    struct { s32 unk_00; u8 unk_04; u8 language; s8 unk_06[301]; u8 padding[5]; } unk_0C;
    u32 heapID;
    int unk_148;
    int unk_14C;
    struct { void *unk_00; void *unk_04; void *unk_08; void *unk_0C; void *unk_10; void *unk_14; void *unk_18; void *unk_1C; void *unk_20; void *unk_24; void *unk_28; } unk_150;
    struct { BOOL unk_00; int unk_04; int unk_08; int unk_0C; } unk_17C;
    void *unk_18C;
    struct { u8 unk_0_anon[0x21c]; u8 unk_220_pad[0x4]; u8 unk_21C[0x8]; } unk_190;
    void *unk_3B4;
    int unk_3B8;
    int unk_3BC;
    int unk_3C0;
    BOOL unk_3C4;
    BOOL unk_3C8;
    struct { int unk_00; int unk_04; int unk_08; } unk_3CC;
    int unk_3D8;
    int unk_3DC;
    int unk_3E0;
    int unk_3E4;
    int unk_3E8;
    int unk_3EC;
    int unk_3F0;
    struct MsgData *unk_3F4;
    struct StringTemplate *unk_3F8;
    struct String *unk_3FC;
    struct { void *unk_00; u16 unk_04; u16 unk_06; u8 unk_08; u8 unk_C_pad[0x3]; void *unk_0C; } unk_400;
    u8 unk_40D;
    u8 unk_40E;
    u8 pad_416[0x2];
} UnkStruct_ov62_022349A8;

// sizeof(UnkStruct_ov62_022349A8) == 0x418

#endif // POKEHEARTGOLD_OVERLAY_39_UNK_STRUCT_OV62_022349_A8_H

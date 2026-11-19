#ifndef POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02264FEC_H
#define POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02264FEC_H

struct ManagedSprite;
struct UnkStruct_ov116_02262A8C;

// One of the 36 bouncing sprites of ov92_02260AE0 (0x34 on DS).
typedef struct UnkStruct_ov116_02264FEC_sub1 {
    struct ManagedSprite *unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    fx32 unk_18;
    fx32 unk_1C;
    fx32 unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
    int unk_28;
    s16 unk_2C;
    s16 unk_2E;
    s16 unk_30;
    s16 unk_32;
} UnkStruct_ov116_02264FEC_sub1;

// Sprite-burst task of the ov116 screen, allocated by ov92_022610E8 (0x764 on DS).
typedef struct UnkStruct_ov116_02264FEC {
    struct UnkStruct_ov116_02262A8C *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    BOOL *unk_10;
    UnkStruct_ov116_02264FEC_sub1 unk_14[36];
} UnkStruct_ov116_02264FEC; // 0x764

#endif // POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02264FEC_H

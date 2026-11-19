#ifndef POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_0226139_C_H
#define POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_0226139_C_H

// UnkStruct_ov116_0226139C: merged by tools/hg/promote.py from 17 m2c local copies (1 variants, base
// src/overlay_92_0225C540.c) and 26 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Camera;
struct G3DPipelineBuffers;
struct NARC;
struct NNSFndAllocatorFunc;
struct PaletteData;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct UnkStruct_ov114_02025CAD4_t;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov114_0225E854_t;

typedef struct UnkStruct_ov116_0226139C {
    void *unk_00;
    void *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    BOOL unk_10;
    struct { u8 unk_00; u8 padding_01[3]; u32 unk_04; } unk_14;
    struct { u32 unk_00; u8 padding_04[4]; } unk_1C;
    struct { struct { u32 unk_00; u8 padding_04[4]; } unk_00[4]; } unk_24;
    u32 unk_44;
    struct { struct NARC *unk_00; struct NARC *unk_04; struct SpriteSystem *unk_08; struct SpriteManager *unk_0C; struct BgConfig *unk_10; struct PaletteData *unk_14; struct G3DPipelineBuffers *unk_18; struct Camera *camera; u8 padding_20[4]; struct { struct NNSFndAllocatorFunc *pFunc; void *pHeap; u32 heapParam1; u32 heapParam2; } unk_24; struct { u8 unk_00; u8 padding_01[3]; } unk_2C; } unk_48;
    u32 unk_78;
    struct UnkStruct_ov114_02025CAD4_t *unk_7C;
    void *unk_80;
    struct { struct SaveData *saveData; u8 unk_04[4]; u8 unk_08; u8 unk_09; u8 unk_0A; u8 unk_0B; void *unk_0C; } unk_84;
    struct UnkStruct_ov114_0225D678_t *unk_94;
    struct { u32 unk_00[4]; u8 unk_10[4]; u32 unk_14; } unk_98;
    struct UnkStruct_ov114_0225E854_t *unk_B0;
    int unk_B4[4];
    BOOL unk_C4;
} UnkStruct_ov116_0226139C;

// sizeof(UnkStruct_ov116_0226139C) == 0xd0

#endif // POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_0226139_C_H

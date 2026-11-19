#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F5018_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F5018_H

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "sprite.h"
#include "vram_transfer_manager.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"

// The 0x1004-byte work object of the ov96 app run by ov96_021F5018 (PokeathlonCourse_AllocPtr4FromHeap).
// Replaces the old partial views (Ov96App_021F5A88, Ov96App_021F6C5C, UnkStruct_ov96_021F6E68).

struct NNSG2dScreenData;

// One participant's ball (ov96_021F6600 / ov96_021F6798 / ov96_021F7194), 3 at +0x90.
struct Ov96SpriteMgr;

typedef struct UnkStruct_ov96_021F5018_Ball {
    void *unk_00;     // 0x00 ov96_021EAF94 sprite set
    void *unk_04;     // 0x04 ov96_021EB3E4
    s32 unk_08;       // 0x08
    s32 unk_0C;       // 0x0C
    u8 unk_10[0x8];   // 0x10
    s32 unk_18;       // 0x18
    s32 unk_1C;       // 0x1C
    u16 unk_20;       // 0x20 score, capped at 999
    u8 unk_22[0x2];   // 0x22
    u16 unk_24;       // 0x24
    u8 unk_26;        // 0x26
    u8 unk_27;        // 0x27
    u8 unk_28;        // 0x28
    u8 unk_29[0x3];   // 0x29
    s32 unk_2C;       // 0x2C
    s32 unk_30;       // 0x30
    s32 unk_34;       // 0x34
} UnkStruct_ov96_021F5018_Ball;

// sizeof(UnkStruct_ov96_021F5018_Ball) == 0x38

// The physics row of one ball (ov96_021F6798), 3 at +0xFAC.
typedef struct UnkStruct_ov96_021F5018_Row {
    s32 unk_00;       // 0x00
    s32 unk_04;       // 0x04
    s32 unk_08;       // 0x08
    s32 unk_0C;       // 0x0C
    s32 unk_10;       // 0x10
    s32 unk_14;       // 0x14
    s32 unk_18;       // 0x18
} UnkStruct_ov96_021F5018_Row;

// sizeof(UnkStruct_ov96_021F5018_Row) == 0x1c

typedef struct UnkStruct_ov96_021F5018 {
    BgConfig *bgConfig;                       // 0x000
    Window windows[5];                        // 0x004
    enum HeapID heapID;                       // 0x054
    void *unk_58;                             // 0x058 GfGfxLoader_GetScrnData buffer
    struct NNSG2dScreenData *unk_5C;          // 0x05C
    struct Ov96SpriteMgr *unk_60;             // 0x060 ov96_021EB180
    void *unk_64;                             // 0x064 ov96_021EB3E4
    void *unk_68[4];                          // 0x068
    void *unk_78[3];                          // 0x078
    Sprite *unk_84[2];                        // 0x084 time digit sprites (ov96_021F6F3C)
    UnkStruct_ov96_021F74A4 *unk_8C;          // 0x08C
    UnkStruct_ov96_021F5018_Ball unk_90[3];   // 0x090
    void *unk_138;                            // 0x138
    s32 unk_13C;                              // 0x13C countdown, 0x708 frames
    u8 unk_140;                               // 0x140
    u8 unk_141;                               // 0x141
    u8 unk_142;                               // 0x142
    u8 unk_143;                               // 0x143
    VecFx32 unk_144[3];                       // 0x144
    u8 unk_168[3];                            // 0x168
    u8 unk_16B;                               // 0x16B
    VecFx32 unk_16C;                          // 0x16C
    u8 unk_178[0x10];                         // 0x178
    void *unk_188;                            // 0x188 ov96_021E9A78
    void *unk_18C;                            // 0x18C ov96_021EA854
    Camera *unk_190;                          // 0x190
    VecFx32 unk_194;                          // 0x194 camera target
    s32 unk_1A0[7][0x80];                     // 0x1A0
    s32 unk_FA0[3];                           // 0xFA0
    UnkStruct_ov96_021F5018_Row unk_FAC[3];   // 0xFAC
    NNSG2dCellTransferState *unk_1000;        // 0x1000 sub_02020654 cell transfer state
} UnkStruct_ov96_021F5018;

// sizeof(UnkStruct_ov96_021F5018) == 0x1004

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F5018_H

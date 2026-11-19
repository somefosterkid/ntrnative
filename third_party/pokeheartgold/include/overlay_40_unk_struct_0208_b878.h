#ifndef POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_0208_B878_H
#define POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_0208_B878_H

#include "constants/heap.h"

// UnkStruct_0208B878: heap object allocated by sub_02087284 (0x108 bytes), referenced by UnkStruct_0208C06C.unk_6F0 / unk_6F4.
// Replaces the anonymous local typedef copies in the ov40 / unk_02087xxx files.

struct NARC;
struct ManagedSprite;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct SysTask;

typedef struct UnkStruct_0208B284 {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;

typedef struct UnkStruct_0208B878_sub1 {
    struct NARC *unk_00;
    struct SpriteSystem *unk_04;
    struct SpriteManager *unk_08;
    struct PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;

typedef struct UnkStruct_0208B878_sub2 {
    struct ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;

typedef struct UnkStruct_0208B878 {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    struct SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;

// sizeof(UnkStruct_0208B878) == 0x108 on DS

#endif // POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_0208_B878_H

#ifndef POKEHEARTGOLD_UNK_0205C22C_H
#define POKEHEARTGOLD_UNK_0205C22C_H

// Union room avatar manager: Heap_Alloc(HEAP_ID_31, sizeof(UnkStruct_0205C22C)) in sub_0205AC88 (DS 0x484).
// Other translation units still carry layout-identical local copies.

#include "global.h"

struct FieldSystem;
struct OverworldAnimManager;
struct PlayerAvatar;
struct SavePalPad;
struct SysTask;
struct UnionRoomManager;
struct UnkStruct_0205C95C;

typedef struct UnkStruct_0205C680 {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 padding_05;
    u16 unk_06;
    u8 unk_08;
    u8 unk_09;
    u8 padding_0A[2];
    u32 unk_0C;
    struct OverworldAnimManager *unk_10;
    struct OverworldAnimManager *unk_14;
} UnkStruct_0205C680;

typedef struct UnkStruct_0205C22C {
    struct UnionRoomManager *unionRoomMan;
    struct SysTask *unk_04;
    struct PlayerAvatar *playerAvatar;
    UnkStruct_0205C680 unk_0C[50 + 1];
    struct FieldSystem *fieldSystem;
    struct SavePalPad *unk_474;
    struct UnkStruct_0205C95C *unk_478;
    int unk_47C;
    int unk_480;
} UnkStruct_0205C22C;

#endif // POKEHEARTGOLD_UNK_0205C22C_H

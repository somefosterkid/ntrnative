#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV49_02264CF8_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV49_02264CF8_H

#include "global.h"

#include "list_menu.h"

struct UnkStruct_ov70_0225CC54_t;

typedef struct UnkStruct_ov70_02268E44 {
    ListMenuTemplate unk_00;
    u32 unk_20;
} UnkStruct_ov70_02268E44;

typedef struct UnkStruct_ov70_02269114 {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u32 unk_08;
} UnkStruct_ov70_02269114;

// A per-player entry of the Wi-Fi Plaza ov49 list menus: allocated 0x50 bytes by ov49_0225EF40 in
// ov49_02263B74 and ov49_02262FB4 (the second site used to name it UnkStruct_ov70_02268BA0).
typedef struct UnkStruct_ov49_02264CF8 {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u16 unk_08;
    u16 unk_0A;
    u8 unk_0C;
    u8 unk_0D;
    s16 unk_0E;
    struct UnkStruct_ov70_0225CC54_t *unk_10;
    UnkStruct_ov70_02268E44 unk_14;
    UnkStruct_ov70_02269114 unk_38;
    u16 unk_44;
    u16 unk_46;
    s32 unk_48;
    BOOL unk_4C;
} UnkStruct_ov49_02264CF8;

// sizeof(UnkStruct_ov49_02264CF8) == 0x50

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV49_02264CF8_H

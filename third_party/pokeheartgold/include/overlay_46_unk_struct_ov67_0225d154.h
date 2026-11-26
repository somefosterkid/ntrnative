#ifndef POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D154_H
#define POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D154_H

#include "overlay_46_unk_struct_ov67_0225d210.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

// UnkStruct_ov67_0225D154: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/overlay_46_02258CB4_02258DA8.c) and 12 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Menu;
struct MsgData;
struct SaveData;
struct String;
struct StringTemplate;
struct SysTask;

typedef struct UnkStruct_ov67_0225D154 {
    struct SaveData *saveData;
    u32 unk_04;
    u32 unk_08;
    struct BgConfig *unk_0C;
    UnkStruct_ov67_0225D210 unk_10;
    UnkStruct_ov67_0225D210 unk_40;
    UnkStruct_ov67_0225D210 unk_70;
    UnkStruct_ov67_0225D210 unk_A0;
    struct Menu *unk_D0;
    UnkStruct_ov61_0222C3B0 unk_D4;
    int unk_400;
} UnkStruct_ov67_0225D154;

// sizeof(UnkStruct_ov67_0225D154) == 0x404

#endif // POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D154_H

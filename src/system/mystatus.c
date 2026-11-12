// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/mystatus.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/player_info.h"
#include "constants/version.h"
#include "field/unity_tower.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "system/str_tool.h"

// The player's name, IDs, gender, country and region (the ROM's string for the file, "mystatus.c"). The names of the
// fields are guessed; the accessors' names are swan's where it gives one.

// pokebw2's PlayerInfo_GetSize
u32 func_02008468(void) {
    return sizeof(PlayerInfo);
}

PlayerInfo *func_0200846c(u32 heapId) {
    PlayerInfo *info = GFL_HeapAllocate(heapId, sizeof(PlayerInfo), FALSE, "mystatus.c", 52);
    func_020084a0(info);
    return info;
}

void func_02008494(const PlayerInfo *src, PlayerInfo *dest) {
    MI_CpuCopy8(src, dest, sizeof(PlayerInfo));
}

void func_020084a0(PlayerInfo *info) {
    MI_CpuFill8(info, 0, sizeof(PlayerInfo));
    info->region = 2;
    func_02008560(info, GAME_VERSION);
}

BOOL func_020084bc(const PlayerInfo *info) {
    int i;

    for (i = 0; i < 8; i++) {
        if (info->name[i] != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

void copyTrainerName(PlayerInfo *info, const u16 *name) {
    wcharsncpy(name, info->name, 8);
}

void copyTrainerNameFromStrbuf(PlayerInfo *info, const StrBuf *name) {
    GFL_StrBufStoreString(name, info->name, 8);
}

u16 *GetPlayerName(PlayerInfo *info) {
    return info->name;
}

void copyName(const u16 *src, u16 *dest, u32 n) {
    wcharsncpy(src, dest, n);
}

void textCopy(const u16 *src, StrBuf *dest) {
    GFL_StrBufLoadString(dest, src);
}

StrBuf *copyTrainerNameToNewStrbuf(const u16 *name, u32 heapId) {
    StrBuf *buf = GFL_StrBufCreate(8, heapId);
    textCopy(name, buf);
    return buf;
}

void setIDAsUInt(PlayerInfo *info, u32 id) {
    info->id = id;
}

u32 getIDAsUInt(PlayerInfo *info) {
    return info->id;
}

u16 getTrainerID(PlayerInfo *info) {
    return info->id;
}

s32 func_0200853c(PlayerInfo *info) {
    return info->profileId;
}

void func_02008540(PlayerInfo *info, s32 profileId) {
    if (info->profileId == 0) {
        info->profileId = profileId;
    }
}

void setTrainerGender(PlayerInfo *info, u32 gender) {
    info->gender = gender;
}

u32 getTrainerGender(PlayerInfo *info) {
    return info->gender;
}

u8 func_02008554(PlayerInfo *info) {
    return info->unk1C;
}


void func_02008558(PlayerInfo *info, u8 value) {
    info->unk1C = value;
}

u8 func_0200855c(PlayerInfo *info) {
    return info->version;
}

void func_02008560(PlayerInfo *info, u8 value) {
    info->version = value;
}

u8 TrainerInfo_GetRegion(PlayerInfo *info) {
    return info->region;
}

void func_02008568(PlayerInfo *info, u8 value) {
    info->region = value;
}

u8 UnityTowerVisitor_GetCountry(PlayerInfo *info) {
    return info->country;
}

u8 UnityTowerVisitor_GetProvince(PlayerInfo *info) {
    return info->province;
}

void func_02008574(PlayerInfo *info, int country, int province) {
    info->country = country;
    info->province = province;
}

// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/player_info.h, the lines in use so far.
#ifndef BWPORT_SAVE_PLAYER_INFO_H
#define BWPORT_SAVE_PLAYER_INFO_H

#include "types.h"
#include "struct_decls.h"

struct PlayerInfo {
    u16 name[8];
    u32 id;
    // The GameSpy profile ID
    s32 profileId;
    u8 country;
    u8 province;
    u8 region;
    // The game version the player started on, VERSION_BLACK or VERSION_WHITE
    u8 version;
    u8 unk1C;
    u8 gender;
    u8 unk1E[2];
};

u16 *GetPlayerName(PlayerInfo *info);
u32 getTrainerGender(PlayerInfo *info);
u8 func_02008554(PlayerInfo *info);
u32 getIDAsUInt(PlayerInfo *info);
// The version (pokebw2's func_02008bfc)
u8 func_0200855c(PlayerInfo *info);
// pokebw2's PlayerInfo_GetSize
u32 func_02008468(void);

void copyTrainerName(PlayerInfo *info, const u16 *name);

PlayerInfo *func_0200846c(u32 heapId);
// Copies a player's info
void func_02008494(const PlayerInfo *src, PlayerInfo *dest);
// Whether the name is empty (all 8 characters 0)
BOOL func_020084bc(const PlayerInfo *info);
void func_02008540(PlayerInfo *info, s32 profileId);
void func_02008560(PlayerInfo *info, u8 value);
void func_02008574(PlayerInfo *info, int country, int province);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u16 getTrainerID(PlayerInfo *info);
u8 TrainerInfo_GetRegion(PlayerInfo *info);
void setTrainerGender(PlayerInfo *info, u32 gender);
void copyTrainerNameFromStrbuf(PlayerInfo *info, const StrBuf *name);
void setIDAsUInt(PlayerInfo *info, u32 id);
void func_020084a0(PlayerInfo *info);
// Copies at most n characters of a name, as wcharsncpy does
void copyName(const u16 *src, u16 *dest, u32 n);
// The player's GameSpy profile ID
s32 func_0200853c(PlayerInfo *info);
// A new game sets this to a random 0 to 7, plus 8 for a female player
void func_02008558(PlayerInfo *info, u8 value);
void func_02008568(PlayerInfo *info, u8 value);

#endif // BWPORT_SAVE_PLAYER_INFO_H

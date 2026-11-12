// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/compatible_irc_sys.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMPATIBLE_IRC_SYS_H
#define BWPORT_APP_COMPATIBLE_IRC_SYS_H

#include "types.h"
#include "struct_decls.h"
#include "save/player_info.h"
#include "gfl/heap.h"

typedef struct CompatibleIrcSys CompatibleIrcSys;

// Black 2's; twinned code confirms the offsets of date, birthMonth, birthDay, species, form, sex, isEgg, isFourteenth
// and trainer in Black
// What a player sends the other: the date, the birthday, the lead Pokémon and the trainer
typedef struct {
    // The year, month, day and weekday, a byte each from the high byte
    u32 date;
    u8 birthMonth;
    u8 birthDay;
    u16 species;
    u8 form;
    u8 sex;
    u8 isEgg;
    u8 isFourteenth : 4;
    u8 unkB_4 : 4;
    PlayerInfo trainer;
} CompatibleIrcProfile;

// pokebw2's CompatibleIrcSys_Update
// Called each frame: now and then sends the other player the count
BOOL func_ov101_021b63d0(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_GetPendingCount
int func_ov101_021b6480(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_ExchangeIds
BOOL func_ov101_021b630c(CompatibleIrcSys *sys, u32 value);
// pokebw2's CompatibleIrcSys_SendCount
BOOL func_ov101_021b6424(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_AddCommands
void func_ov101_021b64f0(CompatibleIrcSys *sys, int base, const void *list, int count, void *work);
// pokebw2's CompatibleIrcSys_RemoveCommands
void func_ov101_021b6508(CompatibleIrcSys *sys, int base);
// pokebw2's CompatibleIrcSys_SendEx
BOOL func_ov101_021b64b8(CompatibleIrcSys *sys, u16 command, u32 size, const void *data, u8 sendId, u32 a5, BOOL a6,
                             BOOL noCopy);

// pokebw2's CompatibleIrcSys_BuildProfile
// Fills a profile from the player's save
void func_ov101_021b658c(GameSystem *gsys, CompatibleIrcProfile *profile);

// pokebw2's CompatibleIrcSys_StartNet
// Each of these runs a step and returns whether it is done, and also returns TRUE at once if the link is off
BOOL func_ov101_021b6150(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_StartNegotiation
BOOL func_ov101_021b6254(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_Sync
BOOL func_ov101_021b62ac(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_ExchangeProfiles
BOOL func_ov101_021b6568(CompatibleIrcSys *sys, GameSystem *gsys);
// pokebw2's CompatibleIrcSys_GetPeerProfile
void func_ov101_021b657c(CompatibleIrcSys *sys, CompatibleIrcProfile *dest);
// pokebw2's CompatibleIrcSys_SendSelection
BOOL func_ov101_021b6518(CompatibleIrcSys *sys, u32 a, u32 b);
// pokebw2's CompatibleIrcSys_ResetCounts
void func_ov101_021b63c4(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_EndNet
BOOL func_ov101_021b61e8(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_IsConnected
BOOL func_ov101_021b62fc(CompatibleIrcSys *sys);

// pokebw2's CompatibleIrcSys_Create
// Creates the link, and frees it. A link made with disabled set does nothing and always reports it is done
CompatibleIrcSys *func_ov101_021b6100(int unk40, GameData *gameData, HeapID heapId, BOOL disabled);
// pokebw2's CompatibleIrcSys_ResetState
void func_ov101_021b63bc(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_PollSelection
BOOL func_ov101_021b652c(CompatibleIrcSys *sys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CompatibleIrcSys_SetCount
void func_ov101_021b63cc(CompatibleIrcSys *sys, int count);
// pokebw2's CompatibleIrcSys_Free
void func_ov101_021b6148(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_SendPing
BOOL func_ov101_021b6540(CompatibleIrcSys *sys);
// pokebw2's CompatibleIrcSys_PollPing
BOOL func_ov101_021b6554(CompatibleIrcSys *sys);

#endif // BWPORT_APP_COMPATIBLE_IRC_SYS_H

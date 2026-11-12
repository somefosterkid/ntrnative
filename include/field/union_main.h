// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_main.h: the lines in use so far.
#ifndef BWPORT_FIELD_UNION_MAIN_H
#define BWPORT_FIELD_UNION_MAIN_H

#include "types.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "field/event_colosseum_battle.h"
#include "struct_decls.h"
#include "field/union_app.h"

// Black 2's; twinned code confirms the offset of unk14 in Black
// The argument of the commands that run the activity's own proc: it starts with the app
typedef struct {
    UnionApp *app;
    u8 unk04[0x10];
    u32 unk14;
} UnionProcParam;

// Black 2's, not checked against Black
// The argument of the colosseum's party select: the party list, then the summary of a Pokémon in it, and the battle
struct UnionColosseumParam {
    PokeListParam list;
    PStatusParam status;
    ColosseumBattleParam *battle;
};

BOOL func_ov012_02173afc(u32 value);
BOOL func_ov012_02173a90(void);
BOOL func_ov012_02173b4c(UnionSystem *sys);
BOOL func_ov012_02173a04(u32 value);
BOOL func_ov012_02173a48(u32 value);
BOOL func_ov012_02173d38(UnionSystem *sys);
BOOL func_ov012_02173cb4(void);

void func_ov012_021739a8(int netId, int size, const void *data, void *work, NetHandle *handle);
// The senders of those commands. The first goes to every machine, those with a mask go to the machines in it
BOOL func_ov012_021739cc(void);
void func_ov012_021739e0(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173a24(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173a68(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173ac4(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173b1c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173d7c(int netId, int size, const void *data, void *work, NetHandle *handle);
// The reply to an entry request: refused, and accepted
BOOL func_ov012_02173d88(u8 sendTo);
void func_ov012_02173db0(int netId, int size, const void *data, void *work, NetHandle *handle);
BOOL func_ov012_02173dbc(u8 sendTo);
// pokebw2's UnionMain_SendEnqueue
// The senders that union_subproc.c's commands use
BOOL func_ov012_02173e1c(void);
// Sends union_app.c's commands
BOOL func_ov012_02173e68(u8 netId);
// pokebw2's UnionMain_SendRequestStatus
BOOL func_ov012_02173ec8(void);
BOOL func_ov012_02173f20(const UnionAppStatus *status, u8 sendTo);
// pokebw2's UnionMain_SendAppProfile
BOOL func_ov012_02173f90(u8 sendTo, const PlayerInfo *info);
BOOL func_ov012_02174010(u8 sendTo, const UnionAppMember *member);
// pokebw2's UnionMain_SendConfirmProfile
BOOL func_ov012_02174084(u8 sendTo, const PlayerInfo *info);
BOOL func_ov012_0217410c(u8 sendTo, u8 leftNetId);

// pokebw2's UnionMain_IsFieldReady
// Whether the field is up for the Union Room's update
BOOL func_ov012_02173840(UnionSystem *sys);
void func_ov012_02173934(UnionSystem *sys, u8 value);
u8 func_ov012_02173940(UnionSystem *sys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UnionCommand_CreateEvent
// The event that runs the command that UnionCommand_Set set, and whether one is set or running. The ids are
// the rows of the table in union_main.c
GameEvent *func_ov012_0217413c(GameSystem *gsys, Field *field, UnionSystem *sys);
// pokebw2's UnionCommand_Set
void func_ov012_0217422c(UnionSystem *sys, u32 id, void *arg);
// pokebw2's UnionCommand_IsActive
BOOL func_ov012_02174268(UnionSystem *sys);
// Sends command 0x1406 to everyone, with a buffer of the system's (in union_receive.c)
BOOL func_ov012_02173bb0(UnionSystem *sys);
// pokebw2's UnionNet_GetRecvBuffer
// The net commands of union_comm.c's table: a command's handler, and for those with data in chunks, where it goes
void *func_ov012_02173984(int netId, void *work, int size);
void func_ov012_02173ccc(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173de4(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173e40(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173e90(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173eec(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173f54(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173fe0(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_0217403c(int netId, int size, const void *data, void *work, NetHandle *handle);
// pokebw2's UnionMain_Boot
// Boots the Union Room's communication
void func_ov012_02173744(GameSystem *gsys);
// Called by the boot after the communication is booted
void func_ov010_021653c4(void);
// pokebw2's UnionMain_Update
// Runs each frame from the field while the Union Room's communication is up
void func_ov012_021737a4(GameCommSys *comm, Field *field);
// pokebw2's UnionMain_OnFieldIn
// The GameCommSys field callbacks for GAME_COMM_NO_UNION
void func_ov012_02173874(void *param, void *work, Field *field);
// pokebw2's UnionMain_OnFieldOut
void func_ov012_021738d4(void *param, void *work, Field *field);
u32 func_ov012_0217391c(UnionSystem *sys);
u32 func_ov012_02173928(UnionSystem *sys);
// pokebw2's UnionMain_IsIdle
// Whether the Union Room is up and idle
BOOL func_ov012_0217394c(GameSystem *gsys);

#endif // BWPORT_FIELD_UNION_MAIN_H

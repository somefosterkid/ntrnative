// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_app.h: the lines in use so far.
#ifndef BWPORT_FIELD_UNION_APP_H
#define BWPORT_FIELD_UNION_APP_H

#include "types.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "gfl/heap.h"

#define UNION_APP_MEMBER_MAX 5

// Black 2's; twinned code confirms the offsets of info and mac in Black
// A member's profile
typedef struct {
    PlayerInfo info;
    u8 mac[6];
    u8 pad[2];
} UnionAppMember;

// pokebw2's UnionApp_CountJoined
u8 func_ov022_021841a4(UnionApp *app);
// pokebw2's UnionApp_GetJoined
u32 func_ov022_021841a0(UnionApp *app);
// pokebw2's UnionApp_GetMember
UnionAppMember *func_ov022_02184180(UnionApp *app, u8 netId);
// pokebw2's UnionApp_RequestStart
void func_ov022_02184174(UnionApp *app);

// Black 2's, not checked against Black
typedef void (*UnionAppMemberCallback)(u8 netId, UnionAppMember *member, void *work);

// pokebw2's UnionApp_OpenEntry
void func_ov022_02184134(UnionApp *app);
// pokebw2's UnionApp_SetCallbacks
void func_ov022_021840fc(UnionApp *app, UnionAppMemberCallback onJoin, UnionAppMemberCallback onLeave, void *work);
// pokebw2's UnionApp_CloseEntry
BOOL func_ov022_02184114(UnionApp *app);

// pokebw2's UnionApp_LimitEntry
void func_ov022_02184148(UnionApp *app, u8 limit);

// pokebw2's UnionApp_RequestEntry
BOOL func_ov022_02183be8(UnionApp *app, u8 netId);
// pokebw2's UnionApp_Enqueue
void func_ov022_02183f6c(UnionApp *app, u8 netId);
// pokebw2's UnionApp_RequestSendStatus
void func_ov022_02183f88(UnionApp *app, u8 netId);
// pokebw2's UnionApp_ReceiveStatus
void func_ov022_02183f98(UnionApp *app, const UnionAppStatus *status);
// pokebw2's UnionApp_GetStatusSize
u32 func_ov022_021840a8(void);
// pokebw2's UnionApp_SetMemberProfile
void func_ov022_02183fb8(UnionApp *app, UnionSystem *sys, u8 netId, UnionAppMember *member);
// pokebw2's UnionApp_RequestSendProfile
void func_ov022_02183fa8(UnionApp *app, u8 netId);
// pokebw2's UnionApp_ConfirmEntry
void func_ov022_02184060(UnionApp *app, u8 netId);

// Whether others can still enter, set by UnionApp_CloseEntry, UnionApp_OpenEntry and UnionApp_LimitEntry
enum {
    UNION_APP_ENTRY_OPEN,
    UNION_APP_ENTRY_CLOSED,
    UNION_APP_ENTRY_LIMITED,
};
// Black 2's; twinned code confirms the offsets of memberMax and joined in Black
// The part of the work that the parent sends the members (UnionApp_GetStatusSize gives its size)
struct UnionAppStatus {
    u8 memberMax;
    // Bit per net ID
    u8 joined;
    u8 unk2[2];
};
// Black 2's; twinned code confirms the offsets of status, entering, confirmed, entryMode, entryCount, entryLimit,
// statusSendTo, profileSendTo, hasProfile, queued, current, currentPending, startRequested, starting, started, members,
// onJoin, onLeave and callbackWork, and its size in Black
// Every set of members below is a byte with a bit per net ID
struct UnionApp {
    UnionAppStatus status;
    // Those that are allowed to enter and haven't joined yet
    u8 entering;
    u8 unk5[2];
    // Those whose entry is confirmed, to join next update
    u8 confirmed;
    u8 unk8;
    u8 entryMode;
    // Entries accepted while entry is limited, and the most that can be
    u8 entryCount;
    u8 entryLimit;
    // Those to send the status to
    u8 statusSendTo;
    // Those to send this machine's profile to
    u8 profileSendTo;
    // Those whose profile has arrived
    u8 hasProfile;
    // Those waiting for their turn to be handled at 0x10
    u8 queued;
    u8 current;
    u8 currentPending;
    u8 unk12;
    u8 unk13;
    u32 unk14;
    u8 unk18[8];
    BOOL startRequested;
    BOOL starting;
    BOOL started;
    UnionAppMember members[UNION_APP_MEMBER_MAX];
    UnionAppMemberCallback onJoin;
    UnionAppMemberCallback onLeave;
    void *callbackWork;
};
// pokebw2's UnionApp_Create
UnionApp *func_ov022_02183b60(UnionSystem *sys, HeapID heapId, u8 memberMax, const PlayerInfo *info);
// pokebw2's UnionApp_Update
void func_ov022_02183c3c(UnionApp *app, UnionSystem *sys);
void func_ov022_02183f7c(UnionApp *app);
u8 func_ov022_02183f84(UnionApp *app);
// pokebw2's UnionApp_HasMembers
BOOL func_ov022_0218401c(UnionApp *app);
// pokebw2's UnionApp_AllProfilesReceived
BOOL func_ov022_0218402c(UnionApp *app);
// pokebw2's UnionApp_JoinSelf
void func_ov022_021840ac(UnionApp *app);
// pokebw2's UnionApp_IsEntryClosed
BOOL func_ov022_021840c8(UnionApp *app);
// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UnionApp_IsStarted
BOOL func_ov022_0218417c(UnionApp *app);
// pokebw2's UnionApp_Free
void func_ov022_02183be0(UnionApp *app);
// pokebw2's UnionApp_OnLeave
void func_ov022_021840a4(UnionApp *app, u8 netId);

#endif // BWPORT_FIELD_UNION_APP_H

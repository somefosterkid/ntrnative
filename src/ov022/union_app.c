// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov069/union_app.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "field/union_app.h"
#include "field/union_comm.h"
#include "field/union_main.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/std.h"
#include "nitro/math.h"
#include "save/player_info.h"
#include "nitro/os.h"

static void func_ov022_02183cc0(UnionApp *app);
static void func_ov022_02183d34(UnionApp *app);
static void func_ov022_02183d4c(UnionApp *app);
static void func_ov022_02183d94(UnionApp *app);
static void func_ov022_02183dac(UnionApp *app);
static void func_ov022_02183ddc(UnionApp *app);
static void func_ov022_02183e48(UnionApp *app, UnionSystem *sys);
static void func_ov022_02183ee0(UnionApp *app, UnionSystem *sys);
static void func_ov022_0218416c(UnionApp *app);

// pokebw2's UnionApp_Create
UnionApp *func_ov022_02183b60(UnionSystem *sys, HeapID heapId, u8 memberMax, const PlayerInfo *info) {
    UnionAppMember member;
    UnionApp *app = GFL_HeapAllocate(heapId, sizeof(UnionApp), TRUE, "union_app.c", 77);

    if (func_0203d378() == TRUE) {
        app->status.joined = 3;
    }
    app->status.memberMax = memberMax;
    app->current = 0xff;
    app->currentPending = 0xff;

    MI_CpuFill8(&member, 0, sizeof(UnionAppMember));
    member.info = *info;
    OS_GetMacAddress(member.mac);
    func_ov022_02183fb8(app, sys, func_0203d1a4(func_020399a8()), &member);
    func_ov022_02184114(app);
    func_ov022_0218416c(app);
    return app;
}

// pokebw2's UnionApp_Free
void func_ov022_02183be0(UnionApp *app) {
    GFL_HeapFree(app);
}

// pokebw2's UnionApp_RequestEntry
BOOL func_ov022_02183be8(UnionApp *app, u8 netId) {
    u32 joined;

    if (app->entryMode == UNION_APP_ENTRY_CLOSED) {
        return FALSE;
    }
    joined = MATH_CountPopulation(app->status.joined);
    if (app->entryMode == UNION_APP_ENTRY_LIMITED && app->entryCount + 1U > app->entryLimit) {
        return FALSE;
    }
    if (joined + 1 <= app->status.memberMax) {
        app->entering |= 1 << netId;
        if (app->entryMode == UNION_APP_ENTRY_LIMITED) {
            app->entryCount++;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionApp_Update
void func_ov022_02183c3c(UnionApp *app, UnionSystem *sys) {
    if (func_0200fb24()) {
        return;
    }
    if (app->started == TRUE) {
        return;
    }
    if (app->startRequested == TRUE) {
        func_ov012_02172d38(sys);
        app->startRequested = FALSE;
        app->starting = TRUE;
        return;
    }
    if (app->starting == TRUE) {
        if (!func_ov012_02172d68(sys)) {
            app->starting = FALSE;
            app->started = TRUE;
        }
        return;
    }
    if (func_0203d378() == TRUE) {
        func_ov022_02183ee0(app, sys);
        func_ov022_02183d34(app);
        func_ov022_02183cc0(app);
        func_ov022_02183d4c(app);
        func_ov022_02183d94(app);
    }
    func_ov022_02183dac(app);
    func_ov022_02183ddc(app);
    func_ov022_02183e48(app, sys);
}

// pokebw2's UnionApp_CheckJoinedLeft
static void func_ov022_02183cc0(UnionApp *app) {
    int i;
    int bit;
    u32 connected = app->status.joined;

    if (app->current != 0xff) {
        return;
    }
    for (i = 0; i < UNION_APP_MEMBER_MAX; i++) {
        bit = 1 << i;
        if ((app->status.joined & bit) && !func_0203d1c0((u8)i)) {
            connected ^= bit;
        }
    }
    if (connected == app->status.joined) {
        return;
    }
    for (i = 0; i < UNION_APP_MEMBER_MAX; i++) {
        bit = 1 << i;
        if ((app->status.joined & bit) && !(connected & bit) && func_ov012_0217410c(connected, i) == TRUE) {
            func_ov022_021840a4(app, i);
            app->status.joined ^= bit;
        }
    }
}

// pokebw2's UnionApp_SendStatus
static void func_ov022_02183d34(UnionApp *app) {
    if (app->statusSendTo != 0 && func_ov012_02173f20(&app->status, app->statusSendTo) == TRUE) {
        app->statusSendTo = 0;
    }
}

// pokebw2's UnionApp_UpdateQueue
static void func_ov022_02183d4c(UnionApp *app) {
    int i;
    int bit;
    u8 queued;

    if (app->current != 0xff) {
        if (func_0203d1c0(app->current)) {
            return;
        }
        app->current = 0xff;
        app->currentPending = 0xff;
    }
    queued = app->queued;
    if (queued == 0) {
        return;
    }
    for (i = 0; i < UNION_APP_MEMBER_MAX; i++) {
        bit = 1 << i;
        if (queued & bit) {
            app->current = i;
            app->currentPending = i;
            app->queued ^= bit;
            return;
        }
    }
}

// pokebw2's UnionApp_SendCurrent
static void func_ov022_02183d94(UnionApp *app) {
    if (app->currentPending != 0xff && func_ov012_02173e68(app->currentPending) == TRUE) {
        app->currentPending = 0xff;
    }
}

// pokebw2's UnionApp_SendProfile
static void func_ov022_02183dac(UnionApp *app) {
    if (app->profileSendTo != 0 &&
        func_ov012_02174010(app->profileSendTo, &app->members[func_0203d1a4(func_020399a8())]) == TRUE) {
        app->profileSendTo = 0;
    }
}

// pokebw2's UnionApp_JoinConfirmed
static void func_ov022_02183ddc(UnionApp *app) {
    int i;
    int bit;

    if (app->confirmed == 0) {
        return;
    }
    for (i = 0; i < UNION_APP_MEMBER_MAX; i++) {
        bit = 1 << i;
        if (app->confirmed & bit) {
            if ((app->hasProfile & bit) && (app->entering & bit)) {
                app->entering ^= bit;
                app->status.joined |= bit;
                if (app->onJoin != NULL) {
                    app->onJoin(i, &app->members[i], app->callbackWork);
                }
                app->current = 0xff;
            }
            app->confirmed ^= bit;
        }
    }
}

// pokebw2's UnionApp_CheckProfileLeft
static void func_ov022_02183e48(UnionApp *app, UnionSystem *sys) {
    int i;
    int bit;

    for (i = 0; i < UNION_APP_MEMBER_MAX; i++) {
        bit = 1 << i;
        if ((app->hasProfile & bit) && !func_0203d1c0((u8)i)) {
            if (func_0203d378() == TRUE || (app->status.joined & bit)) {
                if (app->onLeave != NULL) {
                    app->onLeave(i, &app->members[i], app->callbackWork);
                }
                app->status.joined &= 0xff ^ bit;
                app->hasProfile &= 0xff ^ bit;
            }
            func_ov012_02173614(&sys->situ.mycomm, app->members[i].mac);
        }
    }
}

// pokebw2's UnionApp_CheckEnteringLeft
static void func_ov022_02183ee0(UnionApp *app, UnionSystem *sys) {
    u8 i;
    int bit;

    for (i = 0; i < UNION_APP_MEMBER_MAX; i++) {
        bit = 1 << i;
        if ((app->entering & bit) && !func_0203d1c0(i)) {
            app->entering ^= bit;
            if (app->hasProfile & bit) {
                app->hasProfile &= 0xff ^ bit;
                func_ov012_02173614(&sys->situ.mycomm, app->members[i].mac);
            }
            if (app->entryMode == UNION_APP_ENTRY_LIMITED && app->entryCount != 0) {
                app->entryCount--;
            }
        }
    }
}

// pokebw2's UnionApp_Enqueue
void func_ov022_02183f6c(UnionApp *app, u8 netId) {
    app->queued |= 1 << netId;
}

void func_ov022_02183f7c(UnionApp *app) {
    app->unk12 = TRUE;
}

u8 func_ov022_02183f84(UnionApp *app) {
    return app->unk12;
}

// pokebw2's UnionApp_RequestSendStatus
void func_ov022_02183f88(UnionApp *app, u8 netId) {
    app->statusSendTo |= 1 << netId;
}

// pokebw2's UnionApp_ReceiveStatus
void func_ov022_02183f98(UnionApp *app, const UnionAppStatus *status) {
    MI_CpuCopy8(status, &app->status, sizeof(UnionAppStatus));
}

// pokebw2's UnionApp_RequestSendProfile
void func_ov022_02183fa8(UnionApp *app, u8 netId) {
    app->profileSendTo |= 1 << netId;
}

// pokebw2's UnionApp_SetMemberProfile
void func_ov022_02183fb8(UnionApp *app, UnionSystem *sys, u8 netId, UnionAppMember *member) {
    UnionSelf *self = &sys->situ;

    app->members[netId] = *member;
    app->hasProfile |= 1 << netId;
    if (netId != func_0203d1a4(func_020399a8())) {
        func_ov012_0217357c(&self->mycomm, member->mac, func_02008554(&member->info), getTrainerGender(&member->info));
    }
}

// pokebw2's UnionApp_HasMembers
BOOL func_ov022_0218401c(UnionApp *app) {
    if (app->status.joined != 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionApp_AllProfilesReceived
BOOL func_ov022_0218402c(UnionApp *app) {
    int i;
    int bit;

    for (i = 0; i < UNION_APP_MEMBER_MAX; i++) {
        bit = 1 << i;
        if ((app->status.joined & bit) && func_0203d1c0((u8)i) == TRUE && !(app->hasProfile & bit)) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's UnionApp_ConfirmEntry
void func_ov022_02184060(UnionApp *app, u8 netId) {
    if (func_0203d378() == TRUE) {
        int bit = 1 << netId;

        if ((app->hasProfile & bit) && (app->entering & bit)) {
            app->confirmed |= bit;
        }
    } else {
        app->entering |= 1 << netId;
        app->confirmed |= 1 << netId;
    }
}

// pokebw2's UnionApp_OnLeave
void func_ov022_021840a4(UnionApp *app, u8 netId) {
}

// pokebw2's UnionApp_GetStatusSize
u32 func_ov022_021840a8(void) {
    return sizeof(UnionAppStatus);
}

// pokebw2's UnionApp_JoinSelf
void func_ov022_021840ac(UnionApp *app) {
    app->status.joined |= 1 << func_0203d1a4(func_020399a8());
}

// pokebw2's UnionApp_IsEntryClosed
BOOL func_ov022_021840c8(UnionApp *app) {
    if (app == NULL) {
        return FALSE;
    }
    switch (app->entryMode) {
    case UNION_APP_ENTRY_OPEN:
        return FALSE;
    case UNION_APP_ENTRY_CLOSED:
        return TRUE;
    case UNION_APP_ENTRY_LIMITED:
        if (app->entryCount >= app->entryLimit) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// pokebw2's UnionApp_SetCallbacks
void func_ov022_021840fc(UnionApp *app, UnionAppMemberCallback onJoin, UnionAppMemberCallback onLeave, void *work) {
    app->onJoin = onJoin;
    app->onLeave = onLeave;
    app->callbackWork = work;
}

// pokebw2's UnionApp_CloseEntry
BOOL func_ov022_02184114(UnionApp *app) {
    if (app->entering == 0) {
        func_0203d1e8(func_020399a8(), FALSE);
        app->entryMode = UNION_APP_ENTRY_CLOSED;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionApp_OpenEntry
void func_ov022_02184134(UnionApp *app) {
    func_0203d1e8(func_020399a8(), TRUE);
    app->entryMode = UNION_APP_ENTRY_OPEN;
}

// pokebw2's UnionApp_LimitEntry
void func_ov022_02184148(UnionApp *app, u8 limit) {
    if (func_0203cd98()) {
        func_0203d1e8(func_020399a8(), TRUE);
        app->entryMode = UNION_APP_ENTRY_LIMITED;
        app->entryLimit = limit;
        app->entryCount = 0;
    }
}

static void func_ov022_0218416c(UnionApp *app) {
    app->unk14 = TRUE;
}

// pokebw2's UnionApp_RequestStart
void func_ov022_02184174(UnionApp *app) {
    app->startRequested = TRUE;
}

// pokebw2's UnionApp_IsStarted
BOOL func_ov022_0218417c(UnionApp *app) {
    return app->started;
}

// pokebw2's UnionApp_GetMember
UnionAppMember *func_ov022_02184180(UnionApp *app, u8 netId) {
    int bit = 1 << netId;

    if (!(app->hasProfile & bit) || !(app->status.joined & bit)) {
        return NULL;
    }
    return &app->members[netId];
}

u32 func_ov022_021841a0(UnionApp *app) {
    return app->status.joined;
}


// pokebw2's UnionApp_CountJoined
u8 func_ov022_021841a4(UnionApp *app) {
    return MATH_CountPopulation(app->status.joined);
}

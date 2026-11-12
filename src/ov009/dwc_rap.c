// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov011/dwc_rap.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// dwc_rap.c, in overlay 11: the GFL net's Wi-Fi connection, over Nintendo's DWC library. The name is the ROM's own,
// from the file name of its allocations. The functions have no names in swan's symbols; the names in the asserts
// (data_ov009_021a8264, randommatch_query) are the original's

#include <stdio.h>
#include "gfl/dwc_rap.h"
#include "types.h"
#include "dwc/connectinet.h"
#include "dwc/dwc.h"
#include "dwc/gdb.h"
#include "dwc/nd.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/dwc_vchat.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "gfl/net_devwifi.h"

#define _MATCHSTRINGNUM 0x80
#define MYDWC_STATUS_DATA_SIZE_MAX 0x20
#define DWCRAP_FRIEND_MAX 32
#define DWCRAP_NETID_MAX 2

// The states of the connection, in the work's state
enum {
    DWCRAP_STATE_INIT,
    DWCRAP_STATE_INET,
    DWCRAP_STATE_LOGIN_WAIT,
    DWCRAP_STATE_LOGIN,
    DWCRAP_STATE_SERVERS,
    DWCRAP_STATE_FRIENDS,
    DWCRAP_STATE_READY,
    DWCRAP_STATE_MATCHING,
    DWCRAP_STATE_MATCH_CANCEL,
    DWCRAP_STATE_CONNECTED,
    DWCRAP_STATE_CONNECTED_IDLE,
    DWCRAP_STATE_MATCH_ENDED,
    DWCRAP_STATE_MATCH_FAILED,
    DWCRAP_STATE_MATCH_FAILED_END,
    DWCRAP_STATE_ERROR,
    DWCRAP_STATE_15,
    DWCRAP_STATE_SERVER_ERROR,
    DWCRAP_STATE_KICKED,
    DWCRAP_STATE_CLOSED,
    DWCRAP_STATE_ENDING,
    DWCRAP_STATE_ENDED,
};

// The work of the connection, 0x7ac bytes
typedef struct {
    // The packet that is being sent: its header, and what follows
    union {
        u32 word;
        u8 bytes[4];
    } header;
    u8 data[0xfc];
    DWCFriendData *friendList;
    void *unk104;
    void *userData;
    DWCInetControl inetControl;
    void *recvBuf[2];
    // Errors that came up in the library, until they are given to the game
    int pendingCode[DWCRAP_FRIEND_MAX];
    int pendingParam[DWCRAP_FRIEND_MAX];
    GFLNetRecvFunc parentRecvFunc;
    GFLNetRecvFunc childRecvFunc;
    DWCRapDisconnectFunc disconnectFunc;
    void *disconnectWork;
    DWCRapErrorFunc errorFunc;
    void *errorWork;
    DWCRapEventFunc eventFunc;
    void *eventWork;
    DWCRapConnectFunc connectFunc;
    void *connectWork;
    DWCRapRequestFunc requestFunc;
    void (*kickFunc)(int code);
    char randommatch_query[_MATCHSTRINGNUM];
    // What the library says of each friend, and the data they share about where they are
    u8 friendStatus[DWCRAP_FRIEND_MAX];
    char friendStatusData[DWCRAP_FRIEND_MAX][MYDWC_STATUS_DATA_SIZE_MAX];
    u32 friendCheckIndex;
    int connectResult;
    int state;
    int errorCode;
    int matchType;
    int sending;
    int maxPlayers;
    u32 lastConnectBits;
    int vctActive;
    int friendIndex;
    int lastNetId;
    int matchingBusy;
    int vctMode;
    s16 lastIndex;
    s16 vctEnabled;
    int idleFrames[DWCRAP_NETID_MAX];
    int retryCount;
    int unk790;
    u16 unk794;
    u16 unk796;
    u32 connectBits;
    u8 sendSeq;
    u8 recvSeq;
    u8 unk79E;
    u8 micOn;
    u8 unk7A0;
    u8 unk7A1;
    u8 userDataDirty;
    u8 reportError;
    u8 unk7A4;
    u8 unk7A5;
    u8 sendReady;
    u8 ackPending;
    u8 connected;
    u8 friendsMatchActive;
    u8 unk7AA[2];
} DWCRapWork;

// pokebw2's _dWork
static DWCRapWork *data_ov009_021a8264;

// pokebw2's sQueryFormat
static const char data_ov009_021a1974[] = "%s = '%s'";

static void func_ov009_02155374(int state);
void func_ov009_021554a0(void);
static u32 func_ov009_021554c8(void);
static BOOL func_ov009_02155678(void *param);
static void func_ov009_02155904(void);
static BOOL func_ov009_02155b00(const void *data, int size);
static void func_ov009_02155b68(void);
static void func_ov009_02155b98(DWCError error, int profileID, void *param);
static void func_ov009_02155c0c(DWCError error, BOOL isChanged, void *param);
static void func_ov009_02155c24(int index, u8 status, const char *location, void *param);
static void func_ov009_02155c28(int deleteIndex, int srcIndex, void *param);
static void func_ov009_02155c6c(int index, void *param);
static void func_ov009_02155c70(void);
static void func_ov009_02155c8c(void);
static void func_ov009_02155d2c(int index);
static int func_ov009_02155d7c(int index, void *param);
static void func_ov009_02155da4(u32 header);
u32 func_ov009_02155f58(void);
static BOOL func_ov009_0215610c(void);
static BOOL func_ov009_0215615c(int index);
static u32 func_ov009_021561c0(void);
static void func_ov009_0215631c(void);
static void func_ov009_02156344(HeapID heapId);
static void func_ov009_021567b8(void);
static void func_ov009_02156a58(DWCError error, BOOL cancel, BOOL a2, BOOL a3, int index, void *param);
static void func_ov009_02156b18(DWCError error, BOOL cancel, BOOL a2, BOOL a3, int index, void *param);
static void func_ov009_02156b3c(int index, void *param);
static void func_ov009_02156bd8(int aid);
static void func_ov009_02156c00(int aid);
static void func_ov009_02156c60(void);
static void func_ov009_02156cf4(void);

// pokebw2's DWCRap_SetState
static void func_ov009_02155374(int state) {
    data_ov009_021a8264->state = state;
}

// pokebw2's DWCRap_IsInitialized
BOOL func_ov009_02155388(void) {
    if (data_ov009_021a8264) {
        return TRUE;
    }
    return FALSE;
}

BOOL DWC_CheckHasProfile(DWCUserData *userData);
BOOL DWC_CheckValidConsole(DWCUserData *userData);
int func_ov009_0215539c(void *userData, DWCFriendData *friendList) {
    GFLNetInitData *netInit = func_0203d798();
    int i;

    GFL_ASSERT(data_ov009_021a8264 == NULL);
    data_ov009_021a8264 = allocConfigDSSoftwareFeature(netInit->heapId, sizeof(DWCRapWork), "dwc_rap.c", 302);
    for (i = 0; i < DWCRAP_FRIEND_MAX; i++) {
        data_ov009_021a8264->pendingCode[i] = -1;
    }
    func_ov009_02155374(DWCRAP_STATE_INIT);
    data_ov009_021a8264->vctMode = 0;
    data_ov009_021a8264->friendIndex = -1;
    data_ov009_021a8264->maxPlayers = netInit->maxConnectNum;
    data_ov009_021a8264->lastNetId = -1;
    data_ov009_021a8264->vctEnabled = 0;
    data_ov009_021a8264->unk79E = 0;
    data_ov009_021a8264->unk794 = 1;
    data_ov009_021a8264->unk790 = 1;
    data_ov009_021a8264->unk796 = 1;
    data_ov009_021a8264->userData = userData;
    data_ov009_021a8264->friendList = friendList;
    data_ov009_021a8264->unk7A1 = 1;
    for (i = 0; i < DWCRAP_FRIEND_MAX; i++) {
        data_ov009_021a8264->friendStatus[i] = 0;
    }
    if (!DWC_CheckHasProfile(data_ov009_021a8264->userData)) {
        return 1;
    }
    if (!DWC_CheckValidConsole(data_ov009_021a8264->userData)) {
        return 2;
    }
    return 0;
}


// pokebw2's DWCRap_Delete
void func_ov009_021554a0(void) {
    if (data_ov009_021a8264 != NULL) {
        func_0203d798();
        func_ov009_02156c60();
        func_0203d7fc(data_ov009_021a8264);
        data_ov009_021a8264 = NULL;
    }
    DWC_ClearError();
}

// Connects to the internet, logs in, and updates the friend list
// pokebw2's DWCRap_UpdateLogin
static u32 func_ov009_021554c8(void) {
    OSOwnerInfo owner;
    DWCFriendData *friendList;
    int friendListLen;
    u32 result;

    switch (data_ov009_021a8264->state) {
    case DWCRAP_STATE_INIT:
        func_0203d798();
        func_0205c6ac(&data_ov009_021a8264->inetControl, 2, 1, 20);
        func_0205c758();
        func_ov009_02155374(DWCRAP_STATE_INET);
        data_ov009_021a8264->vctActive = 0;
        // fall through
    case DWCRAP_STATE_INET:
        if (func_0205c88c()) {
            if (func_0205ca20() == 4) {
                func_ov009_02155374(DWCRAP_STATE_LOGIN_WAIT);
            } else {
                func_ov009_02155374(DWCRAP_STATE_ERROR);
            }
        } else {
            func_0205c8e0();
            break;
        }
        // fall through
    case DWCRAP_STATE_LOGIN_WAIT:
        result = func_ov009_02155f58();
        if (result != 0) {
            return result;
        }
        friendList = data_ov009_021a8264->friendList;
        friendListLen = 0;
        if (friendList != NULL) {
            friendListLen = 32;
        }
        DWC_InitFriendsMatch(data_ov009_021a8264->userData, 0x2fc6, "tXH2sN", 0, 0, friendList, friendListLen);
        data_ov009_021a8264->friendsMatchActive = 1;
        {
            u16 name[2] = {0, 0};
            OS_GetOwnerInfo(&owner);
            DWC_LoginAsync(name, NULL, func_ov009_02155b98, NULL);
        }
        func_ov009_02155374(DWCRAP_STATE_LOGIN);
        // fall through
    case DWCRAP_STATE_LOGIN:
        DWC_ProcessFriendsMatch();
        break;
    case DWCRAP_STATE_SERVERS:
        if (data_ov009_021a8264->friendList != NULL) {
            if (!DWC_UpdateServersAsync(NULL, func_ov009_02155c0c, data_ov009_021a8264->userData, func_ov009_02155c24,
                                     &data_ov009_021a8264, func_ov009_02155c28, &data_ov009_021a8264)) {
                func_0203c9ac(0, 0, 0, 0x3ee);
            } else {
                DWC_SetBuddyFriendCallback(func_ov009_02155c6c, NULL);
                DWC_ProcessFriendsMatch();
            }
        }
        func_ov009_02155374(DWCRAP_STATE_FRIENDS);
        break;
    case DWCRAP_STATE_FRIENDS:
        DWC_ProcessFriendsMatch();
        if (data_ov009_021a8264->friendList == NULL) {
            func_ov009_02155374(DWCRAP_STATE_READY);
        }
        break;
    case DWCRAP_STATE_READY:
        DWC_ProcessFriendsMatch();
        data_ov009_021a8264->errorCode = 0x3ef;
        return 0x3ef;
    }
    return func_ov009_02155f58();
}

// pokebw2's DWCRap_SetRecvFuncs
void func_ov009_02155648(GFLNetRecvFunc parentFunc, GFLNetRecvFunc childFunc) {
    data_ov009_021a8264->parentRecvFunc = parentFunc;
    data_ov009_021a8264->childRecvFunc = childFunc;
}

// pokebw2's DWCRap_SetRequestFunc
void func_ov009_02155664(DWCRapRequestFunc func) {
    data_ov009_021a8264->requestFunc = func;
}

// pokebw2's DWCRap_NewClientCallback
static void func_ov009_02155674(int index, void *param) {
}

// pokebw2's DWCRap_AttemptCallback
static BOOL func_ov009_02155678(void *param) {
    return data_ov009_021a8264->unk7A5;
}

u32 DWC_AddMatchKeyString(u32 keyID, const char *keyString, const char *valueSrc);
void func_ov009_02156c60(void);
void func_ov009_02156c00(int aid);
void func_ov009_02155374(int state);
static void func_ov009_02155d80(int netId, u8 cmd, void *arg);
static void func_ov009_02155dcc(u8 netId, u8 *data, int size, void *arg);
static void func_ov009_02155e9c(DWCError a0, BOOL a1, BOOL a2, u8 a3, int a4, void *a5);
static void func_ov009_02155bdc(u8 aid, void *arg);

static void func_ov009_02155d3c(DWCError error, BOOL cancel, BOOL a2, BOOL a3, int index, void *param);


BOOL func_ov009_0215568c(const char *key, int numEntry, BOOL a2, int a3) {
    int i;

    GFL_ASSERT(data_ov009_021a8264 != NULL);
    if (data_ov009_021a8264->state != DWCRAP_STATE_READY) {
        return FALSE;
    }
    func_ov009_02156c60();
    GFL_ASSERT(DWC_AddMatchKeyString(0, "a", key));
    MI_CpuFill8(data_ov009_021a8264->randommatch_query, 0, 0x80);
    sprintf(data_ov009_021a8264->randommatch_query, data_ov009_021a1974, "a", key);
    GFL_ASSERT((int)GFL_STD_StrLen(data_ov009_021a8264->randommatch_query) < 0x80);
    if (a2) {
        DWC_AddMatchKeyString(1, data_ov009_021a8264->randommatch_query, data_ov009_021a8264->randommatch_query);
    }
    for (i = 0; i < numEntry; i++) {
        func_ov009_02156c00(i);
    }
    func_ov009_02155374(DWCRAP_STATE_MATCHING);
    data_ov009_021a8264->maxPlayers = numEntry;
    if (!DWC_ConnectToAnybodyAsync(2, numEntry, data_ov009_021a8264->randommatch_query, func_ov009_02155d3c, func_0203d650(), func_ov009_02155674, func_0203d650(), func_ov009_02155d7c, func_0203d650(), func_ov009_02155678, &data_ov009_021a8264->connectResult, func_0203d650())) {
        return FALSE;
    }
    data_ov009_021a8264->matchType = 0;
    func_ov009_02177138(func_ov009_02155d80, 0);
    func_ov009_02177160(func_ov009_02155dcc, 0);
    DWC_SetConnectionClosedCallback(func_ov009_02155e9c, 0);
    func_ov009_02177188(func_ov009_02155bdc, 0);
    data_ov009_021a8264->sending = 0;
    data_ov009_021a8264->unk7A1 = 1;
    return TRUE;
}



static void func_ov009_02155674(int index, void *param);

BOOL func_ov009_0215580c(const char *query, int numEntry, DWCEvalPlayerCallback evalFunc, void *evalWork) {
    int i;

    GFL_ASSERT(data_ov009_021a8264 != NULL);
    if (data_ov009_021a8264->state != DWCRAP_STATE_READY) {
        return FALSE;
    }
    func_ov009_02156c60();
    for (i = 0; i < numEntry; i++) {
        func_ov009_02156c00(i);
    }
    func_ov009_02155374(DWCRAP_STATE_MATCHING);
    data_ov009_021a8264->maxPlayers = numEntry;
    if (!DWC_ConnectToAnybodyAsync(1, numEntry, query, func_ov009_02155d3c, NULL, func_ov009_02155674, NULL, evalFunc, evalWork, NULL, NULL, NULL)) {
        return FALSE;
    }
    data_ov009_021a8264->matchType = 0;
    func_ov009_02177138(func_ov009_02155d80, 0);
    func_ov009_02177160(func_ov009_02155dcc, 0);
    DWC_SetConnectionClosedCallback(func_ov009_02155e9c, 0);
    func_ov009_02177188(func_ov009_02155bdc, 0);
    data_ov009_021a8264->sending = 0;
    data_ov009_021a8264->unk7A1 = 1;
    data_ov009_021a8264->errorCode = 0x3ef;
    return TRUE;
}


// clang-format on

// pokebw2's DWCRap_MatchFailed
static void func_ov009_02155904(void) {
    if (data_ov009_021a8264->state == DWCRAP_STATE_MATCH_FAILED) {
        func_ov009_02155374(DWCRAP_STATE_MATCH_FAILED_END);
    } else {
        func_ov009_02155374(DWCRAP_STATE_MATCH_ENDED);
    }
}

// pokebw2's DWCRap_Process
int func_ov009_0215592c(int a0) {
    u32 result;

    func_ov009_02155b68();
    switch (data_ov009_021a8264->state) {
    case DWCRAP_STATE_INIT:
    case DWCRAP_STATE_INET:
    case DWCRAP_STATE_LOGIN_WAIT:
    case DWCRAP_STATE_LOGIN:
    case DWCRAP_STATE_SERVERS:
    case DWCRAP_STATE_FRIENDS:
        return func_ov009_021554c8();
    case DWCRAP_STATE_MATCHING:
        if (a0 != 0) {
            func_ov009_02155374(DWCRAP_STATE_MATCH_CANCEL);
        }
        if (data_ov009_021a8264->matchType == 2 && data_ov009_021a8264->friendIndex >= 0 && data_ov009_021a8264->friendStatus[data_ov009_021a8264->friendIndex] != 6) {
            func_ov009_02155374(DWCRAP_STATE_MATCH_FAILED);
        }
        break;
    case DWCRAP_STATE_MATCH_CANCEL:
    case DWCRAP_STATE_MATCH_FAILED:
        DWC_CloseAllConnectionsHard();
        func_ov009_02155904();
        break;
    case DWCRAP_STATE_CONNECTED: {
        GFLNetInitData *netInit = func_0203d798();

        if (data_ov009_021a8264->vctEnabled != 0) {
            func_ov009_02156344(netInit->heapId);
        }
        data_ov009_021a8264->unk794 = 0;
        func_ov009_02155374(DWCRAP_STATE_CONNECTED_IDLE);
        data_ov009_021a8264->errorCode = 1000;
        return 1000;
    }
    case DWCRAP_STATE_MATCH_ENDED:
        func_ov009_02155374(DWCRAP_STATE_READY);
        data_ov009_021a8264->sending = 0;
        data_ov009_021a8264->lastNetId = -1;
        data_ov009_021a8264->errorCode = 0x3e9;
        return 0x3e9;
    case DWCRAP_STATE_MATCH_FAILED_END:
        func_ov009_02155374(DWCRAP_STATE_READY);
        data_ov009_021a8264->sending = 0;
        data_ov009_021a8264->lastNetId = -1;
        data_ov009_021a8264->errorCode = 0x3ea;
        return 0x3ea;
    case DWCRAP_STATE_ERROR:
        return func_ov009_02155f58();
    case DWCRAP_STATE_CLOSED:
        if (data_ov009_021a8264->vctActive == 0) {
            DWC_CloseAllConnectionsHard();
            func_ov009_02155374(DWCRAP_STATE_ENDING);
            break;
        }
        // fall through
    case DWCRAP_STATE_READY:
        result = func_ov009_021561c0();
        if (result == 0) {
            data_ov009_021a8264->errorCode = 0x3ef;
            return 0x3ef;
        }
        return result;
    }
    return func_ov009_021561c0();
}

// pokebw2's DWCRap_SendPacket
BOOL func_ov009_02155a80(const void *data, int size, int type) {
    u16 netIds;

    if (size >= 0x100) {
        return FALSE;
    }
    if (data_ov009_021a8264->sending != 0) {
        return FALSE;
    }
    data_ov009_021a8264->header.word = type | (data_ov009_021a8264->unk794 << 8);
    data_ov009_021a8264->sendSeq++;
    data_ov009_021a8264->header.bytes[2] = data_ov009_021a8264->sendSeq;
    MI_CpuCopy8(data, data_ov009_021a8264->data, size);
    data_ov009_021a8264->sending = 1;
    netIds = DWC_GetAIDBitmap();
    if (netIds != DWC_SendReliableBitmap(netIds, &data_ov009_021a8264->header, size + 4)) {
        data_ov009_021a8264->sending = 0;
        return FALSE;
    }
    return TRUE;
}

// pokebw2's DWCRap_SendAck
static BOOL func_ov009_02155b00(const void *data, int size) {
    if (func_ov009_02155a80(data, size, 3)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DWCRap_Send
BOOL func_ov009_02155b14(const void *data, int size) {
    if (data_ov009_021a8264->sendReady == 0) {
        return FALSE;
    }
    if (!func_ov009_02155a80(data, size, 1)) {
        return FALSE;
    }
    data_ov009_021a8264->sendReady = 0;
    if (data_ov009_021a8264->childRecvFunc != NULL) {
        data_ov009_021a8264->childRecvFunc(DWC_GetMyAID(), (u8 *)data, size);
    }
    return TRUE;
}

// pokebw2's DWCRap_FlushAck
static void func_ov009_02155b68(void) {
    if (data_ov009_021a8264->ackPending != 0) {
        u32 data = 0;

        if (func_ov009_02155b00(&data, 4)) {
            data_ov009_021a8264->ackPending = 0;
        }
    }
}

// pokebw2's DWCRap_LoginCallback
static void func_ov009_02155b98(DWCError error, int profileID, void *param) {
    if (func_0205984c(data_ov009_021a8264->userData)) {
        func_02059870(data_ov009_021a8264->userData);
        data_ov009_021a8264->userDataDirty = 1;
    }
    if (error == 0) {
        func_ov009_02155374(DWCRAP_STATE_SERVERS);
    } else {
        func_ov009_02155374(DWCRAP_STATE_ERROR);
    }
}

// pokebw2's DWCRap_TimeoutCallback
static void func_ov009_02155bdc(u8 aid, void *arg) {
    if (aid < DWCRAP_NETID_MAX) {
        if (data_ov009_021a8264->connected != 0) {
            DWC_CloseAllConnectionsHard();
            data_ov009_021a8264->lastNetId = -1;
            func_ov009_02155374(DWCRAP_STATE_ENDED);
        }
    }
}

// pokebw2's DWCRap_UpdateServersCallback
static void func_ov009_02155c0c(DWCError error, BOOL isChanged, void *param) {
    if (error == 0) {
        func_ov009_02155374(DWCRAP_STATE_READY);
    } else {
        func_ov009_02155374(DWCRAP_STATE_ERROR);
    }
}

// pokebw2's DWCRap_FriendStatusCallback
static void func_ov009_02155c24(int index, u8 status, const char *location, void *param) {
}

// pokebw2's DWCRap_DeleteFriendCallback
static void func_ov009_02155c28(int deleteIndex, int srcIndex, void *param) {
    int i;

    func_0203d798();
    for (i = 0; i < DWCRAP_FRIEND_MAX; i++) {
        if (data_ov009_021a8264->pendingCode[i] == -1) {
            data_ov009_021a8264->pendingCode[i] = deleteIndex;
            data_ov009_021a8264->pendingParam[i] = srcIndex;
        }
    }
}

// pokebw2's DWCRap_BuddyCallback
static void func_ov009_02155c6c(int index, void *param) {
}

// pokebw2's DWCRap_ResetIdleFrames
static void func_ov009_02155c70(void) {
    MI_CpuFill8(data_ov009_021a8264->idleFrames, 0, sizeof(data_ov009_021a8264->idleFrames));
}

BOOL DWC_SetRecvTimeoutTime(u8 aid, u32 value);
u8 DWC_GetMyAID(void);
u32 DWC_GetAIDBitmap(void);
void func_ov009_02155c70(void);




static void func_ov009_02155c8c(void) {
    int i;

    for (i = 0; i < 2; i++) {
        DWC_SetRecvTimeoutTime(i, 0);
    }
    if (DWC_GetMyAID() == 0) {
        for (i = 0; i < data_ov009_021a8264->maxPlayers; i++) {
            if (i != DWC_GetMyAID() && (DWC_GetAIDBitmap() & (1 << i))) {
                GFL_ASSERT(DWC_SetRecvTimeoutTime(i, 10000));
            }
        }
    } else {
        GFL_ASSERT(DWC_SetRecvTimeoutTime(0, 10000));
    }
    data_ov009_021a8264->connected = 1;
    func_ov009_02155c70();
}


// clang-format on

// pokebw2's DWCRap_OnMatched
static void func_ov009_02155d2c(int index) {
    func_ov009_02155374(DWCRAP_STATE_CONNECTED);
    func_ov009_02155c8c();
}

// pokebw2's DWCRap_MatchedCallback
static void func_ov009_02155d3c(DWCError error, BOOL cancel, BOOL a2, BOOL a3, int index, void *param) {
    if (error == 0) {
        if (cancel == 0) {
            func_ov009_02155d2c(index);
        } else {
            func_ov009_02155904();
        }
    } else {
        func_ov009_02155374(DWCRAP_STATE_ERROR);
    }
    if (data_ov009_021a8264->connectFunc != NULL) {
        data_ov009_021a8264->connectFunc(DWC_GetMyAID(), data_ov009_021a8264->connectWork);
    }
}

// pokebw2's DWCRap_EvalCallback
static int func_ov009_02155d7c(int index, void *param) {
    return 1;
}

// pokebw2's DWCRap_SendCallback
static void func_ov009_02155d80(int size, u8 aid, void *arg) {
    if (aid < DWCRAP_NETID_MAX) {
        data_ov009_021a8264->sending = 0;
        data_ov009_021a8264->idleFrames[aid] = 0;
    }
}

// pokebw2's DWCRap_SetUnk790
static void func_ov009_02155da4(u32 header) {
    if (header & 0x100) {
        data_ov009_021a8264->unk790 = 1;
    } else {
        data_ov009_021a8264->unk790 = 0;
    }
}

// clang-format off
// pokebw2's DWCRap_RecvCallback
static void func_ov009_02155dcc(u8 aid, u8 *data, int size, void *arg) {
    u32 header = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];

    GFL_ASSERT(aid < 2);
    if (aid < DWCRAP_NETID_MAX) {
        data_ov009_021a8264->connected = 1;
        if ((u8)header == 1) {
            func_ov009_02155da4(header);
            data_ov009_021a8264->recvSeq = data[2];
            data_ov009_021a8264->ackPending = 1;
        } else if ((u8)header == 3) {
            data_ov009_021a8264->sendReady = 1;
            return;
        } else {
            if (func_ov013_021aef44(aid, data, size)) {
                return;
            }
            return;
        }
        if (DWC_GetMyAID() == 0) {
            if (data_ov009_021a8264->parentRecvFunc != NULL) {
                data_ov009_021a8264->parentRecvFunc(aid, data + 4, size - 4);
            }
        } else {
            if (data_ov009_021a8264->childRecvFunc != NULL) {
                data_ov009_021a8264->childRecvFunc(aid, data + 4, size - 4);
            }
        }
    }
}
// clang-format on

// pokebw2's DWCRap_ClosedCallback
static void func_ov009_02155e9c(DWCError error, BOOL a1, BOOL a2, u8 aid, int index, void *param) {
    if (aid < DWCRAP_NETID_MAX) {
        data_ov009_021a8264->sending = 0;
        data_ov009_021a8264->retryCount = 0;
        data_ov009_021a8264->matchingBusy = 0;
        if (error == 0) {
            if (data_ov009_021a8264->unk7A1 == 1 && DWC_GetNumConnectionHost() == 1) {
                if (data_ov009_021a8264->state != DWCRAP_STATE_MATCH_CANCEL) {
                    func_ov009_02155374(DWCRAP_STATE_CLOSED);
                }
                if (data_ov009_021a8264->vctActive != 0) {
                    func_ov009_02156400();
                }
            }
            if (a1 == 0) {
                func_ov009_02155374(DWCRAP_STATE_CLOSED);
            }
        }
        if (data_ov009_021a8264->reportError != 0) {
            GFLNetErrorInfo *info = func_0203ca98();

            if (info->type == 0) {
                func_0203c9ac(info->code, info->unk4, info->unk8, 0x3f1);
            }
        }
        func_0203a338(aid);
        if (data_ov009_021a8264->disconnectFunc != NULL) {
            data_ov009_021a8264->disconnectFunc(aid, data_ov009_021a8264->disconnectWork);
        }
    }
}

u32 getGameVersion(void);
u32 func_ov009_02155f58(void) {
    int code;
    int type;
    DWCError error;
    u32 result;
    u32 errorResult = 0;

    error = DWC_GetLastErrorEx(&code, &type);
    if (error == 0) {
        goto noError;
    }
    if (data_ov009_021a8264->eventFunc != NULL && data_ov009_021a8264->eventFunc(data_ov009_021a8264->eventWork, code,
            type, error) == 1) {
        return 0;
    }
    if (data_ov009_021a8264->errorFunc != NULL) {
        data_ov009_021a8264->errorFunc(data_ov009_021a8264->errorWork, code, type, error);
    }
    errorResult = code;
    if (code == 0 || type == 1) {
        errorResult = error;
    }
    result = errorResult;
    switch (type) {
    case 1:
        DWC_ClearError();
        if (data_ov009_021a8264 != NULL && data_ov009_021a8264->state == DWCRAP_STATE_ERROR) {
            func_ov009_02155374(DWCRAP_STATE_READY);
        }
        break;
    case 2:
        if (code <= -40000 && code >= -41999) {
            func_ov175_021e1340();
        }
        if (data_ov009_021a8264 != NULL && data_ov009_021a8264->state == DWCRAP_STATE_ERROR) {
            func_ov009_02155374(DWCRAP_STATE_READY);
        }
        break;
    case 4:
        if (code <= -40000 && code >= -41999) {
            func_ov175_021e1340();
noError:
            result = errorResult;
        }
        break;
    case 5:
        func_ov114_021c5b80();
        break;
    case 3:
    case 6:
        if (data_ov009_021a8264 != NULL) {
            switch (data_ov009_021a8264->state) {
            case DWCRAP_STATE_LOGIN:
            case DWCRAP_STATE_READY:
            case DWCRAP_STATE_MATCHING:
            case DWCRAP_STATE_MATCH_CANCEL:
            case DWCRAP_STATE_CONNECTED:
            case DWCRAP_STATE_CONNECTED_IDLE:
            case DWCRAP_STATE_MATCH_ENDED:
            case DWCRAP_STATE_ERROR:
            case DWCRAP_STATE_CLOSED:
            case DWCRAP_STATE_ENDING:
            case DWCRAP_STATE_ENDED:
                if (data_ov009_021a8264->friendsMatchActive == 1) {
                    DWC_ShutdownFriendsMatch();
                    data_ov009_021a8264->friendsMatchActive = 0;
                }
                    // fall through
            case DWCRAP_STATE_INIT:
            case DWCRAP_STATE_INET:
            case DWCRAP_STATE_LOGIN_WAIT:
                func_0205cc20();
                break;
            }
        }
        if (data_ov009_021a8264 != NULL) {
            func_ov009_02155374(DWCRAP_STATE_SERVER_ERROR);
        }
        break;
    case 7:
        if (data_ov009_021a8264 != NULL) {
            func_ov009_02155374(DWCRAP_STATE_KICKED);
            if (data_ov009_021a8264->kickFunc != NULL) {
                data_ov009_021a8264->kickFunc(-code);
            }
        }
        break;
    }
    if (data_ov009_021a8264->unk79E != 0) {
        result = 0x3ee;
        func_0203c9ac(code, type, error, result);
    } else if (data_ov009_021a8264->state == DWCRAP_STATE_ENDED) {
        result = 0x3f0;
        func_0203c9ac(code, type, error, result);
    } else if (error != 0) {
        func_0203c9ac(code, type, error, 0);
        getGameVersion();
    }
    return result;
}


// pokebw2's DWCRap_CanPing
static BOOL func_ov009_0215610c(void) {
    int i;
    BOOL found = FALSE;

    for (i = 0; i < data_ov009_021a8264->maxPlayers; i++) {
        if (i != DWC_GetMyAID() && DWC_IsValidAID(i)) {
            found = TRUE;
            if (!DWC_IsSendableReliable(i)) {
                return FALSE;
            }
        }
    }
    return found;
}

// pokebw2's DWCRap_SendIdlePing
static BOOL func_ov009_0215615c(int index) {
    if (data_ov009_021a8264->sending == 0 && func_ov009_0215610c() && (DWC_GetAIDBitmap() & 0xfffe)) {
        data_ov009_021a8264->sending = 1;
        data_ov009_021a8264->header.word = 2 | (data_ov009_021a8264->unk794 << 8);
        DWC_SendReliableBitmap(DWC_GetAIDBitmap(), &data_ov009_021a8264->header, 4);
        data_ov009_021a8264->idleFrames[index] = 0;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DWCRap_Update
static u32 func_ov009_021561c0(void) {
    int i;

    func_ov009_02156cf4();
    DWC_ProcessFriendsMatch();
    func_ov009_021567b8();
    if (data_ov009_021a8264->vctActive != 0) {
        if (data_ov009_021a8264->unk794 == 1 && data_ov009_021a8264->unk790 == 1 && data_ov009_021a8264->unk796 == 1) {
            func_ov013_021aee14(FALSE);
        } else {
            func_ov013_021aee14(TRUE);
        }
        if (data_ov009_021a8264->lastConnectBits != DWC_GetAIDBitmap() && data_ov009_021a8264->micOn == 0 && data_ov009_021a8264->vctEnabled != 0) {
            if (func_ov013_021af238(DWC_GetAIDBitmap(), DWC_GetMyAID())) {
                data_ov009_021a8264->lastConnectBits = DWC_GetAIDBitmap();
            }
        }
    }
    if (data_ov009_021a8264->state == DWCRAP_STATE_ENDED) {
        u32 result = func_ov009_02155f58();

        if (result != 0) {
            return result;
        }
        return 0x3eb;
    } else if (data_ov009_021a8264->state == DWCRAP_STATE_ENDING) {
        return 0x3ec;
    } else if (data_ov009_021a8264->state == DWCRAP_STATE_CONNECTED || data_ov009_021a8264->state == DWCRAP_STATE_CONNECTED_IDLE) {
        for (i = 0; i < data_ov009_021a8264->maxPlayers; i++) {
            if (data_ov009_021a8264->idleFrames[i]++ >= 240 && data_ov009_021a8264->sending == 0 && func_ov009_0215615c(i)) {
                func_ov009_02155c70();
                break;
            }
        }
    }
    return func_ov009_02155f58();
}

// pokebw2's DWCRap_GetNetId
int func_ov009_021562ec(void) {
    if (data_ov009_021a8264 != NULL) {
        if (data_ov009_021a8264->state == DWCRAP_STATE_CONNECTED || data_ov009_021a8264->state == DWCRAP_STATE_CONNECTED_IDLE ||
            data_ov009_021a8264->state == DWCRAP_STATE_CLOSED) {
            return DWC_GetMyAID();
        }
    }
    return -1;
}

// pokebw2's DWCRap_VctEndCallback
static void func_ov009_0215631c(void) {
    data_ov009_021a8264->vctActive = 0;
}

void func_ov009_02156330(void) {
    data_ov009_021a8264->unk794 = 1;
}

// pokebw2's DWCRap_StartVct
static void func_ov009_02156344(HeapID heapId) {
    int mode;

    data_ov009_021a8264->unk794 = 1;
    data_ov009_021a8264->unk790 = 1;
    data_ov009_021a8264->unk796 = 1;
    if (data_ov009_021a8264->vctActive == 0) {
        switch (data_ov009_021a8264->vctMode) {
        case 2:
            mode = 1;
            break;
        case 3:
            mode = 2;
            break;
        case 4:
            mode = 3;
            break;
        case 5:
            mode = 4;
            break;
        default:
            mode = 4;
            break;
        }
        func_ov013_021aef6c(heapId, mode, 1);
        func_ov013_021af18c(func_ov009_0215631c);
        data_ov009_021a8264->vctActive = 1;
    }
}

// pokebw2's DWCRap_StartVoiceChat
void func_ov009_021563c0(void) {
    func_ov009_02156344(func_0203d798()->heapId);
}

// pokebw2's DWCRap_StopVoiceChat
void func_ov009_021563d0(void) {
    if (data_ov009_021a8264->vctActive != 0) {
        func_ov013_021af13c();
    }
}

// pokebw2's DWCRap_IsVoiceChatActive
int func_ov009_021563ec(void) {
    return data_ov009_021a8264->vctActive;
}

// pokebw2's DWCRap_ResetVoiceChat
void func_ov009_02156400(void) {
    func_ov013_021af1a0();
    if (data_ov009_021a8264 != NULL) {
        data_ov009_021a8264->vctActive = 0;
        data_ov009_021a8264->lastConnectBits = 0;
        data_ov009_021a8264->unk794 = 0;
    }
}

// pokebw2's DWCRap_SetMic
void func_ov009_0215642c(BOOL on) {
    if (on) {
        func_ov013_021af2ac(DWC_GetMyAID());
        data_ov009_021a8264->micOn = TRUE;
    } else {
        data_ov009_021a8264->micOn = FALSE;
    }
    func_ov013_021af30c(data_ov009_021a8264->micOn);
}

// clang-format off
// pokebw2's DWCRap_GetErrorKind
int func_ov009_02156460(int code, int type) {
    int hundreds = code / 100;
    int thousands = code / 1000;

    if (code == 20101) {
        return 1;
    }
    if (thousands == 23) {
        return 1;
    }
    if (code == 20108) {
        return 2;
    }
    if (code == 20110) {
        return 3;
    }
    if (hundreds == 512) {
        return 4;
    }
    if (hundreds == 500) {
        return 5;
    }
    if (code == 51103) {
        return 6;
    }
    if (hundreds == 510) {
        return 6;
    }
    if (hundreds == 511) {
        return 6;
    }
    if (hundreds == 513) {
        return 6;
    }
    if (code >= 52000 && code <= 52003) {
        return 8;
    }
    if (code >= 52010 && code <= 52012) {
        return 8;
    }
    if (code >= 52100 && code <= 52103) {
        return 8;
    }
    if (code >= 52110 && code <= 52112) {
        return 8;
    }
    if (code >= 52200 && code <= 52203) {
        return 8;
    }
    if (code >= 52210 && code <= 52212) {
        return 8;
    }
    if (code >= 52400 && code <= 52403) {
        return 8;
    }
    if (code >= 52410 && code <= 52412) {
        return 8;
    }
    if (code >= 52500 && code <= 52503) {
        return 8;
    }
    if (code >= 52510 && code <= 52512) {
        return 8;
    }
    if (code >= 52700 && code <= 52703) {
        return 8;
    }
    if (code >= 52710 && code <= 52712) {
        return 8;
    }
    if (code == 80430) {
        return 9;
    }
    if (thousands == 20) {
        return 0;
    }
    if (hundreds == 520) {
        return 0;
    }
    if (hundreds == 521) {
        return 0;
    }
    if (hundreds == 522) {
        return 0;
    }
    if (hundreds == 523) {
        return 0;
    }
    if (hundreds == 530) {
        return 0;
    }
    if (hundreds == 531) {
        return 0;
    }
    if (hundreds == 532) {
        return 0;
    }
    if (code < 10000) {
        return 14;
    }
    if (thousands == 31) {
        return 12;
    }
    switch (type) {
    case 0:
    case 1:
        return 14;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return 11;
    default:
        GFL_ASSERT(0);
        break;
    case 7:
        return 15;
    }
    return -1;
}
// clang-format on

// pokebw2's DWCRap_RequestClose
BOOL func_ov009_02156690(BOOL a0) {
    int code;
    int type;

    if (DWC_GetLastErrorEx(&code, &type)) {
        DWC_ClearError();
    }
    if (a0 == 0) {
        switch (data_ov009_021a8264->state) {
        case DWCRAP_STATE_MATCHING:
        case DWCRAP_STATE_CONNECTED:
        case DWCRAP_STATE_CONNECTED_IDLE:
            if (data_ov009_021a8264->vctActive != 0) {
                func_ov013_021af13c();
            }
            func_ov009_02155374(DWCRAP_STATE_CLOSED);
            break;
        case DWCRAP_STATE_READY:
        case DWCRAP_STATE_SERVER_ERROR:
        case DWCRAP_STATE_ENDING:
        case DWCRAP_STATE_ENDED:
            return TRUE;
        }
    } else {
        switch (data_ov009_021a8264->state) {
        case DWCRAP_STATE_READY:
        case DWCRAP_STATE_ENDING:
        case DWCRAP_STATE_ENDED:
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's DWCRap_ResetToReady
BOOL func_ov009_02156730(void) {
    if (data_ov009_021a8264->state == DWCRAP_STATE_ENDING || data_ov009_021a8264->state == DWCRAP_STATE_ENDED ||
        data_ov009_021a8264->state == DWCRAP_STATE_SERVER_ERROR || data_ov009_021a8264->state == DWCRAP_STATE_READY) {
        func_ov009_02155374(DWCRAP_STATE_READY);
        data_ov009_021a8264->errorCode = 0x3ef;
        data_ov009_021a8264->lastNetId = -1;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DWCRap_Shutdown
void func_ov009_0215677c(void) {
    if (data_ov009_021a8264 != NULL) {
        DWC_ClearError();
        if (data_ov009_021a8264->friendsMatchActive != 0) {
            DWC_ShutdownFriendsMatch();
        }
        data_ov009_021a8264->friendsMatchActive = 0;
        func_0205cc20();
        func_ov009_02156400();
        func_ov009_021554a0();
    }
}

// pokebw2's DWCRap_UpdateFriendStatus
static void func_ov009_021567b8(void) {
    if (data_ov009_021a8264->friendList != NULL) {
        u32 index = data_ov009_021a8264->friendCheckIndex & 0x1f;

        if (func_0205912c(&data_ov009_021a8264->friendList[index])) {
            int size;
            u8 status = DWC_GetFriendStatusData(&data_ov009_021a8264->friendList[index], data_ov009_021a8264->friendStatusData[index], &size);

            if (size < 1) {
                data_ov009_021a8264->friendStatus[index] = 0;
                MI_CpuFill8(data_ov009_021a8264->friendStatusData[index], 0, MYDWC_STATUS_DATA_SIZE_MAX);
            } else {
                data_ov009_021a8264->friendStatus[index] = status;
            }
        }
        data_ov009_021a8264->friendCheckIndex++;
    }
}

BOOL DWC_SetOwnStatusData(const char *statusData, u32 size);
void func_ov009_0215683c(const char *data, int size) {
    GFL_ASSERT(size <= MYDWC_STATUS_DATA_SIZE_MAX);
    DWC_SetOwnStatusData(data, size);
}


// clang-format on

// pokebw2's DWCRap_GetFriendStatusData
char *func_ov009_02156860(int index) {
    return data_ov009_021a8264->friendStatusData[index];
}

// pokebw2's DWCRap_GetFriendStatus
u8 func_ov009_02156874(int index) {
    return data_ov009_021a8264->friendStatus[index];
}

// pokebw2's DWCRap_Connect
int func_ov009_02156888(int friendIndex, int numEntry, BOOL a2) {
    int i;
    int entries = numEntry;
    BOOL ok;

    if (func_ov009_02156c84()) {
        return -4;
    }
    if (data_ov009_021a8264->state != DWCRAP_STATE_READY) {
        data_ov009_021a8264->retryCount++;
        if (data_ov009_021a8264->retryCount > 120) {
            return -3;
        }
        return -1;
    }
    func_ov009_02156c60();
    data_ov009_021a8264->connectBits = 0;
    data_ov009_021a8264->unk7A1 = 1;
    data_ov009_021a8264->friendIndex = friendIndex;
    data_ov009_021a8264->maxPlayers = numEntry;
    data_ov009_021a8264->lastIndex = -1;
    if (a2) {
        entries = 2;
    }
    data_ov009_021a8264->matchingBusy = 1;
    if (friendIndex < 0) {
        ok = DWC_SetupGameServer(2, entries, func_ov009_02156a58, func_0203d650(), func_ov009_02156b3c,
                                 func_0203d650(), func_ov009_02155678, &data_ov009_021a8264->connectResult, func_0203d650());
        data_ov009_021a8264->matchType = 1;
    } else {
        ok = DWC_ConnectToGameServerAsync(2, friendIndex, func_ov009_02156b18, NULL, func_ov009_02156b3c,
                                 func_0203d650(), func_ov009_02155678, &data_ov009_021a8264->connectResult, func_0203d650());
        data_ov009_021a8264->matchType = 2;
    }
    if (!ok) {
        data_ov009_021a8264->retryCount++;
        if (data_ov009_021a8264->retryCount > 120) {
            func_0203c9ac(0, 0, 0, 0x3f1);
        }
        return -2;
    }
    data_ov009_021a8264->retryCount = 0;
    for (i = 0; i < numEntry; i++) {
        func_ov009_02156c00(i);
    }
    func_ov009_02155374(DWCRAP_STATE_MATCHING);
    func_ov009_02177138(func_ov009_02155d80, 0);
    func_ov009_02177160(func_ov009_02155dcc, 0);
    DWC_SetConnectionClosedCallback(func_ov009_02155e9c, 0);
    func_ov009_02177188(func_ov009_02155bdc, 0);
    data_ov009_021a8264->sending = 0;
    return 0;
}

// pokebw2's DWCRap_GetFriendIndex
int func_ov009_02156a3c(void) {
    if (data_ov009_021a8264 != NULL) {
        return data_ov009_021a8264->friendIndex;
    }
    return -1;
}

// pokebw2's DWCRap_AnybodyMatchedCallback
static void func_ov009_02156a58(DWCError error, BOOL cancel, BOOL a2, BOOL a3, int index, void *param) {
    BOOL failed = FALSE;

    data_ov009_021a8264->matchingBusy = 0;
    if (error == 0) {
        if (cancel == 0) {
            if (data_ov009_021a8264->unk7A4 != 0 && index == -1) {
                failed = TRUE;
            }
            if (data_ov009_021a8264->requestFunc != NULL) {
                if (!data_ov009_021a8264->requestFunc(index, func_0203d650())) {
                    failed = TRUE;
                }
            }
            if (data_ov009_021a8264->unk7A0 != 0 || failed) {
                if (index != -1) {
                    DWC_CloseConnectionHard(index);
                }
            }
            data_ov009_021a8264->friendIndex = index;
            data_ov009_021a8264->connectBits = DWC_GetAIDBitmap();
            if (data_ov009_021a8264->connectBits == 1) {
                func_ov009_02155374(DWCRAP_STATE_MATCH_CANCEL);
            } else {
                func_ov009_02155d2c(index);
            }
        } else if (a2 == 0) {
            data_ov009_021a8264->lastIndex = index;
            data_ov009_021a8264->lastNetId = -1;
        }
    }
}

// pokebw2's DWCRap_FriendMatchedCallback
static void func_ov009_02156b18(DWCError error, BOOL cancel, BOOL a2, BOOL a3, int index, void *param) {
    data_ov009_021a8264->matchingBusy = 0;
    if (error == 0 && cancel == 0) {
        func_ov009_02155d2c(index);
    }
}

// pokebw2's DWCRap_ConnectedCallback
static void func_ov009_02156b3c(int index, void *param) {
    data_ov009_021a8264->lastNetId = index;
    if (data_ov009_021a8264->connectFunc != NULL) {
        data_ov009_021a8264->connectFunc(index, data_ov009_021a8264->connectWork);
    }
}

BOOL func_ov009_02156b68(void) {
    return func_ov013_021af210();
}

s16 func_ov009_02156b70(void) {
    if (data_ov009_021a8264 != NULL) {
        return data_ov009_021a8264->vctEnabled;
    }
    return 0;
}

// pokebw2's DWCRap_GetLastNetId
int func_ov009_02156b8c(void) {
    if (data_ov009_021a8264 != NULL) {
        return data_ov009_021a8264->lastNetId;
    }
    return 0;
}

s16 func_ov009_02156ba8(void) {
    if (data_ov009_021a8264 != NULL) {
        return data_ov009_021a8264->lastIndex;
    }
    return -1;
}

void func_ov009_02156bc4(s16 a0) {
    data_ov009_021a8264->vctEnabled = a0;
}

// pokebw2's DWCRap_FreeRecvBuffer
static void func_ov009_02156bd8(int aid) {
    if (data_ov009_021a8264->recvBuf[aid] != NULL) {
        func_0203d7fc(data_ov009_021a8264->recvBuf[aid]);
        data_ov009_021a8264->recvBuf[aid] = NULL;
    }
}

// pokebw2's DWCRap_AllocRecvBuffer
static void func_ov009_02156c00(int aid) {
    GFLNetInitData *netInit = func_0203d798();

    func_ov009_02156bd8(aid);
    if (data_ov009_021a8264->recvBuf[aid] == NULL) {
        data_ov009_021a8264->recvBuf[aid] = allocConfigDSSoftwareFeature(netInit->heapId, 0x1000, "dwc_rap.c", 2932);
        DWC_SetRecvBuffer(aid, data_ov009_021a8264->recvBuf[aid], 0x1000);
    }
}

// pokebw2's DWCRap_FreeRecvBuffers
static void func_ov009_02156c60(void) {
    int i;

    for (i = 0; i < DWCRAP_NETID_MAX; i++) {
        func_ov009_02156bd8(i);
    }
    data_ov009_021a8264->sendReady = 1;
}

// pokebw2's DWCRap_IsUserDataDirty
BOOL func_ov009_02156c84(void) {
    return data_ov009_021a8264->userDataDirty;
}

// pokebw2's DWCRap_ClearUserDataDirty
void func_ov009_02156c98(void) {
    data_ov009_021a8264->userDataDirty = 0;
}

// pokebw2's DWCRap_IsReady
BOOL func_ov009_02156cac(void) {
    if (data_ov009_021a8264 != NULL) {
        if (data_ov009_021a8264->state == DWCRAP_STATE_READY) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// pokebw2's DWCRap_IsConnected
BOOL func_ov009_02156cd0(void) {
    if (data_ov009_021a8264 != NULL && (data_ov009_021a8264->state == DWCRAP_STATE_CONNECTED || data_ov009_021a8264->state == DWCRAP_STATE_CONNECTED_IDLE)) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov009_02156cf4(void) {
    int i;

    for (i = 0; i < DWCRAP_FRIEND_MAX; i++) {
        if (data_ov009_021a8264->pendingCode[i] != -1) {
            GFLNetInitData *netInit = func_0203d798();

            if (netInit->unk34.wifiError.callback != NULL) {
                netInit->unk34.wifiError.callback(data_ov009_021a8264->pendingCode[i], data_ov009_021a8264->pendingParam[i], func_0203d650());
            }
            data_ov009_021a8264->pendingCode[i] = -1;
        }
    }
}


// pokebw2's DWCRap_GetStatus
int func_ov009_02156d5c(void) {
    return data_ov009_021a8264->errorCode;
}

void func_ov009_02156d70(int a0) {
    data_ov009_021a8264->unk7A5 = a0;
}

// pokebw2's DWCRap_IsEnding
BOOL func_ov009_02156d84(void) {
    if (data_ov009_021a8264 == NULL) {
        return TRUE;
    }
    if (data_ov009_021a8264->state == DWCRAP_STATE_ENDING || data_ov009_021a8264->state == DWCRAP_STATE_SERVER_ERROR ||
        data_ov009_021a8264->state == DWCRAP_STATE_ENDED) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DWCRap_SetReportError
void func_ov009_02156db0(int a0) {
    data_ov009_021a8264->reportError = a0;
}

// pokebw2's DWCRap_SetErrorFunc
void func_ov009_02156dc4(DWCRapErrorFunc func, void *work) {
    data_ov009_021a8264->errorFunc = func;
    data_ov009_021a8264->errorWork = work;
}

// pokebw2's DWCRap_SetEventFunc
void func_ov009_02156de0(DWCRapEventFunc func, void *work) {
    data_ov009_021a8264->eventFunc = func;
    data_ov009_021a8264->eventWork = work;
}

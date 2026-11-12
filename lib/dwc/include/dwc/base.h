// DWC's base library (lib/dwc/src/base): the declarations its files share, Black's layouts for the fields they read
#ifndef BWPORT_DWC_BASE_H
#define BWPORT_DWC_BASE_H

#include "types.h"
#include "dwc/dwc.h"
#include "gs/gp.h"
#include "gs/gt2.h"
#include "gs/serverbrowsing.h"

// dwc_common.c: the key-value strings the friend code reads its common values from
int DWC_SetCommonKeyValueString(const char *key, const char *value, char *string, char separator);
int DWC_AddCommonKeyValueString(const char *key, const char *value, char *string, char separator);
int DWC_GetCommonValueString(const char *key, char *value, const char *string, char separator);
s32 DWCi_GetMathRand32(u32 max);
u32 DWCi_WStrLen(const u16 *str);

// dwc_friend_base.c
// Starts the count of the GP requests in progress (pokebw2's DWCi_InitGPProcessCount)
void DWCi_InitGPProcessCount(void);
// Returns the index of a friend in the friend list by profile ID
int DWCi_GetFriendListIndex(int profileID);
// Sets the GP status string and data of a profile
int DWCi_SetGPStatus(int profileID, char *statusString, char *statusData);

// dwc_login.c: the login state the friend code checks
BOOL DWCi_CheckLogin(void);
DWCUserData *DWCi_GetUserData(void);
void DWCi_StopLogin(int dwcError, int errorCode);

// dwc_main.c
// The version of the matching protocol (Black's)
#define DWC_MATCHING_VERSION 90

// The connections the match keeps (Black's; pokebw2 has 32 too)
#define DWC_MAX_CONNECTIONS 32
#define DWC_MATCH_NN_FAILURE_MAX 5

// The info of an established connection (NitroDWC's DWCConnectionInfo, Black's layout: aid at 1, param at 4)
typedef struct DWCConnectionInfo {
    u8 index;
    u8 aid;
    u16 reserve;
    void *param;
} DWCConnectionInfo;

// The GT2 connections and their infos, by index (data_ov009_021aa8a0, data_ov009_021aa920 in Black)
extern GT2Connection data_ov009_021aa8a0[DWC_MAX_CONNECTIONS];
extern DWCConnectionInfo data_ov009_021aa920[DWC_MAX_CONNECTIONS];

// Whether an AID is one of the connected machines' (pokebw2's DWCi_IsValidAID)
BOOL DWCi_IsValidAID(u8 aid);
GT2Connection *DWCi_GetGT2ConnectionByProfileID(int profileID, int numHost);
// The index of a connection in the connection list
int DWCi_GT2GetConnectionListIdx(void);
// Handles a GT2 error of a connection (pokebw2's DWCi_HandleGT2Error)
int func_ov009_0216bea4(u8 aid);
int DWCi_HandleGPError_dwc_main(int result);
GT2Connection *DWCi_GetGT2ConnectionByIdx(int index);
DWCConnectionInfo *DWCi_GetConnectionInfoByIdx(int index);

// The match control (NitroDWC's DWCMatchControl) DWCi_GetMatchCnt returns: the DWCFriendsMatchControl's matchcnt, at 0x350.
// Black's layout, whose fields from 0x28 on differ from NitroDWC's (0x7cc bytes); fields named by offset where unknown
typedef struct DWCMatchSocket {
    GT2Socket socket;
} DWCMatchSocket;

// A match entry, its byte 0x16 is the player's handle
// The most profile IDs a match command carries (NitroDWC's)
#define DWC_MAX_MATCH_IDX_LIST 64

typedef struct DWCMatchEntry {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u16 unk_c;
    u16 unk_e;
    u32 unk_10;
    u16 unk_14;
    u8 unk_16;
    u8 unk_17;
    s32 unk_18;
    u64 unk_1c;
    u32 unk_24;
} DWCMatchEntry;

// NitroDWC's DWCNNInfo: the info of a NAT negotiation
typedef struct DWCNNInfo {
    u8 isQR2;
    u8 retryCount;
    u16 port;
    u32 ip;
    int cookie;
} DWCNNInfo;

// Called with a value, the match control's state at 0x2c and its parameter at 0x38
typedef void (*DWCStateChangedFunc)(s32 arg, u32 state, u32 param);

typedef struct DWCMatchState {
    GPConnection *unk_0;
    DWCMatchSocket *unk_4;
    GT2ConnectionCallbacks *unk_8;
    u8 unk_c;
    u8 unk_d;
    u8 unk_e[0x2];
    void *unk_10;
    vu8 unk_14;
    vu8 unk_15;
    vu8 unk_16;
    vu8 unk_17;
    u8 unk_18;
    u8 unk_19[0x3];
    u32 unk_1c;
    u64 unk_20;
    u32 unk_28;
    u32 unk_2c;
    u8 unk_30;
    u8 unk_31[0x3];
    DWCStateChangedFunc unk_34;
    u32 unk_38;
    u64 unk_3c;
    u32 unk_44;
    u32 unk_48;
    u8 unk_4c; // cleared at the shutdown (NitroDWC's qr2ShutdownFlag)
    u8 unk_4d[0x3];
    s32 entryCount; // the match entries, 0x20 at most
    DWCMatchEntry entries[0x20];
    DWCMatchEntry unk_554;
    DWCMatchEntry unk_57c;
    u32 unk_5a4;
    u32 unk_5a8;
    u8 unk_5ac[0x28];
    u16 unk_5d4;
    u8 unk_5d6[0x2];
    u32 unk_5d8;
    u32 unk_5dc;
    ServerBrowser unk_5e0;
    s32 unk_5e4;
    u64 unk_5e8;
    u64 sbUpdateRequestTick;
    u8 nnRecvCount;
    u8 nnFailureCount;
    u16 nnCookieRand;
    int nnLastCookie;
    u64 nnFailedTime;
    u64 nnFinishTime;
    DWCNNInfo nnInfo;
    u32 unk_61c;
    u8 unk_620;
    u8 unk_621;
    u8 unk_622[0x6];
    u8 unk_628;
    u8 unk_629;
    u8 unk_62a;
    u8 unk_62b[0x4];
    u8 unk_62f;
    u16 unk_630;
    u16 unk_632;
    u32 unk_634;
    u32 unk_638;
    u64 unk_63c;
    u32 unk_644;
    u64 unk_648;
    u32 unk_650;
    u32 unk_654;
    u64 unk_658;
    u64 unk_660;
    u64 unk_668;
    u64 unk_670;
    vu32 unk_678; // NitroDWC's volatile int profileID
    u32 unk_67c;
    s32 unk_680;
    const char *unk_684;
    const char *unk_688;
    DWCFriendData *unk_68c;
    int unk_690;
    u8 unk_694[0x40];
    s32 unk_6d4;
    u8 unk_6d8;
    u8 unk_6d9;
    u16 unk_6da;
    u32 unk_6dc;
    u8 unk_6e0[0x80];
    u32 unk_760;
    s32 unk_764;
    u64 unk_768;
    DWCMatchedSCCallback unk_770;
    void *unk_774;
    DWCNewClientCallback unk_778;
    void *unk_77c;
    DWCEvalPlayerCallback evalCallback;
    void *evalParam;
    u32 unk_788;
    u32 unk_78c;
    u32 unk_790;
    u8 unk_794[0x4];
    u32 unk_798;
    u8 unk_79c[0x4];
    u64 unk_7a0;
    u8 unk_7a8;
    u8 unk_7a9[0x3];
    u32 unk_7ac;
    u32 unk_7b0;
    u8 unk_7b4;
    u8 unk_7b5;
    u8 unk_7b6;
    u8 unk_7b7;
    s32 (*unk_7b8)(u32, u32);
    u32 unk_7bc;
    u32 unk_7c0;
    u32 unk_7c4;
    u32 unk_7c8;
    u64 unk_7cc;
    u32 unk_7d4;
} DWCMatchState;

// A machine's transport connection, 0x38 bytes each; only the fields the functions read are named
typedef struct DWCTransportConnection {
    const void *unk_0;
    void *unk_4;
    int unk_8;
    int unk_c;
    int unk_10;
    int unk_14;
    int unk_18;
    u8 sendState;
    u8 recvState;
    u8 unk_1e;
    u8 unk_1f;
    u16 unk_20;
    u16 unk_22;
    u64 unk_24;
    u32 unk_2c;
    u64 unk_30;
} DWCTransportConnection;

// Called with a value, the message's netId and the field at 0x71c
typedef void (*DWCTransFunc)(u32 arg, u8 netId, u32 value);

// The transport's state: the connections, then the field at 0x724 that the functions keep
typedef struct DWCTransport {
    DWCTransportConnection connections[0x20];
    DWCUserSendCallback unk_700;
    DWCUserRecvCallback unk_704;
    DWCUserRecvTimeoutCallback unk_708;
    DWCTransFunc unk_70c;
    int unk_710;
    int unk_714;
    int unk_718;
    u32 unk_71c;
    u16 unk_720;
    u8 unk_722[2];
    u32 unk_724;
} DWCTransport;

// The DWC control's state, login and update-servers callbacks, at offsets (NitroDWC's DWCFriendsMatchControl;
// only the fields dwc_main.c and dwc_login.c read, Black's offsets)

// The login control, at 0x90 of the friends and match control (NitroDWC's DWCLoginControl, Black's offsets)
typedef struct DWCLoginControl {
    GPConnection *pGpObj;                   // 0x0
    int state;                              // 0x4
    int productID;                          // 0x8
    u32 gameCode;                           // 0xc
    u16 *playerName;                        // 0x10
    DWCLoginCallback callback;          // 0x14
    void *param;                            // 0x18
    DWCUserData *userData;                  // 0x1c
    u8 unk_20[0x30 - 0x20];
    s32 unk_30;                             // 0x30
    u64 unk_34;                             // 0x34
    DWCLoginIdData unk_3c;                  // 0x3c
    char authToken[0x100];                  // 0x48
    char partnerChallenge[0x100];           // 0x148
    u8 unk_248[0x260 - 0x248];
} DWCLoginControl;

typedef struct DWCFriendsMatchControl {
    GT2Socket gt2Socket;                    // 0x0
    GT2ConnectionCallbacks gt2Callbacks;    // 0x4
    int gt2SendBufSize;                     // 0x14
    int gt2RecvBufSize;                     // 0x18
    GPConnection gpObj;                     // 0x1c
    DWCUserData *userData;                  // 0x20
    int state;                              // 0x24
    int lastState;                          // 0x28
    u8 aid;                                 // 0x2c
    u8 ownCloseFlag;                        // 0x2d
    u8 unk_2e[0x2];
    u16 playerName[(0x64 - 0x30) / 2];      // 0x30
    int profileID;                          // 0x64
    const char *gameName;                   // 0x68
    const char *secretKey;                  // 0x6c
    DWCLoginCallback loginCallback;     // 0x70
    void *loginParam;                       // 0x74
    DWCUpdateServersCallback updateServersCallback; // 0x78
    void *updateServersParam;               // 0x7c
    DWCMatchedSCCallback matchedSCCallback; // 0x80
    void *matchedSCParam;                   // 0x84
    DWCConnectionClosedCallback closedFunc; // 0x88
    void *closedParam; // 0x8c
    DWCLoginControl logcnt;                 // 0x90
    DWCFriendControl friendcnt;             // 0x2f0
    DWCMatchState matchcnt;                 // 0x350, the match control DWCi_GetMatchCnt returns
    DWCTransport transinfo;                 // 0xb28
} DWCFriendsMatchControl;

// The DWC control block (Black's data_ov009_021aa878; its pointer to the friends and match control at 4)
typedef struct DWCControl {
    u32 unk_0;
    DWCFriendsMatchControl *block;
} DWCControl;

// The GT2 connection's info that dwc_main.c reads (Black's, by offset)
typedef struct DWCGt2Info {
    u32 unk_0;
    u8 unk_4[0x16 - 4];
    u8 unk_16;
} DWCGt2Info;

// Functions dwc_main.c calls that are not written yet (Black's dsd names)
GT2Connection DWCi_GetGT2Connection(u8 aid);
DWCGt2Info *func_ov009_02176410(u8 aid);
// Closes a connection: by its GT2 connection, or (with flag set) by the AID given; reason is a GT2CloseReason
void func_ov009_0216c230(GT2Connection connection, GT2CloseReason reason, BOOL flag, u8 aid);
void func_ov009_0216ec18(void);

// dwc_match.c



DWCMatchState *DWCi_GetMatchCnt(void);
BOOL func_ov009_02175458(void);
u64 func_ov009_02176594(void);
int func_ov009_0217576c(BOOL force);
BOOL func_ov009_021758c4(u8 aid, s32 value, s32 mode);
// Starts the match control: its GP connection, GT2 socket and callbacks, the game's name and secret key, the friends
void func_ov009_0216cc28(DWCMatchState *matchcnt, GPConnection *pGpObj, GT2Socket *pGt2Socket,
                         GT2ConnectionCallbacks *gt2Callbacks, const char *gameName, const char *secretKey,
                         DWCFriendData *friendList, int friendListLen);
void func_ov009_021728b8(void);

void DWCi_NNFreeNegotiateList(void);
// Starts a random match (NitroDWC's DWCi_ConnectToAnybodyAsync)
void func_ov009_0216cf1c(int type, u8 numEntry, const char *addFilter, DWCMatchedSCCallback matched, void *matchedParam,
                         DWCNewClientCallback newClient, void *newClientParam, DWCEvalPlayerCallback eval, void *evalParam,
                         DWCAttemptFunc attempt, int *attemptParam, void *param);
// Sets up a game server (NitroDWC's DWCi_SetupGameServer)
void func_ov009_0216d138(int type, u8 numEntry, DWCMatchedSCCallback matched, void *matchedParam, DWCNewClientCallback newClient,
                         void *newClientParam, DWCAttemptFunc attempt, int *attemptParam, void *param);
// Connects to a game server (NitroDWC's DWCi_ConnectToGameServerAsync)
void func_ov009_0216d27c(int type, int profileID, DWCMatchedSCCallback matched, void *matchedParam, DWCNewClientCallback newClient,
                         void *newClientParam, DWCAttemptFunc attempt, int *attemptParam, void *param);
// Tells the server browser the matching status
int func_ov009_0216f20c(int status);
void func_ov009_02175f4c(s32 param_1);
void func_ov009_0217601c(void);
// The number of machines connected
int func_ov009_0217603c(void);
void DWCi_CloseMatching(void);
// Handles a match message of a buddy: the message's command byte first
void func_ov009_0216eab8(GPConnection *pconnection, GPProfile profile, u8 *message);
BOOL DWCi_IsShutdownMatch(void);
// Returns the server's entry, or NULL
DWCMatchEntry *func_ov009_02176358(void);
int func_ov009_021754fc(void);
s32 func_ov009_02176674(void);
BOOL func_ov009_0217695c(void);
BOOL func_ov009_02175fc8(void);
// Handles the match's closing of a connection, returning whether it did
BOOL func_ov009_0216f0a0(DWCError error, int errorCode, int profileID);
// Handles the closing of a connection of a server, by the profile ID
void func_ov009_0216f1e8(s32 profileID);
// Runs the match, with whether the match is the one in progress
void func_ov009_0216d57c(BOOL flag);
void func_ov009_0216f334(void);
// The callback of a GT2 connection attempt (NitroDWC's DWCi_GT2ConnectAttemptCallback)
void func_ov009_0216e5b0(GT2Socket socket, GT2Connection connection, u32 ip, u16 port, int latency, GT2Byte *message, int len);
// The callback of a GT2 message from an unknown address
GT2Bool DWCi_GT2UnrecognizedMessageCallback(GT2Socket socket, u32 ip, u16 port, u8 *message, s32 len);
// The callback of a connected GT2 connection
void func_ov009_0216e834(GT2Connection connection, GT2Result result, GT2Byte *message, int len);

// dwc_match.c: stops the match with the error, or the friend process
void DWCi_StopMatching(DWCError error, int errorCode);
void DWCi_StopFriendProcess(DWCError error, int errorCode);
// dwc_friend_base.c: the profile ID of a friend list index, and the buddy callbacks
int DWCi_GetProfileIDFromList(int index);
BOOL DWCi_GPRecvBuddyAuthCallback(GPConnection *connection, GPRecvBuddyAuthArg *arg, void *param);
void DWCi_GPRecvBuddyRequestCallback(GPConnection *connection, GPRecvBuddyRequestArg *arg, void *param);
void DWCi_GPRecvBuddyStatusCallback(GPConnection *connection, GPRecvBuddyStatusArg *arg, void *param);

// Not written yet: the match entry of an index
DWCMatchEntry *func_ov009_0217637c(int index);
// Sends a message's part on a connection (pokebw2's func_ov011_0216ec4c)
void func_ov009_0216ec4c(u8 aid, u16 state, void *buffer);
// Not written yet: whether a buddy's status is a matching one
BOOL func_ov009_02176984(const GPBuddyStatus *status);

// dwc_login.c
void func_ov009_0216a404(void);
BOOL DWCi_LoginAsync(void);
void DWCi_LoginProcess(void);
void DWCi_LoginInit(DWCLoginControl *logcnt, DWCUserData *userData, GPConnection *pGpObj, int productID, u32 gameCode,
                    u16 *playerName, DWCLoginCallback callback, void *param);
// dwc_friend_base.c
void func_ov009_02169ff8(void);
void func_ov009_0216a00c(void);
void CloseStatsConnection(void);
void DWCi_ShutdownFriend(void);
void DWCi_FriendProcess(void);
void DWCi_UpdateServersAsync(const char *authToken, const char *partnerChallenge,
                             DWCUpdateServersCallback updateCallback, void *updateParam,
                             DWCFriendStatusCallback statusCallback, void *statusParam,
                             DWCDeleteFriendListCallback deleteCallback, void *deleteParam);
void DWCi_FriendInit(DWCFriendControl *friendcnt, GPConnection *pGpObj, const u16 *playerName,
                     DWCFriendData *friendList, int friendListLen);

// dwc_transport.c
void DWCi_ShutdownTransport(void);
void DWCi_ClearTransConnection(u8 aid);
void func_ov009_021773d8(void);
void DWCi_InitTransport(DWCTransport *transport);
void func_ov009_02177848(u8 type, u8 key, const void *message, int size, int flag);

#endif // BWPORT_DWC_BASE_H

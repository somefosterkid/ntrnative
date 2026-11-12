// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/dwc.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_DWC_H
#define BWPORT_DWC_DWC_H

#include "types.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "gs/gp.h"

// The player's account and a friend's, as the save keeps them: DWCAccUserData and DWCAccFriendData of the SDK
// A 12-byte login ID as Black's save keeps it
typedef struct DWCLoginIdData {
    u32 data[3];
} DWCLoginIdData;

typedef struct DWCUserData {
    u32 unk_0;
    DWCLoginIdData unk_4;
    u32 unk_10[3];
    s32 unk_1c;
    u32 flag;
    u32 unk_24;
    u8 unk_28[0x14];
    u32 crc32;
} DWCUserData;

typedef struct DWCFriendData {
    u32 data[3];
} DWCFriendData;

// The Wi-Fi ID's info as the save keeps it
typedef struct DWCWiFiInfo {
    u64 attestedUserId; // 43 bits; 0 while no ID has been confirmed
    u64 notAttestedId;  // 43 bits; made from the console's MAC address, the time and the salt
    u16 pass;           // 10 bits; a random number from 0 to 999
    u16 randomHistory;  // 16 bits; part of what the pending ID is made from
} DWCWiFiInfo;

void DWCi_BM_GetWiFiInfo(DWCWiFiInfo *info);

// What the Wi-Fi ID is given out as: the two IDs, and whether the first is set
typedef struct DWCAuthWiFiId {
    u64 uId;
    u64 notAttestedId;
    u32 flag;
} DWCAuthWiFiId;

// The error that none has set, and the numbers that the part that failed gives (the one at 9 stays: nothing replaces it)
typedef enum DWCError {
    DWC_ERROR_NONE = 0,
    DWC_ERROR_DS_MEMORY_ANY = 1,
    DWC_ERROR_AUTH_ANY = 2,
    DWC_ERROR_AUTH_OUT_OF_SERVICE = 3,
    DWC_ERROR_AUTH_STOP_SERVICE = 4,
    DWC_ERROR_AC_ANY = 5,
    DWC_ERROR_NETWORK = 6,
    DWC_ERROR_GHTTP_ANY = 7,
    DWC_ERROR_DISCONNECTED = 8,
    DWC_ERROR_FATAL = 9,
    DWC_ERROR_FRIENDS_SHORTAGE = 10,
    DWC_ERROR_NOT_FRIEND_SERVER = 11,
    DWC_ERROR_SERVER_FULL = 12,
    DWC_ERROR_ND_ANY = 13,
    DWC_ERROR_ND_HTTP = 14,
    DWC_ERROR_SVL_ANY = 15,
    DWC_ERROR_SVL_HTTP = 16,
    DWC_ERROR_PROF_ANY = 17,
    DWC_ERROR_PROF_HTTP = 18,
} DWCError;

// The callbacks of the friend list's update: its end, with whether the list changed; a friend's status; a friend that
// was a copy of another and was deleted; and a friend that became a buddy
typedef void (*DWCUpdateServersCallback)(DWCError error, BOOL isChanged, void *param);
typedef void (*DWCFriendStatusCallback)(int index, u8 status, const char *statusString, void *param);
typedef void (*DWCDeleteFriendListCallback)(int deletedIndex, int srcIndex, void *param);
typedef void (*DWCBuddyFriendCallback)(int index, void *param);
// The callbacks of the persistent storage server's login, saving and loading
typedef void (*DWCStorageLoginCallback)(DWCError error, void *param);
typedef void (*DWCSaveToServerCallback)(BOOL success, BOOL isPublic, void *param);
typedef void (*DWCLoadFromServerCallback)(BOOL success, int index, char *data, int len, void *param);

// Asked how well another machine fits a random match, and the function's work
typedef int (*DWCEvalPlayerCallback)(int index, void *work);

// Matching: with the key and number, anybody, or a friend, and the callbacks that tell how it went
typedef void (*DWCMatchedSCCallback)(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index, void *param);
typedef void (*DWCNewClientCallback)(int index, void *param);
typedef BOOL (*DWCAttemptFunc)(void *param);

// The functions called when data has been sent, when it has been received and when a machine has not sent for too long,
// with the parameter they were set with, and the one called when a connection to another machine has been closed
typedef void (*DWCUserSendCallback)(int size, u8 netId, void *param);
typedef void (*DWCUserRecvCallback)(u8 netId, u8 *data, int size, void *param);
typedef void (*DWCUserRecvTimeoutCallback)(u8 netId, void *param);
typedef void (*DWCConnectionClosedCallback)(DWCError error, BOOL isLocal, BOOL isServer, u8 aid, int index, void *param);

// Clears the error, unless it is 9
void DWC_ClearError(void);

// Starts the connection to the Nintendo Wi-Fi Connection with the access point's name (an empty one takes the
// default) and the buffer of the connection, returning FALSE if it could not start
BOOL DWC_SVLGetTokenAsync(const char *apName, void *buffer);
// The connection's state, 3 once connected, ending it after a state from 3 to 5
int DWC_SVLProcess(void);

// Whether two friends' data name the same friend
BOOL DWC_IsEqualFriendData(const DWCFriendData *friendData, const DWCFriendData *other);
// pokebw2's DWC_GetGsProfileId
s32 func_020598ec(const DWCUserData *userData, const DWCFriendData *friendData);
// pokebw2's DWC_IsValidFriendData
BOOL func_020596d8(const DWCFriendData *friendData);
// Whether a friend key is valid for the player's user data, and the friend data of a friend key
BOOL func_02059354(const DWCUserData *userData, u64 friendKey);
void DWC_CreateFriendKeyToken(DWCFriendData *token, u64 friendKey);
// Main's: the inet part and the user data and friend data parts
void func_0205c758(void);
BOOL func_0205c88c(void);
int func_0205ca20(void);
void func_0205c8e0(void);
void DWC_InitFriendsMatch(DWCUserData *userData, int productId, const char *secretKey, int sendBufSize,
                          int recvBufSize, DWCFriendData *friendList, int friendListLen);
typedef void (*DWCLoginCallback)(DWCError error, int profileId, void *param);
BOOL DWC_LoginAsync(const u16 *ingamesn, const char *reserved, DWCLoginCallback callback, void *param);
void DWC_ProcessFriendsMatch(void);
BOOL DWC_UpdateServersAsync(const char *playerName, DWCUpdateServersCallback updateCallback, void *updateParam,
                            DWCFriendStatusCallback statusCallback, void *statusParam,
                            DWCDeleteFriendListCallback deleteCallback, void *deleteParam);
BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void *param);
BOOL func_ov009_02177138(DWCUserSendCallback func, void *param);
BOOL func_ov009_02177160(DWCUserRecvCallback func, void *param);
BOOL DWC_SetConnectionClosedCallback(DWCConnectionClosedCallback func, void *param);
BOOL func_ov009_02177188(DWCUserRecvTimeoutCallback func, void *param);
int DWC_CloseAllConnectionsHard(void);
// Not in dwc_rap.c, past it in the overlay
u32 DWC_GetAIDBitmap(void);
u32 DWC_SendReliableBitmap(u32 netIds, const void *data, int size);
// Sends one frame of packet memory to the machine with the given ID (dwc_transport.c)
BOOL func_ov009_02177044(u8 aid, const void *buffer, int size);
// The machines: their IDs and which are there
u8 DWC_GetMyAID(void);
// An AID (dwc_main.c's: func_ov009_0216b9b0's, else the first match entry's; 0xff for none)
u8 func_ov009_0216c800(void);
BOOL func_0205984c(void *userData);
void func_02059870(void *userData);
int DWC_GetNumConnectionHost(void);
void DWC_ShutdownFriendsMatch(void);
BOOL DWC_IsValidAID(u8 netId);
BOOL DWC_IsSendableReliable(u8 netId);
BOOL func_0205912c(const DWCFriendData *friendData);
u8 DWC_GetFriendStatusData(const DWCFriendData *friendData, char *statusData, int *size);
u8 DWC_GetFriendStatus(const DWCFriendData *friendData, char *statusString);
u8 DWC_GetFriendStatusDataSC(const DWCFriendData *friendData, u8 *maxEntry, u8 *numEntry, char *statusData, int *size);
BOOL func_ov009_02168d7c(void);
void DWC_Free(int heap, void *ptr, int unused);
void DWCi_SetError(int heap, u32 code);
BOOL DWCi_IsError(void);
void *DWC_Alloc(int heap, u32 size);
void func_020592a0(DWCFriendData *friendData, int value);
int func_020591cc(const DWCFriendData *friendData);
BOOL DWC_IsBuddyFriendData(const DWCFriendData *friendData);
void DWCi_SetBuddyFriendData(DWCFriendData *friendData);
void func_02059240(DWCFriendData *friendData, int value);
void func_02059270(DWCFriendData *friendData, int value);
void DWC_SetGsProfileId(DWCFriendData *friendData, int profileID);
s32 func_02059b40(const char *src, u32 srcLen, char *dst, u32 dstSize);
s32 func_02059cbc(const char *src, s32 srcLen, u8 *dst, u32 dstSize);

// The friend list's states
typedef enum DWCFriendState {
    DWC_FRIEND_STATE_INIT = 0,
    DWC_FRIEND_STATE_PERS_LOGIN,
    DWC_FRIEND_STATE_LOGON,
    DWC_FRIEND_STATE_NUM,
} DWCFriendState;

// The friend list's state, which dwc_match.c gives the memory for
typedef struct DWCFriendControl {
    // 0 until the update starts, 1 while it runs and 2 once it is over
    DWCFriendState state;
    GPConnection *pGpObj;
    // How many times GameSpy's GP has been run, and when it was last
    u32 gpProcessCount;
    OSTick lastGpProcess;
    int friendListLen;
    DWCFriendData *friendList;
    // The friend that the update is at, whether it changed the list, and its step: 0 to read the buddy list, 1 to go
    // through the friends, 2 while a profile search runs and 3 once the friends are done
    u8 buddyUpdateIdx;
    u8 friendListChanged;
    u8 buddyUpdateState;
    // How many parts of the update are left: the friends, and the reverse buddies of the profile IDs
    u8 updateSteps;
    // Nonzero while the persistent storage server is logged in to
    int persLogin;
    // The player's profile ID, which nothing sets
    int profileID;
    // The player's name
    const u16 *playerName;
    DWCUpdateServersCallback updateCallback;
    void *updateParam;
    DWCFriendStatusCallback statusCallback;
    void *statusParam;
    DWCDeleteFriendListCallback deleteCallback;
    void *deleteParam;
    DWCBuddyFriendCallback buddyCallback;
    void *buddyParam;
    // The callbacks of the persistent storage server's login, saving and loading, which nothing sets
    DWCStorageLoginCallback persLoginCallback;
    void *persLoginParam;
    DWCSaveToServerCallback saveCallback;
    DWCLoadFromServerCallback loadCallback;
    // The profile IDs whose reverse buddies are asked for
    int *profileIds;
} DWCFriendControl;
BOOL DWC_SetupGameServer(int type, u8 numEntry, DWCMatchedSCCallback matched, void *matchedParam,
                         DWCNewClientCallback newClient, void *newClientParam, DWCAttemptFunc attempt,
                         int *attemptParam, void *param);
BOOL DWC_ConnectToGameServerAsync(int type, int friendIndex, DWCMatchedSCCallback matched, void *matchedParam,
                         DWCNewClientCallback newClient, void *newClientParam, DWCAttemptFunc attempt,
                         int *attemptParam, void *param);
BOOL DWC_ConnectToAnybodyAsync(int type, u8 numEntry, const char *addFilter, DWCMatchedSCCallback matched, void *matchedParam,
                               DWCNewClientCallback newClient, void *newClientParam, DWCEvalPlayerCallback eval, void *evalParam,
                               DWCAttemptFunc attempt, int *attemptParam, void *param);
int DWC_CloseConnectionHard(u8 netId);
BOOL DWC_SetRecvBuffer(u8 netId, void *buffer, int size);

// The server's date and time
BOOL DWC_GetDateTime(RTCDate *date, RTCTime *time);
u32 DWC_AddMatchKeyInt(u32 keyId, const char *key, const int *valueSrc);
int DWC_GetMatchIntValue(int index, const char *key, int defaultValue);
// Asks the server whether count words are bad, and runs the request, returning 2 once it succeeded
BOOL func_ov009_0217bc44(const u16 **words, int count, const char *reserved, int timeout, char *result,
                         int *badWordCount, int region);
int DWC_CheckProfanityProcess(void);

// Starts connecting to the access point; one of the calls that end the connection after a server error; and the
// connection's state
BOOL DWC_NASLoginAsync(void);
int func_ov009_02168124(void);
// The friend key of the player's user data
u64 func_0205994c(const DWCUserData *userData);

// pokebw2's DWC_CheckHasProfile
BOOL DWC_CheckHasProfile(DWCUserData *userData);
// pokebw2's DWC_CheckValidConsole
BOOL DWC_CheckValidConsole(DWCUserData *userData);

// Black's TWLDWC functions in main_0205a26c.c, whose files aren't known: the allocator SOCL is given (func_0205a26c to
// func_0205a2b4). The NVRAM reads and writes of the Wi-Fi settings and their checks are bm/bm_internal.h's
void func_0205a26c(u32 param_1);
void func_0205a280(void *param_1, u32 param_2);
void func_0205a29c(void *param_1);
void func_0205a2b4(u32 param_1, u32 param_2);

void DWC_CreateExchangeToken(const DWCUserData *userData, DWCFriendData *token);

#endif // BWPORT_DWC_DWC_H

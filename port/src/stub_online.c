// The PC build's online play: none. Stubs for the DWC (Nintendo Wi-Fi Connection), dwcutil (the WFC settings),
// GameSpy and NitroWiFi functions and data that the compiled C (src/, lib/gfl, lib/poke_net, lib/spl) calls or
// reads, which lib/dwc, lib/dwcutil and lib/nitrowifi define in the ROM build (the PC build compiles none of them).
//
// What they do: the console has no Wi-Fi settings and finds no access point, so the connection to the internet
// fails the way the DS's does when it finds none (the AC's error 5 with no access point seen: DWC_ERROR_AC_ANY with
// the code -50099, whose type 6 makes the game close the connection and show the error). Everything that needs a
// connection (log-in, matchmaking, downloads, HTTP, GameSpy's competition and storage, voice chat, sockets) fails to
// start, with DWC's "disconnected" error set if no other is. The offline parts keep their meaning: the error store,
// the save's user data (its CRC32 and dirty flag), friend keys and friend data (the same layout and checksum as the
// SDK's, so a save keeps working on a DS), and the Wi-Fi settings read at boot (blank). The return values are those
// the callers' failure paths test for (read from the callers in src/ov009, ov013, ov114, ov121, ov123, ov135, ov175,
// src/system/wih_dwc.c and lib/poke_net). lib/dpw, which isn't compiled either, has port/src/stub_dpw.c.
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/apinfo.h"
#include "dwc/bm.h"
#include "dwc/connectinet.h"
#include "dwc/crypt.h"
#include "dwc/gdb.h"
#include "dwc/nd.h"
#include "dwc/nhttp.h"
#include "dwc/sc.h"
#include "dwc/utility.h"
#include "dwc/vct.h"
#include "gfl/dwc_vchat.h"
// struct_decls.h has the SOCInAddr typedef soc.h uses (our nitro/os.h brought it in; libntr's doesn't)
#include "struct_decls.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"

// ---- the prototypes the game declares outside these headers (the same as its callers') ----------------------------

// include/gfl/net_devwifi.h and lib/dwc/include/dwc/error.h (error.h's DWC_ERROR_* macros clash with dwc.h's enum)
int DWC_GetLastErrorEx(int *code, int *type);
// src/ov009/dwc_rap.c
u32 DWC_AddMatchKeyString(u32 keyID, const char *keyString, const char *valueSrc);
BOOL DWC_SetOwnStatusData(const char *statusData, u32 size);
BOOL DWC_SetRecvTimeoutTime(u8 aid, u32 value);
// src/ov009/dwc_rapcommon.c
BOOL DWC_CheckUserData(void *userData);
void func_020596fc(u32 *param_1);
int func_02059f6c(const char *gameName, u32 gameCode, void *(*alloc)(u32, u32, int), void (*free)(u32, void *, u32));
// include/app/wifi_note.h
u64 DWC_GetFriendKey(const DWCFriendData *friendData);
// lib/dwc/include/dwc/nhttp_internal.h (which includes host and game headers a stub doesn't need)
void func_ov114_021be698(void);
int func_ov114_021be838(int connection, char **header);
int func_ov114_021bf184(int handle);
int func_ov114_021bf308(int handle);
int func_ov114_021bf488(int handle);
int func_ov114_021bf4b0(int handle, int *received, int *size);
// src/ov175/wifibattlematch_net.c's report calls
int func_ov175_021df23c(int a0, int a1, int a2, ScReport **connection);
int func_ov175_021df298(ScReport *connection);
int func_ov175_021df2c8(ScReport *connection);
int func_ov175_021df2f8(ScReport *connection);
int func_ov175_021df328(ScReport *connection);
int func_ov175_021df358(ScReport *connection, int index, u8 *unk30, int a3, int mode,
                        const AuthCertificate *certificate, u8 *unk58);
int func_ov175_021df3b0(ScReport *connection, int a1, int a2);

// ---- the error store (dwc_error.c's behaviour) --------------------------------------------------------------------

// The AC's code for a search that ended without seeing an access point (dwc_ac_result.c's GetNotFoundAP)
#define STUB_CODE_NO_ACCESS_POINT -50099
// dwc_connectinet.c's LINK_LOST_CODE: the code of a connection that is gone
#define STUB_CODE_DISCONNECTED -54000
// The states of DWC's SVL token, NAS log-in and profanity check that mean "failed" (poke_net_internal.h's
// DWC_SVL_STATE_ERROR; the SDK's NAS log-in has the same order; DWC_CheckProfanityProcess's 2 is success)
#define STUB_SVL_STATE_ERROR 4
#define STUB_NASLOGIN_STATE_ERROR 4
#define STUB_PROF_STATE_FAILED 3
// Sc_Startup's and Gdb_Startup's failure (lib/dwc/include/dwc/sc.h)
#define STUB_SC_ERROR 7

static struct {
    int error;
    int code;
} sStubError;

static void StubDwc_SetError(int error, int code) {
    if (sStubError.error != DWC_ERROR_FATAL) {
        sStubError.error = error;
        sStubError.code = code;
    }
}

// What a call that needs a connection leaves: DWC's "disconnected", unless an error is already set
static void StubDwc_NoConnection(void) {
    if (sStubError.error == DWC_ERROR_NONE) {
        StubDwc_SetError(DWC_ERROR_DISCONNECTED, STUB_CODE_DISCONNECTED);
    }
}

int DWC_GetLastErrorEx(int *code, int *type) {
    if (code != NULL) {
        *code = sStubError.code;
    }
    if (type != NULL) {
        switch (sStubError.error) {
        case 2: case 3: case 4: case 5: case 8: case 14: case 16: case 18:
            *type = 6;
            break;
        case 6:
            *type = 3;
            break;
        case 7:
            *type = 4;
            break;
        case 10: case 11: case 12: case 21:
            *type = 1;
            break;
        case 1: case 9:
            *type = 7;
            break;
        case 13:
            *type = 5;
            break;
        case 15: case 17: case 19: case 20: case 22:
            *type = 2;
            break;
        default:
            *type = 0;
            break;
        }
    }
    return sStubError.error;
}

void DWC_ClearError(void) {
    if (sStubError.error != DWC_ERROR_FATAL) {
        sStubError.error = DWC_ERROR_NONE;
        sStubError.code = 0;
    }
}

// ---- DWC_Init and the library's allocator --------------------------------------------------------------------------

// What DWC_Init (func_02059f6c) was given: the user data's game code and the game's allocator
static u32 sStubGameCode = 0x4952414a;
static void *(*sStubAlloc)(u32, u32, int);
static void (*sStubFree)(u32, void *, u32);

// DWC_Init: 0 is DWC_INIT_RESULT_NOERROR (3 would show the erased-user-information warning)
int func_02059f6c(const char *gameName, u32 gameCode, void *(*alloc)(u32, u32, int), void (*free)(u32, void *, u32)) {
    (void)gameName;
    sStubGameCode = gameCode;
    sStubAlloc = alloc;
    sStubFree = free;
    return 0;
}

void *DWC_Alloc(int heap, u32 size) {
    return sStubAlloc != NULL ? sStubAlloc((u32)heap, size, 32) : NULL;
}

void DWC_Free(int heap, void *ptr, int unused) {
    if (sStubFree != NULL && ptr != NULL) {
        sStubFree((u32)heap, ptr, (u32)unused);
    }
}

// ---- the save's user data and friend data (dwc_account.c's layout) ------------------------------------------------

// The SDK's CRC32 of the user data: MATH_CalcCRC32 with the reversed table of 0xedb88320
static u32 StubDwc_Crc32(const void *data, u32 size) {
    const u8 *p = data;
    u32 crc = 0xffffffff;
    u32 i;
    int bit;

    for (i = 0; i < size; i++) {
        crc ^= p[i];
        for (bit = 0; bit < 8; bit++) {
            crc = (crc >> 1) ^ (0xedb88320 & (0 - (crc & 1)));
        }
    }
    return ~crc;
}

// A friend data's or login ID's flags (bits 11 to 31 of its first word) and their data type (the low two bits:
// 0 none, 1 login ID, 2 friend key, 3 GameSpy profile ID)
static u32 StubDwc_DataType(const u32 *data) {
    return (data[0] >> 11) & 3;
}

static void StubDwc_SetDataType(u32 *data, u32 type) {
    data[0] = (data[0] & ~(3u << 11)) | (type << 11);
}

static u64 StubDwc_FriendKeyOf(const DWCFriendData *friendData) {
    return ((u64)friendData->data[2] << 32) | friendData->data[1];
}

// The friend key of a profile ID: the ID with the checksum (dwc_account.c's func_020592d0, with MD5)
static u64 StubDwc_MakeFriendKey(u32 profileId, u32 gameCode) {
    MATHMD5Context ctx;
    u32 data[2];
    u8 hash[16];

    data[0] = profileId;
    data[1] = gameCode;
    MATH_MD5Init(&ctx);
    MATH_MD5Update(&ctx, data, sizeof(data));
    MATH_MD5GetHash(&ctx, hash);
    return ((u64)((hash[0] >> 1) & 0x7f) << 32) | profileId;
}

static BOOL StubDwc_IsValidFriendKey(u64 friendKey, u32 gameCode) {
    s32 id = (s32)(u32)friendKey;

    return id > 0 && StubDwc_MakeFriendKey((u32)id, gameCode) == friendKey;
}

BOOL DWC_CheckUserData(void *userData) {
    return StubDwc_Crc32(userData, 0x3c) == ((DWCUserData *)userData)->crc32;
}

// DWC_CreateUserData: a user data with no login ID and no profile yet (the DS takes its login ID from the console's
// Wi-Fi ID, which a PC hasn't), marked dirty
void func_020596fc(u32 *param_1) {
    DWCUserData *userData = (DWCUserData *)param_1;

    MI_CpuFill8(userData, 0, sizeof(DWCUserData));
    userData->unk_0 = sizeof(DWCUserData);
    userData->unk_24 = sStubGameCode;
    userData->crc32 = StubDwc_Crc32(userData, 0x3c);
    userData->flag |= 1;
}

// DWC_CheckDirtyFlag
BOOL func_0205984c(void *userData) {
    return (((DWCUserData *)userData)->flag & 1) != 0;
}

// DWC_ClearDirtyFlag
void func_02059870(void *userData) {
    DWCUserData *data = userData;

    data->flag &= ~1u;
    data->crc32 = StubDwc_Crc32(data, 0x3c);
}

BOOL DWC_CheckHasProfile(DWCUserData *userData) {
    return userData->unk_1c > 0;
}

// The console is the one the user data was made on (a PC has no Wi-Fi ID to tell otherwise)
BOOL DWC_CheckValidConsole(DWCUserData *userData) {
    (void)userData;
    return TRUE;
}

void DWC_CreateExchangeToken(const DWCUserData *userData, DWCFriendData *token) {
    MI_CpuFill8(token, 0, sizeof(DWCFriendData));
    if (userData->unk_1c <= 0) {
        *token = *(const DWCFriendData *)&userData->unk_4;
        return;
    }
    token->data[1] = (u32)userData->unk_1c;
    StubDwc_SetDataType(token->data, 3);
}

void DWC_CreateFriendKeyToken(DWCFriendData *token, u64 friendKey) {
    MI_CpuFill8(token, 0, sizeof(DWCFriendData));
    token->data[1] = (u32)friendKey;
    token->data[2] = (u32)(friendKey >> 32);
    StubDwc_SetDataType(token->data, 2);
}

u64 DWC_GetFriendKey(const DWCFriendData *friendData) {
    return StubDwc_DataType(friendData->data) == 2 ? StubDwc_FriendKeyOf(friendData) : 0;
}

BOOL DWC_IsEqualFriendData(const DWCFriendData *friendData, const DWCFriendData *other) {
    u32 type = StubDwc_DataType(friendData->data);

    if (type != StubDwc_DataType(other->data)) {
        return FALSE;
    }
    switch (type) {
    case 3:
        return friendData->data[1] == other->data[1];
    case 1:
        return (friendData->data[0] & 0x7ff) == (other->data[0] & 0x7ff) && friendData->data[1] == other->data[1] &&
               friendData->data[2] == other->data[2];
    case 2:
        return StubDwc_FriendKeyOf(friendData) == StubDwc_FriendKeyOf(other);
    }
    return FALSE;
}

// Whether a friend data is a buddy's that is in the friend list (flags 4 and 8): none ever is offline
BOOL func_0205912c(const DWCFriendData *friendData) {
    u32 flags = friendData->data[0] >> 11;

    return StubDwc_DataType(friendData->data) == 3 && (flags & 4) && (flags & 8);
}

// DWC_CheckFriendKey
BOOL func_02059354(const DWCUserData *userData, u64 friendKey) {
    return StubDwc_IsValidFriendKey(friendKey, userData->unk_24);
}

// DWC_IsValidFriendData
BOOL func_020596d8(const DWCFriendData *friendData) {
    return StubDwc_DataType(friendData->data) != 0;
}

// DWC_GetGsProfileId
s32 func_020598ec(const DWCUserData *userData, const DWCFriendData *friendData) {
    u64 friendKey;

    switch (StubDwc_DataType(friendData->data)) {
    case 2:
        friendKey = StubDwc_FriendKeyOf(friendData);
        return StubDwc_IsValidFriendKey(friendKey, userData->unk_24) ? (s32)(u32)friendKey : 0;
    case 3:
        return (s32)friendData->data[1];
    case 1:
        return -1;
    }
    return 0;
}

// DWC_CreateFriendKey: 0 until the player has a profile
u64 func_0205994c(const DWCUserData *userData) {
    return userData->unk_1c != 0 ? StubDwc_MakeFriendKey((u32)userData->unk_1c, userData->unk_24) : 0;
}

// ---- the connection to the internet (dwc_connectinet.c's interface) ----------------------------------------------

static DWCInetControl *sStubInet;

// DWC_InitInetEx
void func_0205c6ac(DWCInetControl *control, u16 dmaNo, u16 powerMode, u16 sslPriority) {
    (void)powerMode;
    (void)sslPriority;
    MI_CpuFill8(control, 0, sizeof(DWCInetControl));
    control->dmaNo = dmaNo;
    control->powerMode = 1;
    control->state = DWC_CONNECTINET_STATE_IDLE;
    sStubInet = control;
}

// DWC_ConnectInetAsync: the search starts...
void func_0205c758(void) {
    if (sStubInet != NULL) {
        sStubInet->state = DWC_CONNECTINET_STATE_OPERATING;
    }
}

// DWC_CheckInet: ...and ends at the first check without an access point
BOOL func_0205c88c(void) {
    if (sStubInet == NULL) {
        return TRUE;
    }
    if (sStubInet->state == DWC_CONNECTINET_STATE_OPERATING) {
        sStubInet->result = STUB_CODE_NO_ACCESS_POINT;
        sStubInet->state = DWC_CONNECTINET_STATE_ERROR;
        sStubInet->ended = 1;
        StubDwc_SetError(DWC_ERROR_AC_ANY, STUB_CODE_NO_ACCESS_POINT);
    }
    return TRUE;
}

// DWC_ProcessInet
void func_0205c8e0(void) {
}

// DWC_GetInetStatus
int func_0205ca20(void) {
    return sStubInet != NULL ? sStubInet->state : DWC_CONNECTINET_STATE_NOT_INITIALIZED;
}

// DWC_CleanupInet
void func_0205cc20(void) {
    sStubInet = NULL;
}

// DWC_GetLinkLevel: the weakest
int func_0205ccd8(void) {
    return 0;
}

// DWC_UpdateConnection: no connection, so none is lost
BOOL func_0205d074(void) {
    return FALSE;
}

// DWCi_GetBssDescParseWorkSize: the work func_0205d01c is given (wih_dwc.c allocates it)
u32 func_0205d010(void) {
    return 4;
}

// DWCi_BssDescToApInfo: no beacon is a Nintendo access point's
BOOL func_0205d01c(DWCBssWork *work, const WMBssDesc *bss, DWCApInfo *apInfo) {
    (void)work;
    (void)bss;
    (void)apInfo;
    return FALSE;
}

// The console's four pages of Wi-Fi settings (main.c reads them at boot): a console that was never set up
DWCBmReadResult func_0205faa8(DWCMemPage *pages, int unused) {
    (void)unused;
    MI_CpuFill8(pages, 0, 4 * sizeof(DWCMemPage));
    return DWC_BM_READ_OK;
}

// ---- the friends match: log-in, servers, matchmaking and transport ------------------------------------------------

void DWC_InitFriendsMatch(DWCUserData *userData, int productId, const char *secretKey, int sendBufSize,
                          int recvBufSize, DWCFriendData *friendList, int friendListLen) {
    (void)userData, (void)productId, (void)secretKey, (void)sendBufSize, (void)recvBufSize, (void)friendList;
    (void)friendListLen;
}

BOOL DWC_LoginAsync(const u16 *ingamesn, const char *reserved, DWCLoginCallback callback, void *param) {
    (void)ingamesn, (void)reserved, (void)callback, (void)param;
    StubDwc_NoConnection();
    return FALSE;
}

void DWC_ProcessFriendsMatch(void) {
}

void DWC_ShutdownFriendsMatch(void) {
}

BOOL DWC_UpdateServersAsync(const char *playerName, DWCUpdateServersCallback updateCallback, void *updateParam,
                            DWCFriendStatusCallback statusCallback, void *statusParam,
                            DWCDeleteFriendListCallback deleteCallback, void *deleteParam) {
    (void)playerName, (void)updateCallback, (void)updateParam, (void)statusCallback, (void)statusParam;
    (void)deleteCallback, (void)deleteParam;
    StubDwc_NoConnection();
    return FALSE;
}

BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void *param) {
    (void)callback, (void)param;
    return TRUE;
}

BOOL DWC_SetConnectionClosedCallback(DWCConnectionClosedCallback func, void *param) {
    (void)func, (void)param;
    return TRUE;
}

// DWC_SetUserSendCallback, DWC_SetUserRecvCallback, DWC_SetUserRecvTimeoutCallback
BOOL func_ov009_02177138(DWCUserSendCallback func, void *param) {
    (void)func, (void)param;
    return TRUE;
}

BOOL func_ov009_02177160(DWCUserRecvCallback func, void *param) {
    (void)func, (void)param;
    return TRUE;
}

BOOL func_ov009_02177188(DWCUserRecvTimeoutCallback func, void *param) {
    (void)func, (void)param;
    return TRUE;
}

// Not logged in: no status to set
BOOL DWC_SetOwnStatusData(const char *statusData, u32 size) {
    (void)statusData, (void)size;
    return FALSE;
}

// Every friend is offline (0), with no status data
u8 DWC_GetFriendStatusData(const DWCFriendData *friendData, char *statusData, int *size) {
    (void)friendData, (void)statusData;
    if (size != NULL) {
        *size = 0;
    }
    return 0;
}

u32 DWC_AddMatchKeyInt(u32 keyId, const char *key, const int *valueSrc) {
    (void)keyId, (void)key, (void)valueSrc;
    return 0;
}

u32 DWC_AddMatchKeyString(u32 keyID, const char *keyString, const char *valueSrc) {
    (void)keyID, (void)keyString, (void)valueSrc;
    return 0;
}

int DWC_GetMatchIntValue(int index, const char *key, int defaultValue) {
    (void)index, (void)key;
    return defaultValue;
}

BOOL DWC_SetupGameServer(int type, u8 numEntry, DWCMatchedSCCallback matched, void *matchedParam,
                         DWCNewClientCallback newClient, void *newClientParam, DWCAttemptFunc attempt,
                         int *attemptParam, void *param) {
    (void)type, (void)numEntry, (void)matched, (void)matchedParam, (void)newClient, (void)newClientParam;
    (void)attempt, (void)attemptParam, (void)param;
    StubDwc_NoConnection();
    return FALSE;
}

BOOL DWC_ConnectToGameServerAsync(int type, int friendIndex, DWCMatchedSCCallback matched, void *matchedParam,
                                  DWCNewClientCallback newClient, void *newClientParam, DWCAttemptFunc attempt,
                                  int *attemptParam, void *param) {
    (void)type, (void)friendIndex, (void)matched, (void)matchedParam, (void)newClient, (void)newClientParam;
    (void)attempt, (void)attemptParam, (void)param;
    StubDwc_NoConnection();
    return FALSE;
}

BOOL DWC_ConnectToAnybodyAsync(int type, u8 numEntry, const char *addFilter, DWCMatchedSCCallback matched,
                               void *matchedParam, DWCNewClientCallback newClient, void *newClientParam, DWCEvalPlayerCallback eval,
                               void *evalParam, DWCAttemptFunc attempt, int *attemptParam, void *param) {
    (void)type, (void)numEntry, (void)addFilter, (void)matched, (void)matchedParam, (void)newClient;
    (void)newClientParam, (void)eval, (void)evalParam, (void)attempt, (void)attemptParam, (void)param;
    StubDwc_NoConnection();
    return FALSE;
}

int DWC_CloseAllConnectionsHard(void) {
    return 0;
}

int DWC_CloseConnectionHard(u8 netId) {
    (void)netId;
    return -1;
}

u32 DWC_GetAIDBitmap(void) {
    return 0;
}

u8 DWC_GetMyAID(void) {
    return 0;
}

int DWC_GetNumConnectionHost(void) {
    return 0;
}

BOOL DWC_IsValidAID(u8 netId) {
    (void)netId;
    return FALSE;
}

BOOL DWC_IsSendableReliable(u8 netId) {
    (void)netId;
    return FALSE;
}

u32 DWC_SendReliableBitmap(u32 netIds, const void *data, int size) {
    (void)netIds, (void)data, (void)size;
    return 0;
}

BOOL DWC_SetRecvBuffer(u8 netId, void *buffer, int size) {
    (void)netId, (void)buffer, (void)size;
    return FALSE;
}

BOOL DWC_SetRecvTimeoutTime(u8 aid, u32 value) {
    (void)aid, (void)value;
    return FALSE;
}

// ---- NAS: the server's time, the SVL token, the NAS log-in, the profanity check ---------------------------------

// No server time without a NAS log-in
BOOL DWC_GetDateTime(RTCDate *date, RTCTime *time) {
    (void)date, (void)time;
    return FALSE;
}

BOOL DWC_NASLoginAsync(void) {
    StubDwc_NoConnection();
    return FALSE;
}

// DWC_NASLoginProcess
int func_ov009_02168124(void) {
    StubDwc_NoConnection();
    return STUB_NASLOGIN_STATE_ERROR;
}

BOOL DWC_SVLGetTokenAsync(const char *apName, void *buffer) {
    (void)apName, (void)buffer;
    StubDwc_NoConnection();
    return FALSE;
}

int DWC_SVLProcess(void) {
    StubDwc_NoConnection();
    return STUB_SVL_STATE_ERROR;
}

// DWC_CheckProfanityExAsync
BOOL func_ov009_0217bc44(const u16 **words, int count, const char *reserved, int timeout, char *result,
                         int *badWordCount, int region) {
    (void)words, (void)count, (void)reserved, (void)timeout, (void)result, (void)region;
    if (badWordCount != NULL) {
        *badWordCount = 0;
    }
    StubDwc_NoConnection();
    return FALSE;
}

int DWC_CheckProfanityProcess(void) {
    return STUB_PROF_STATE_FAILED;
}

// ---- the WFC settings (dwcutil): the player leaves them at once ---------------------------------------------------

int DWC_StartUtility(void *work, int language, int param) {
    (void)work, (void)language, (void)param;
    return 0;
}

// ---- downloads (dwc_nd.c) ----------------------------------------------------------------------------------------

// DWC_NdInitAsync
BOOL func_ov114_021c5928(DWCNdCallback callback, const char *gameCode, const char *password) {
    (void)callback, (void)gameCode, (void)password;
    StubDwc_NoConnection();
    return FALSE;
}

// DWC_NdProcess
void func_ov114_021c5ab8(void) {
}

// DWC_NdCleanupAsync: nothing to clean up
BOOL func_ov114_021c5b80(void) {
    return FALSE;
}

// DWC_NdSetAttr
BOOL func_ov114_021c5c04(const char *attr1, const char *attr2, const char *attr3) {
    (void)attr1, (void)attr2, (void)attr3;
    return FALSE;
}

// DWC_NdGetFileListAsync
BOOL func_ov114_021c5c38(DWCNdFileInfo *files, u32 offset, u32 count) {
    (void)files, (void)offset, (void)count;
    StubDwc_NoConnection();
    return FALSE;
}

// DWC_NdGetFileAsync
BOOL func_ov114_021c5cfc(DWCNdFileInfo *file, void *buffer, u32 size) {
    (void)file, (void)buffer, (void)size;
    StubDwc_NoConnection();
    return FALSE;
}

// DWC_NdCancelAsync
BOOL func_ov114_021c5db4(void) {
    return FALSE;
}

// DWC_NdGetProgress
BOOL func_ov114_021c5e14(u32 *received, u32 *contentLength) {
    if (received != NULL) {
        *received = 0;
    }
    if (contentLength != NULL) {
        *contentLength = 0;
    }
    return FALSE;
}

// ---- HTTP (d_nhttp.c): it starts, but no connection can be made --------------------------------------------------

// The CA certificate (dwc_ca_nintendo_class2.c) nhttp_rap.c hands to the connection
u8 data_ov009_021a2f2c[4];

// NHTTP_Startup: 0 is success
int func_ov114_021be648(NHttpAllocFunc alloc, NHttpFreeFunc free, u32 priority) {
    (void)alloc, (void)free, (void)priority;
    return 0;
}

// NHTTP_Cleanup
void func_ov114_021be698(void) {
}

// NHTTP_GetError
int func_ov114_021be6ac(void) {
    return 0;
}

// NHTTP_AddHeaderField and kin: 0 is success, and there is no connection to add to
int func_ov114_021be6bc(int connection, const char *label, const char *value) {
    (void)connection, (void)label, (void)value;
    return -1;
}

int func_ov114_021be79c(int connection, const void *data, u32 size) {
    (void)connection, (void)data, (void)size;
    return -1;
}

int func_ov114_021be838(int connection, char **header) {
    (void)connection;
    if (header != NULL) {
        *header = NULL;
    }
    return -1;
}

int func_ov114_021be880(int connection) {
    (void)connection;
    return -1;
}

int func_ov114_021be8e8(int connection, const char *user, const char *password) {
    (void)connection, (void)user, (void)password;
    return -1;
}

int func_ov114_021be9c8(int connection, void *certificates, int count) {
    (void)connection, (void)certificates, (void)count;
    return -1;
}

// NHTTP_CreateConnection: 0 is "none" (the callers' failure path)
int func_ov114_021bf04c(const char *url, int method, void *buffer, u32 bufferSize, NHttpCallback callback, void *arg) {
    (void)url, (void)method, (void)buffer, (void)bufferSize, (void)callback, (void)arg;
    return 0;
}

int func_ov114_021bf184(int handle) {
    (void)handle;
    return -1;
}

int func_ov114_021bf308(int handle) {
    (void)handle;
    return -1;
}

int func_ov114_021bf488(int handle) {
    (void)handle;
    return -1;
}

int func_ov114_021bf4b0(int handle, int *received, int *size) {
    (void)handle;
    if (received != NULL) {
        *received = 0;
    }
    if (size != NULL) {
        *size = 0;
    }
    return -1;
}

// ---- the server's signature check (crypt.c) ----------------------------------------------------------------------

// Crypt_VerifySignature: nothing downloaded is ever verified
int func_ov114_021caa70(const void *data, u32 size, void *signature, const u8 *key) {
    (void)data, (void)size, (void)signature, (void)key;
    return 0;
}

// Crypt_SetAllocator
void func_ov114_021cab1c(CryptAllocFunc alloc, CryptFreeFunc free, u32 arg) {
    (void)alloc, (void)free, (void)arg;
}

// ---- GameSpy's competition (dwc_sc.c) and storage (gdb.c): the Wi-Fi battles' reports ---------------------------

int func_ov175_021dee3c(int gameId, int server) {
    (void)gameId, (void)server;
    StubDwc_NoConnection();
    return STUB_SC_ERROR;
}

int func_ov175_021def58(void) {
    return 0;
}

int func_ov175_021defdc(void) {
    return STUB_SC_ERROR;
}

int func_ov175_021df018(ScResultFunc func, int timeout, void *userData) {
    (void)func, (void)timeout, (void)userData;
    return STUB_SC_ERROR;
}

int func_ov175_021df09c(int authoritative, ScResultFunc func, int timeout, void *userData) {
    (void)authoritative, (void)func, (void)timeout, (void)userData;
    return STUB_SC_ERROR;
}

int func_ov175_021df130(ScReport *report, int authoritative, ScResultFunc func, int timeout, void *userData) {
    (void)report, (void)authoritative, (void)func, (void)timeout, (void)userData;
    return STUB_SC_ERROR;
}

int func_ov175_021df1c8(const char *csid) {
    (void)csid;
    return STUB_SC_ERROR;
}

static char sStubEmptyId[1];

char *func_ov175_021df20c(void) {
    return sStubEmptyId;
}

char *func_ov175_021df224(void) {
    return sStubEmptyId;
}

int func_ov175_021df23c(int a0, int a1, int a2, ScReport **connection) {
    (void)a0, (void)a1, (void)a2;
    if (connection != NULL) {
        *connection = NULL;
    }
    return STUB_SC_ERROR;
}

int func_ov175_021df298(ScReport *connection) {
    (void)connection;
    return STUB_SC_ERROR;
}

int func_ov175_021df2c8(ScReport *connection) {
    (void)connection;
    return STUB_SC_ERROR;
}

int func_ov175_021df2f8(ScReport *connection) {
    (void)connection;
    return STUB_SC_ERROR;
}

int func_ov175_021df328(ScReport *connection) {
    (void)connection;
    return STUB_SC_ERROR;
}

int func_ov175_021df358(ScReport *connection, int index, u8 *unk30, int a3, int mode,
                        const AuthCertificate *certificate, u8 *unk58) {
    (void)connection, (void)index, (void)unk30, (void)a3, (void)mode, (void)certificate, (void)unk58;
    return STUB_SC_ERROR;
}

int func_ov175_021df3b0(ScReport *connection, int a1, int a2) {
    (void)connection, (void)a1, (void)a2;
    return STUB_SC_ERROR;
}

int func_ov175_021df3e8(ScReport *report, u8 key, int value) {
    (void)report, (void)key, (void)value;
    return STUB_SC_ERROR;
}

int func_ov175_021df420(ScReport *report, u8 key, u8 value) {
    (void)report, (void)key, (void)value;
    return STUB_SC_ERROR;
}

int func_ov175_021df458(ScReport *report) {
    (void)report;
    return 0;
}

int func_ov175_021df490(const AuthCertificate **certificate) {
    if (certificate != NULL) {
        *certificate = NULL;
    }
    return STUB_SC_ERROR;
}

s32 func_ov175_021e10cc(s32 gameId, DWCUserData *userData, s32 server) {
    (void)gameId, (void)userData, (void)server;
    StubDwc_NoConnection();
    return STUB_SC_ERROR;
}

// Gdb_Shutdown and kin: 0, the library is off
u32 func_ov175_021e11f0(void) {
    return 0;
}

u32 func_ov175_021e1340(void) {
    return 0;
}

void func_ov175_021e1414(void) {
}

// Gdb_GetState
u32 func_ov175_021e1434(void) {
    return GDB_STATE_OFF;
}

u32 func_ov175_021e1454(void) {
    return 0;
}

u32 func_ov175_021e1464(const char *table, GdbField *fields, int fieldNum, s32 *recordId) {
    (void)table, (void)fields, (void)fieldNum, (void)recordId;
    return STUB_SC_ERROR;
}

u32 func_ov175_021e1540(const char *table, s32 recordId, GdbField *fields, int fieldNum) {
    (void)table, (void)recordId, (void)fields, (void)fieldNum;
    return STUB_SC_ERROR;
}

u32 func_ov175_021e1618(const char *table, const char **fieldNames, int fieldNum, GdbRecordsFunc func, void *work) {
    (void)table, (void)fieldNames, (void)fieldNum, (void)func, (void)work;
    return STUB_SC_ERROR;
}

u32 func_ov175_021e16f0(const char *table, s32 *recordIds, int recordNum, const char **fieldNames, int fieldNum,
                        GdbRecordsFunc func, void *work) {
    (void)table, (void)recordIds, (void)recordNum, (void)fieldNames, (void)fieldNum, (void)func, (void)work;
    return STUB_SC_ERROR;
}

// ---- voice chat (vct): no session can start ----------------------------------------------------------------------

BOOL func_ov013_021b0614(VCTSession *session) {
    (void)session;
    return FALSE;
}

void func_ov013_021b074c(VCTSession *session) {
    (void)session;
}

BOOL func_ov013_021b07f8(const void *samples, int size) {
    (void)samples, (void)size;
    return FALSE;
}

BOOL func_ov013_021b0b48(void *dst, int size, u32 *heardMask) {
    (void)dst, (void)size;
    if (heardMask != NULL) {
        *heardMask = 0;
    }
    return FALSE;
}

BOOL func_ov013_021b0c64(int codec) {
    (void)codec;
    return FALSE;
}

void func_ov013_021b0cd8(BOOL enabled) {
    (void)enabled;
}

void func_ov013_021b0d08(BOOL enabled) {
    (void)enabled;
}

VCTSession *func_ov013_021b1f2c(u8 aid) {
    (void)aid;
    return NULL;
}

BOOL func_ov013_021b1f54(VCTSession *session) {
    (void)session;
    return FALSE;
}

int func_ov013_021b20b0(VCTSession *session, int command) {
    (void)session, (void)command;
    return -1;
}

int func_ov013_021b2180(VCTSession *session, int command) {
    (void)session, (void)command;
    return -1;
}

int func_ov013_021b21a4(u8 aid) {
    (void)aid;
    return -1;
}

int func_ov013_021b2254(u8 aid) {
    (void)aid;
    return -1;
}

void func_ov013_021b3064(s32 *out) {
    if (out != NULL) {
        *out = 0;
    }
}

BOOL func_ov013_021b370c(VCTConfig *config) {
    (void)config;
    return FALSE;
}

void func_ov013_021b3844(void) {
}

void func_ov013_021b386c(void) {
}

BOOL func_ov013_021b38b0(u8 aid, void *data, int size) {
    (void)aid, (void)data, (void)size;
    return FALSE;
}

// ---- NitroWiFi's sockets (lib/poke_net's Global Link calls): no network ------------------------------------------

#define STUB_SOC_ERROR -1

int func_ov009_0215f428(int domain, int type, int protocol) {
    (void)domain, (void)type, (void)protocol;
    return STUB_SOC_ERROR;
}

int func_ov009_0215f480(int sock, const SOCSockAddrIn *address) {
    (void)sock, (void)address;
    return STUB_SOC_ERROR;
}

int func_ov009_0215f4dc(int sock, void *buffer, int size, int flags) {
    (void)sock, (void)buffer, (void)size, (void)flags;
    return STUB_SOC_ERROR;
}

int func_ov009_0215f590(int sock, const void *buffer, int size, int flags) {
    (void)sock, (void)buffer, (void)size, (void)flags;
    return STUB_SOC_ERROR;
}

int func_ov009_0215f638(int sock, int how) {
    (void)sock, (void)how;
    return STUB_SOC_ERROR;
}

int func_ov009_0215f644(int sock) {
    (void)sock;
    return STUB_SOC_ERROR;
}

SOCHostEnt *func_ov009_0215f650(const char *host) {
    (void)host;
    return NULL;
}

char *func_ov009_0215fa9c(SOCInAddr addr) {
    static char none[] = "0.0.0.0";

    (void)addr;
    return none;
}

int func_ov009_0215fc80(int sock, void *sslInfo) {
    (void)sock, (void)sslInfo;
    return STUB_SOC_ERROR;
}

// CPS_Resolve: no address
CPSInAddr func_ov009_0215c910(const char *name) {
    (void)name;
    return 0;
}

// The SSL's entropy pool: nothing to stir
void func_ov009_02162248(const void *seed, u32 len) {
    (void)seed, (void)len;
}

// ---- the game's voice chat over VCT (src/ov013/dwc_rap_vchat.c, which port/exclude.txt leaves out): no voice chat
// ever starts ----

static void (*sStubVChatEndCallback)(void);

// DWCRapVChat_Start
void func_ov013_021aef6c(HeapID heapId, int codec, int count) {
    (void)heapId, (void)codec, (void)count;
}

// DWCRapVChat_Update
void func_ov013_021aee14(BOOL stalled) {
    (void)stalled;
}

// DWCRapVChat_ReceivePacket: no packet is the voice chat's (dwc_rap.c handles it itself)
BOOL func_ov013_021aef44(int aid, u8 *data, int size) {
    (void)aid, (void)data, (void)size;
    return FALSE;
}

// DWCRapVChat_RequestEnd: ends at once, calling the end callback as the end of a session does
void func_ov013_021af13c(void) {
    void (*callback)(void) = sStubVChatEndCallback;

    sStubVChatEndCallback = NULL;
    if (callback != NULL) {
        callback();
    }
}

// DWCRapVChat_SetEndCallback
void func_ov013_021af18c(void (*callback)(void)) {
    sStubVChatEndCallback = callback;
}

// DWCRapVChat_End
void func_ov013_021af1a0(void) {
    sStubVChatEndCallback = NULL;
}

// DWCRapVChat_GetSpeaking
BOOL func_ov013_021af210(void) {
    return FALSE;
}

// DWCRapVChat_AddHeard, DWCRapVChat_RemoveHeard
BOOL func_ov013_021af238(u32 netIds, int myNetId) {
    (void)netIds, (void)myNetId;
    return FALSE;
}

BOOL func_ov013_021af2ac(int myNetId) {
    (void)myNetId;
    return FALSE;
}

// DWCRapVChat_SetMuted
void func_ov013_021af30c(BOOL muted) {
    (void)muted;
}

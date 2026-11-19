// port/include_plat/dwc_gen4.h: the DWC 2.2 names that pret pokeplatinum uses and the port's lib/dwc (Black's DWC, with
// Black's own names for the same functions) lacks: declarations only, since the online side is stubbed on the PC. The
// names and signatures are from how pret's code calls them; a constant's number is Black's where Black's code or lib/dwc
// uses the same thing, else in the library's order, and Platinum's code only compares them with each other.
#ifndef BWPORT_PLAT_DWC_GEN4_H
#define BWPORT_PLAT_DWC_GEN4_H

#include <dwc/dwc_ghttp.h>
#include <dwc/connectinet.h>
#include <dwc/apinfo.h>
#include <dwc/memfunc.h>
#include <dwc/nd.h>

// ---- the Wi-Fi connection ----

// DWC_SetAuthServer's argument; Platinum only passes RELEASE
#define DWC_CONNECTINET_AUTH_TEST 0
#define DWC_CONNECTINET_AUTH_RELEASE 1

void DWC_InitInetEx(DWCInetControl *inetControl, u16 dmaNo, u16 powerMode, u16 sslPriority);
void DWC_SetAuthServer(int authServer);
void DWC_ConnectInetAsync(void);
void DWC_ProcessInet(void);
BOOL DWC_CheckInet(void);
int DWC_GetInetStatus(void);
void DWC_CleanupInet(void);
BOOL DWC_CleanupInetAsync(void);
BOOL DWC_GetApInfo(DWCApInfo *apInfo);
int DWC_GetLinkLevel(void);
// What kind of access point DWCApInfo.aptype tells (Black's apinfo.h: 4 and 8 are the hotspots that have a spot info)
#define DWC_APINFO_TYPE_SHOP 4
// Platinum spells Black's DWCApInfo.apType in lower case
#define aptype apType

// DWC_UpdateConnection is the per-frame step that the games' loops call, and that Mystery Gift keeps as a callback
BOOL DWC_UpdateConnection(void);

// Alloc and free, which DWC takes from the game (Black's DWCi_SetMemFunc, with Platinum's free that gets a u32 size)
void DWC_SetMemFunc(void *(*alloc)(DWCAllocType name, u32 size, int align), void (*free)(DWCAllocType name, void *ptr, u32 size));
// One more than Black's last (DWC_ALLOCTYPE_ND)
#define DWC_ALLOCTYPE_ENC (DWC_ALLOCTYPE_ND + 1)

// ---- starting DWC and the log-in to the NAS ----

// The work area that DWC_Init takes (the game allocates it, and nothing on the PC reads it, so the size is a placeholder)
#define DWC_INIT_WORK_SIZE 0x1000
// What DWC_Init tells about the settings it found in the flash
enum {
    DWC_INIT_RESULT_NOERROR,
    DWC_INIT_RESULT_CREATE_USERDATA,
    DWC_INIT_RESULT_DESTROY_USERDATA,
    DWC_INIT_RESULT_DESTROY_OTHER_SETTING,
};
int DWC_Init(void *work);

// DWC_NASLoginProcess's result: Black's calls (WorldTrade's func_ov009_02168124, which is it) take 3 for done, and 0, 4
// and 5 for the three ways it can end badly, so these are placed there; 1 and 2 are still logging in
typedef enum DWCNasLoginState {
    DWC_NASLOGIN_STATE_ERROR = 0,
    DWC_NASLOGIN_STATE_CONNECTING = 1,
    DWC_NASLOGIN_STATE_SUCCESS = 3,
    DWC_NASLOGIN_STATE_CANCELED = 4,
    DWC_NASLOGIN_STATE_DIRTY = 5,
} DWCNasLoginState;
DWCNasLoginState DWC_NASLoginProcess(void);
void DWC_NASLoginAbort(void);

// DWC_StartUtility's last argument: Platinum passes this for every region, so any value does (Black's utility.h has
// FOR_USA, which is 0)
#define DWC_UTILITY_TOP_MENU_COMMON 0

// ---- the HTTP client ----

// A GHTTP request's error, below 0, in place of its number (Black's ghttp.h GHTTPRequestError)
#define DWC_GHTTP_IN_ERROR GHTTPFailedToOpenFile
#define DWC_GHTTP_INVALID_POST GHTTPInvalidPost
#define DWC_GHTTP_INSUFFICIENT_MEMORY GHTTPInsufficientMemory
#define DWC_GHTTP_INVALID_FILE_NAME GHTTPInvalidFileName
#define DWC_GHTTP_INVALID_BUFFER_SIZE GHTTPInvalidBufferSize
#define DWC_GHTTP_INVALID_URL GHTTPInvalidURL
#define DWC_GHTTP_UNSPECIFIED_ERROR GHTTPUnspecifiedError
// What a request ends with (Black's dwc_ghttp.h DWCGHTTPResult)
#define DWC_GHTTP_SUCCESS DWCGHTTPSuccess
#define DWC_GHTTP_OUT_OF_MEMORY DWCGHTTPOutOfMemory
#define DWC_GHTTP_BUFFER_OVERFLOW DWCGHTTPBufferOverflow
#define DWC_GHTTP_PARSE_URL_FAILED DWCGHTTPParseURLFailed
#define DWC_GHTTP_HOST_LOOKUP_FAILED DWCGHTTPHostLookupFailed
#define DWC_GHTTP_SOCKET_FAILED DWCGHTTPSocketFailed
#define DWC_GHTTP_CONNECT_FAILED DWCGHTTPConnectFailed
#define DWC_GHTTP_BAD_RESPONSE DWCGHTTPBadResponse
#define DWC_GHTTP_REQUEST_REJECTED DWCGHTTPRequestRejected
#define DWC_GHTTP_UNAUTHORIZED DWCGHTTPUnauthorized
#define DWC_GHTTP_FORBIDDEN DWCGHTTPForbidden
#define DWC_GHTTP_FILE_NOT_FOUND DWCGHTTPFileNotFound
#define DWC_GHTTP_SERVER_ERROR DWCGHTTPServerError
#define DWC_GHTTP_FILE_INCOMPLETE DWCGHTTPFileIncomplete
#define DWC_GHTTP_FILE_TOO_BIG DWCGHTTPFileToBig
#define DWC_GHTTP_MEMORY_ERROR DWCGHTTPMemoryError

// A request for a URL, with the game's callback for its end; its number, or an error below 0
int DWC_GetGHTTPData(const char *url, DWCGHTTPCompletedCallback completed, void *param);

// ---- the server list (SVL) ----

#define DWC_SVL_TOKEN_LENGTH 300
typedef struct DWCSvlResult {
    char svltoken[DWC_SVL_TOKEN_LENGTH + 1];
} DWCSvlResult;
typedef int DWCSvlState;
// (DWC_SVLGetTokenAsync and DWC_SVLProcess are Black's dwc.h's, with a void * for the result)

// ---- the download server (ND) ----

// ND's error: the callback of DWC_NdInitAsync gets it where Black's DWCNdCallback has a u32
typedef u32 DWCNdError;
BOOL DWC_NdInitAsync(DWCNdCallback callback, const char *gameCode, const char *password);
BOOL DWC_NdSetAttr(const char *attr1, const char *attr2, const char *attr3);
BOOL DWC_NdGetFileListNumAsync(int *count);
BOOL DWC_NdGetFileListAsync(DWCNdFileInfo *files, u32 offset, u32 count);
BOOL DWC_NdGetFileAsync(DWCNdFileInfo *file, void *buffer, u32 size);
BOOL DWC_NdCancelAsync(void);
BOOL DWC_NdCleanupAsync(void (*callback)(void));
BOOL DWC_NdGetProgress(u32 *received, u32 *contentLength);

// ---- the user data and the friends ----

BOOL DWC_CheckUserData(const DWCUserData *userData);
void DWC_CreateUserData(DWCUserData *userData, u32 gameCode);
void DWC_ClearDirtyFlag(DWCUserData *userData);
BOOL DWC_CheckDirtyFlag(const DWCUserData *userData);
s32 DWC_GetGsProfileId(const DWCUserData *userData, const DWCFriendData *friendData);
u64 DWC_CreateFriendKey(const DWCUserData *userData);
u64 DWC_GetFriendKey(const DWCFriendData *friendData);
BOOL DWC_CheckFriendKey(const DWCUserData *userData, u64 friendKey);
BOOL DWC_IsValidFriendData(const DWCFriendData *friendData);
int DWC_GetFriendDataType(const DWCFriendData *friendData);

// What DWC_GetFriendStatusData tells of a friend (MATCH_SC_SV is Black's 6, which dwc_rap.c tells apart)
enum {
    DWC_STATUS_OFFLINE,
    DWC_STATUS_ONLINE,
    DWC_STATUS_PLAYING,
    DWC_STATUS_MATCH_ANYBODY,
    DWC_STATUS_MATCH_FRIEND,
    DWC_STATUS_MATCH_SC_CL,
    DWC_STATUS_MATCH_SC_SV,
};

// ---- matching and the transport ----

#define DWC_MATCH_OPTION_MIN_COMPLETE 0
#define DWC_SET_MATCH_OPT_RESULT_SUCCESS 0
typedef struct DWCMatchOptMinComplete {
    u8 valid;
    u8 minEntry;
    u16 timeout;
} DWCMatchOptMinComplete;
int DWC_SetMatchingOption(int option, const void *value, int size);
int DWC_AddMatchKeyString(u8 index, const char *keyString, const char *value);
BOOL DWC_CancelMatching(void);
BOOL DWC_StopSCMatchingAsync(void (*callback)(void *param), void *param);
BOOL DWC_SetOwnStatusData(const char *statusData, int size);
BOOL DWC_CloseConnectionHardBitmap(const u32 *bitmap);
BOOL DWC_SetRecvTimeoutTime(u8 aid, u32 time);
void DWC_SetUserSendCallback(void (*callback)(int size, u8 aid));
void DWC_SetUserRecvCallback(void (*callback)(u8 aid, u8 *data, int size));
void DWC_SetUserRecvTimeoutCallback(void (*callback)(u8 aid));

// DWC 2.2's friends-match calls, whose arguments differ from Black's DWC (dwc.h renames Black's while its headers are
// read): the control is given by the game, and the match has no type, attempt or work argument
typedef void (*DWCMatchedAnyCallback)(DWCError error, BOOL cancel, void *param);
void DWC_InitFriendsMatch(DWCFriendsMatchControl *friendsMatch, DWCUserData *userData, int productId, const char *gameName,
                          const char *secretKey, int sendBufSize, int recvBufSize, DWCFriendData *friendList,
                          int friendListLen);
BOOL DWC_ConnectToAnybodyAsync(u8 numEntry, const char *addFilter, DWCMatchedAnyCallback matched, void *matchedParam,
                               DWCEvalPlayerCallback eval, void *evalParam);
BOOL DWC_SetupGameServer(u8 numEntry, DWCMatchedSCCallback matched, void *matchedParam, DWCNewClientCallback newClient,
                         void *newClientParam);
BOOL DWC_ConnectToGameServerAsync(int friendIndex, DWCMatchedSCCallback matched, void *matchedParam,
                                  DWCNewClientCallback newClient, void *newClientParam);

// ---- NitroWiFi's WCM, which DWC's umbrella header brings in ----

// WCM_GetPhase's result, in the library's order of phases; Platinum only asks whether the link is up as a DCF one
typedef enum WCMPhase {
    WCM_PHASE_NULL,
    WCM_PHASE_WAIT,
    WCM_PHASE_WAIT_TO_IDLE,
    WCM_PHASE_IDLE,
    WCM_PHASE_IDLE_TO_SEARCH,
    WCM_PHASE_SEARCH,
    WCM_PHASE_SEARCH_TO_IDLE,
    WCM_PHASE_IDLE_TO_DCF,
    WCM_PHASE_DCF,
    WCM_PHASE_DCF_TO_IDLE,
    WCM_PHASE_FATAL_ERROR,
    WCM_PHASE_IRREGULAR,
    WCM_PHASE_TERMINATING,
} WCMPhase;
s32 WCM_GetPhase(void);

// ---- base 64 ----

// GameSpy base 64 (Black's lib/dwc has it): http_b64.c and ppwlobby call B64Encode
#include <dwc/gsPlatformUtil.h>

int DWC_Base64Encode(const char *src, int srcSize, char *dst, int dstSize);
int DWC_Base64Decode(const char *src, int srcSize, char *dst, int dstSize);

#endif // BWPORT_PLAT_DWC_GEN4_H

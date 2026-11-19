// Platinum's online play on the PC: none. Stubs for the NitroDWC 2.2, NitroWiFi, libvct and ppwlobby functions that
// pret's pokeplatinum calls (src/nintendo_wfc, gts_application, wifi_battle_tower, main_menu, overlay061/066/098,
// lib/gds, lib/ppwlobby), compiled into pl.o with Platinum's headers (port/include_plat), so each name takes the
// arguments pret's callers pass and stays Platinum's own: pl.o keeps no global but pl_bwgame (tools/port_relink.py), so
// these definitions are local to it and do not meet Black 2's stubs of the same names (VCT_*, DWC_*), whose arguments
// differ. What they do is Black's (port/src/stub_online.c): the console finds no access point, nothing connects, and
// every call that needs a connection fails the way its callers test for. The offline parts (the save's user data, the
// friend data) stay what the SDK's are: DWC_CheckUserData and the friend functions are stub_online.c's, reached through
// port/src/stub_b2w2.c's forwarders; the ones whose arguments Platinum gives otherwise are here.
#include <dwc.h>
#include <nitroWiFi/soc.h>
#ifdef PLATINUM
#include <ppwlobby/ppw_lobby.h>
#endif
#include <string.h>

// ---- the error store and the library's setup ------------------------------------------------------------------------

// port/src/stub_online.c: the error store (the error and its code) and the friend data's "is a buddy's" test
int DWC_GetLastErrorEx(int *code, int *type);
BOOL func_0205912c(const DWCFriendData *friendData);

// DWC_GetLastError: the error, and its code through code (Platinum's callers read them and go on)
u32 DWC_GetLastError(u32 *code) {
    int errorCode = 0;
    int error = DWC_GetLastErrorEx(&errorCode, NULL);

    if (code != NULL) {
        *code = (u32)errorCode;
    }
    return (u32)error;
}

// What the game gave DWC to allocate with: nothing on the PC reads it
static void *(*sAlloc)(DWCAllocType name, u32 size, int align);
static void (*sFree)(DWCAllocType name, void *ptr, u32 size);

void DWC_SetMemFunc(void *(*alloc)(DWCAllocType name, u32 size, int align), void (*free)(DWCAllocType name, void *ptr, u32 size)) {
    sAlloc = alloc;
    sFree = free;
}

// DWC_Init: the settings in the flash are fine (a PC has none to find damaged)
int DWC_Init(void *work) {
    (void)work;
    return DWC_INIT_RESULT_NOERROR;
}

void DWC_SetAuthServer(int authServer) {
    (void)authServer;
}

// ---- the save's user data (dwc_account.c's layout, Black's stub_online.c) ---------------------------------------------

// The SDK's CRC32 of the user data (MATH_CalcCRC32 with the table of 0xedb88320), over everything before the CRC
static u32 Crc32(const void *data, u32 size) {
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

// DWC_CreateUserData: a user data for the game code it is given (Black's takes the code from DWC_Init, so the forwarder
// stub_b2w2.c makes would give Platinum's saves Black's code, and the friend keys that hold it with them), with no login ID
// and no profile yet (the DS takes its login ID from the console's Wi-Fi ID), marked dirty
void DWC_CreateUserData(DWCUserData *userData, u32 gameCode) {
    memset(userData, 0, sizeof(DWCUserData));
    userData->unk_0 = sizeof(DWCUserData);
    userData->unk_24 = gameCode;
    userData->flag |= 1;
    userData->crc32 = Crc32(userData, 0x3c);
}

// The friend data's type (the low two bits of its flags: 0 none, 1 login ID, 2 friend key, 3 GameSpy profile ID)
int DWC_GetFriendDataType(const DWCFriendData *friendData) {
    return (friendData->data[0] >> 11) & 3;
}

BOOL DWC_IsBuddyFriendData(const DWCFriendData *friendData) {
    return func_0205912c(friendData);
}

// ---- the link to the network ----------------------------------------------------------------------------------------

BOOL DWC_GetApInfo(DWCApInfo *apInfo) {
    (void)apInfo;
    return FALSE;   // no access point was seen
}

BOOL DWC_CleanupInetAsync(void) {
    return TRUE;   // nothing is connected: the clean-up is done at once
}

void DWC_NASLoginAbort(void) {
}

s32 WCM_GetPhase(void) {
    return WCM_PHASE_NULL;   // the radio is off: Platinum's checks for the DCF link fail
}

// ---- downloads, HTTP, matching -----------------------------------------------------------------------------------------

BOOL DWC_NdGetFileListNumAsync(int *count) {
    (void)count;
    return FALSE;
}

BOOL DWC_InitGHTTP(const char *gameCode) {
    (void)gameCode;
    return FALSE;   // HTTP_Init takes this as an error
}

BOOL DWC_ProcessGHTTP(void) {
    return FALSE;
}

BOOL DWC_ShutdownGHTTP(void) {
    return TRUE;
}

int DWC_GetGHTTPData(const char *url, DWCGHTTPCompletedCallback completed, void *param) {
    (void)url, (void)completed, (void)param;
    return DWC_GHTTP_UNSPECIFIED_ERROR;   // the request does not start (below 0)
}

int DWC_SetMatchingOption(int option, const void *value, int size) {
    (void)option, (void)value, (void)size;
    return DWC_SET_MATCH_OPT_RESULT_SUCCESS;
}

BOOL DWC_CancelMatching(void) {
    return FALSE;
}

BOOL DWC_StopSCMatchingAsync(void (*callback)(void *param), void *param) {
    (void)callback, (void)param;
    return FALSE;
}

BOOL DWC_CloseConnectionHardBitmap(const u32 *bitmap) {
    (void)bitmap;
    return FALSE;
}

// GameSpy's base 64 (gsPlatformUtil.c): input of length bytes to a NUL-ended string, the standard alphabet; type 1 spells
// "+/=" as "[]_" (GameSpy's password and chat encodings). No server answers, so only the shape has to be right
void B64Encode(const char *input, char *output, int length, int encodingType) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const u8 *in = (const u8 *)input;
    char c62 = encodingType == 1 ? '[' : '+';
    char c63 = encodingType == 1 ? ']' : '/';
    char pad = encodingType == 1 ? '_' : '=';
    int i;

    for (i = 0; i < length; i += 3) {
        u32 v = (u32)in[i] << 16;
        int left = length - i;
        int k;
        char sym[4];

        if (left > 1) {
            v |= (u32)in[i + 1] << 8;
        }
        if (left > 2) {
            v |= in[i + 2];
        }
        for (k = 0; k < 4; k++) {
            sym[k] = alphabet[(v >> (18 - 6 * k)) & 63];
            sym[k] = sym[k] == '+' ? c62 : (sym[k] == '/' ? c63 : sym[k]);
        }
        if (left < 3) {
            sym[3] = pad;
        }
        if (left < 2) {
            sym[2] = pad;
        }
        memcpy(output, sym, 4);
        output += 4;
    }
    *output = '\0';
}

// ---- sockets (lib/gds's calls): no network -------------------------------------------------------------------------------

int SOC_Socket(int domain, int type, int protocol) {
    (void)domain, (void)type, (void)protocol;
    return -1;
}

int SOC_Connect(int sock, const SOCSockAddrIn *address) {
    (void)sock, (void)address;
    return -1;
}

int SOC_Send(int sock, const void *buffer, int size, int flags) {
    (void)sock, (void)buffer, (void)size, (void)flags;
    return -1;
}

int SOC_Recv(int sock, void *buffer, int size, int flags) {
    (void)sock, (void)buffer, (void)size, (void)flags;
    return -1;
}

int SOC_Shutdown(int sock, int how) {
    (void)sock, (void)how;
    return -1;
}

int SOC_Close(int sock) {
    (void)sock;
    return -1;
}

int SOC_Fcntl(int sock, int command, int flags) {
    (void)sock, (void)command, (void)flags;
    return -1;
}

SOCHostEnt *SOC_GetHostByName(const char *host) {
    (void)host;
    return NULL;
}

char *SOC_InetNtoA(SOCInAddr addr) {
    static char none[] = "0.0.0.0";

    (void)addr;
    return none;
}

// ---- voice chat (libvct): no session can start --------------------------------------------------------------------------

BOOL VCT_Init(VCTConfig *config) {
    (void)config;
    return FALSE;
}

void VCT_Cleanup(void) {
}

void VCT_Main(void) {
}

VCTSession *VCT_CreateSession(u8 aid) {
    (void)aid;
    return NULL;
}

BOOL VCT_DeleteSession(VCTSession *session) {
    (void)session;
    return FALSE;
}

int VCT_Request(VCTSession *session, int request) {
    (void)session, (void)request;
    return -1;
}

int VCT_Response(VCTSession *session, int response) {
    (void)session, (void)response;
    return -1;
}

int VCT_AddConferenceClient(u8 aid) {
    (void)aid;
    return -1;
}

BOOL VCT_StartStreaming(VCTSession *session) {
    (void)session;
    return FALSE;
}

void VCT_StopStreaming(VCTSession *session) {
    (void)session;
}

BOOL VCT_SendAudio(const void *samples, int size) {
    (void)samples, (void)size;
    return FALSE;
}

BOOL VCT_ReceiveAudio(void *dst, int size, u32 *heardMask) {
    (void)dst, (void)size;
    if (heardMask != NULL) {
        *heardMask = 0;
    }
    return FALSE;
}

BOOL VCT_SetCodec(int codec) {
    (void)codec;
    return FALSE;
}

void VCT_EnableVAD(BOOL enable) {
    (void)enable;
}

void VCT_EnableEchoCancel(BOOL enable) {
    (void)enable;
}

void VCT_GetVADInfo(VCTVADInfo *info) {
    if (info != NULL) {
        info->scale = 0;
        info->activity = 0;
    }
}

BOOL VCT_HandleData(u8 aid, void *data, int size) {
    (void)aid, (void)data, (void)size;
    return FALSE;
}

#ifdef PLATINUM   // (HeartGold, port/game/hg_online.c, has no Wi-Fi Plaza)
// ---- the Wi-Fi Plaza's lobby (lib/ppwlobby): the connection never gets made, so the lobby stays what it starts as ----------

PPW_LOBBY_RESULT PPW_LobbyInitializeAsync(const char *gameName, const char *secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix, const PPW_LobbyCallbacks *lobbyCallbacks, const DWCUserData *dwcUserdata, const u8 *userProfile, u32 userProfileSize) {
    (void)gameName, (void)secretKey, (void)prefix, (void)lobbyCallbacks, (void)dwcUserdata, (void)userProfile, (void)userProfileSize;
    return PPW_LOBBY_RESULT_ERROR_SESSION;
}

PPW_LOBBY_RESULT PPW_LobbyShutdownAsync() {
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_STATE PPW_LobbyProcess() {
    return PPW_LOBBY_STATE_NOTINITIALIZED;
}

PPW_LOBBY_ERROR PPW_LobbyGetLastError() {
    return PPW_LOBBY_ERROR_SESSION;
}

// The error code the game shows for a lobby error (the library's code base and the error's number)
s32 PPW_LobbyToErrorCode(PPW_LOBBY_ERROR err) {
    return PPW_LOBBY_ERROR_CODE_BASE + (s32)err;
}

// The lobby error a result of a call stands for
PPW_LOBBY_ERROR ov66_02237134(PPW_LOBBY_RESULT result) {
    switch (result) {
    case PPW_LOBBY_RESULT_SUCCESS:
        return PPW_LOBBY_ERROR_NONE;
    case PPW_LOBBY_RESULT_ERROR_ALLOC:
        return PPW_LOBBY_ERROR_ALLOC;
    case PPW_LOBBY_RESULT_ERROR_SESSION:
        return PPW_LOBBY_ERROR_SESSION;
    default:
        return PPW_LOBBY_ERROR_UNKNOWN;
    }
}

PPW_LOBBY_RESULT PPW_LobbyJoinSubChannelAsync(PPW_LOBBY_CHANNEL_KIND channelKind) {
    (void)channelKind;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbyLeaveSubChannel() {
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbyGetTimeInfo(PPW_LobbyTimeInfo *timeInfo) {
    (void)timeInfo;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbyGetSchedule(PPW_LobbySchedule *schedule, u32 *scheduleSize) {
    (void)schedule, (void)scheduleSize;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_CHANNEL_STATE PPW_LobbyGetSubChannelState() {
    return PPW_LOBBY_CHANNEL_STATE_NONE;
}

PPW_LOBBY_RESULT PPW_LobbySetChannelData(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char *key, const u8 *data, u32 dataSize) {
    (void)userId, (void)channelKind, (void)key, (void)data, (void)dataSize;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbyGetChannelDataAsync(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char *key, void *param) {
    (void)userId, (void)channelKind, (void)key, (void)param;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbySendChannelBinaryMessage(PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const u8 *data, u32 dataSize) {
    (void)channelKind, (void)type, (void)data, (void)dataSize;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbySendPlayerBinaryMessage(s32 userId, s32 type, const u8 *data, u32 dataSize) {
    (void)userId, (void)type, (void)data, (void)dataSize;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

s32 PPW_LobbyGetMyUserId() {
    return PPW_LOBBY_INVALID_USER_ID;
}

PPW_LOBBY_RESULT PPW_LobbyUpdateMyProfile(const u8 *userProfile, u32 userProfileSize, BOOL force) {
    (void)userProfile, (void)userProfileSize, (void)force;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbyStartRecruit(PPW_LobbyRecruitInfo *recruitInfo) {
    (void)recruitInfo;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbyUpdateRecruitInfo(const PPW_LobbyRecruitInfo *recruitInfo) {
    (void)recruitInfo;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbyStopRecruit() {
    return PPW_LOBBY_RESULT_ERROR_STATE;
}

PPW_LOBBY_RESULT PPW_LobbySubmitQuestionnaire(s32 answerNo) {
    (void)answerNo;
    return PPW_LOBBY_RESULT_ERROR_STATE;
}
#endif

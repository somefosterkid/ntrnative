#include "global.h"

#include "overlay_00_thumb.h"
#include "unk_02037C94.h"

enum NintendoWFCResult {
    NINTENDO_WFC_RESULT_MATCHMAKING_SUCCESS = DWC_ERROR_NUM,
    NINTENDO_WFC_RESULT_CONN_RESET,
    NINTENDO_WFC_RESULT_CONN_RESET_AFTER_HOST_LEFT,
    NINTENDO_WFC_RESULT_RECEIVE_TIMED_OUT,
    NINTENDO_WFC_RESULT_CONNECTION_CLOSED,
    NINTENDO_WFC_RESULT_UNUSED_5,
    NINTENDO_WFC_RESULT_DUMMY_6,
    NINTENDO_WFC_RESULT_CONNECTED_TO_SERVER,
};

typedef void (*NintendoWFCDataTransferCallback)(u16 aid, u16 *data, u16 length);

typedef void (*NintendoWFCConnectionClosedCB)(u16 aid, void *userParam);

typedef BOOL (*NintendoWFCHostMatchCallback)(int hostFriendIdx);

typedef void (*NintendoWFCNewClientCallback)(u16 id, void *userParam);

typedef void (*NintendoWFCFatalErrorCallback)(int errorCode);

BOOL sub_02034084(int param0);

enum NintendoWFCState {
    WFC_STATE_INIT = 0,
    WFC_STATE_WAIT_CONNECTION,
    WFC_STATE_INIT_LOGIN,
    WFC_STATE_WAIT_FOR_LOGIN,
    WFC_STATE_READY,
    WFC_STATE_WAIT_MATCHMAKING,
    WFC_STATE_FORCE_CANCEL,
    WFC_STATE_MATCHMAKING_SUCCESS,
    WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS,
    WFC_STATE_RESET_AFTER_DISCONNECT,
    WFC_STATE_HOST_DISCONNECTED,
    WFC_STATE_RESET_AFTER_HOST_LEFT,
    WFC_STATE_CONNECTION_ERROR,
    WFC_STATE_UNUSED_13,
    WFC_STATE_ERROR_DISCONNECTED,
    WFC_STATE_FATAL_ERROR,
    WFC_STATE_CLOSE_CONNECTION,
    WFC_STATE_CONNECTION_CLOSED,
    WFC_STATE_RECEIVE_TIMED_OUT,
};

enum NintendoWFCConnectionRole {
    WFC_CONNECTION_NONE = 0,
    WFC_CONNECTION_HOST,
    WFC_CONNECTION_CLIENT,
};

typedef struct NintendoWFCManager {
    u8 sendBuffer[256];
    DWCFriendData *friends;
    DWCFriendsMatchControl friendsMatchCtl;
    DWCUserData *userData;
    DWCInetControl inetCtl;
    SaveData *saveData;
    void *allocStartPtr;
    void *receiveBuffers[4];
    void *heap1AllocPtr;
    NNSFndHeapHandle heap1;
    void *heap2AllocPtr;
    NNSFndHeapHandle heap2;
    u32 heap2Size;
    NintendoWFCDataTransferCallback dataTransferServerCallback;
    NintendoWFCDataTransferCallback dataTransferClientCallback;
    NintendoWFCConnectionClosedCB connectionClosedCallback;
    void *connectionClosedCBArg;
    NintendoWFCNewClientCallback newClientCallback;
    void *newClientCallbackArg;
    NintendoWFCHostMatchCallback hostMatchCallback;
    NintendoWFCFatalErrorCallback fatalErrorCallback;
    u8 ppwMatchKey[128];
    u8 friendConnStatus[32];
    void *friendStatusesBuffer;
    int friendStatusSize;
    u32 nextFriendIdx;
    enum NintendoWFCState state;
    int connectionRole;
    int dataSent;
    int maxPlayers;
    u32 voiceConnectedIDs;
    enum HeapID primaryHeapID;
    enum HeapID secondaryHeapID;
    int heapSize;
    BOOL voiceChatActive;
    int hostFriendIdx;
    int latestNewClientFriendIdx;
    BOOL voiceChatEnabled;
    BOOL unused_1094;
    int voiceChatCodec;
    int enableTimeout;
    int clientUpdateTimers[8];
    int timer;
    int otherPlayersEnabledVoiceChat;
    u16 inVoiceChatLobby;
    u16 voiceChatEnabled_Battles;
    u32 matchmakingAIDBitmap;
    u8 packetsSent;
    u8 unused_10D1;
    u8 dummy_10D2;
    u8 dummy_10D3;
    u8 matchmakingCancelState;
    u8 receiveBuffersOnGlobalHeap[4];
    u8 disconnectIfAlone;
    u8 needsGameSave;
} NintendoWFCManager;

int ov00_021E6850(void);

extern NintendoWFCManager *_0221A680;

void ov00_021E5E34(void);

int ov00_021E5E54(BOOL cancelConnection) {
    switch (_0221A680->state) {
    case WFC_STATE_WAIT_MATCHMAKING:
        if (cancelConnection) {
            _0221A680->state = WFC_STATE_FORCE_CANCEL;
        }

        if (_0221A680->connectionRole == WFC_CONNECTION_CLIENT
            && _0221A680->hostFriendIdx >= 0
            && _0221A680->friendConnStatus[_0221A680->hostFriendIdx] != DWC_STATUS_MATCH_SC_SV) {
            _0221A680->state = WFC_STATE_HOST_DISCONNECTED;
        }

        break;
    case WFC_STATE_FORCE_CANCEL:
    case WFC_STATE_HOST_DISCONNECTED:
        if (_0221A680->connectionRole == WFC_CONNECTION_NONE) {
            DWC_CancelMatching();
        } else {
            DWC_CloseAllConnectionsHard();
            ov00_021E5E34();
        }
        break;
    case WFC_STATE_MATCHMAKING_SUCCESS:
        if (!sub_02034084(sub_0203993C())) {
            ov00_021E69A8(_0221A680->primaryHeapID);
        }

        if (_0221A680->voiceChatEnabled) {
            _0221A680->inVoiceChatLobby = TRUE;
        } else {
            _0221A680->inVoiceChatLobby = FALSE;
        }

        _0221A680->state = WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS;
        return NINTENDO_WFC_RESULT_MATCHMAKING_SUCCESS;
    case WFC_STATE_RESET_AFTER_DISCONNECT:
        _0221A680->state = WFC_STATE_READY;
        _0221A680->dataSent = FALSE;
        _0221A680->latestNewClientFriendIdx = -1;
        return NINTENDO_WFC_RESULT_CONN_RESET;
    case WFC_STATE_RESET_AFTER_HOST_LEFT:
        _0221A680->state = WFC_STATE_READY;
        _0221A680->dataSent = FALSE;
        _0221A680->latestNewClientFriendIdx = -1;
        return NINTENDO_WFC_RESULT_CONN_RESET_AFTER_HOST_LEFT;
    case WFC_STATE_CONNECTION_ERROR:
        return ov00_021E6690();
    case WFC_STATE_CLOSE_CONNECTION:
        if (_0221A680->voiceChatActive == FALSE) {
            DWC_CloseAllConnectionsHard();
            _0221A680->state = WFC_STATE_CONNECTION_CLOSED;
            break;
        }
    default:
        break;
    }

    return ov00_021E6850();
}

BOOL ov00_021E6790(void);

BOOL ov00_021E5F84(void *data, int size);

BOOL ov00_021E5F84(void *data, int size) {
    if (!(size < 256)) {
        return FALSE;
    }

    if (DWC_GetMyAID() == 0) {
        if (_0221A680->dataTransferServerCallback != NULL) {
            _0221A680->dataTransferServerCallback(0, data, size);
        }

        return TRUE;
    } else {
        if (_0221A680->dataSent || !ov00_021E6790()) {
            return FALSE;
        }

        *((u32 *)_0221A680->sendBuffer) = 0x1 | (_0221A680->inVoiceChatLobby << 8);
        _0221A680->sendBuffer[2] = ++_0221A680->packetsSent;

        MI_CpuCopy8(data, &(_0221A680->sendBuffer[4]), size);
        _0221A680->dataSent = 1;

        DWC_SendReliableBitmap(0x1, _0221A680->sendBuffer, size + 4);

        return TRUE;
    }
}

int ov00_021E602C(void *data, int size);

int ov00_021E602C(void *data, int size) {
    if (!(size < 256)) {
        return FALSE;
    }

    if (!(DWC_GetMyAID() == 0)) {
        return FALSE;
    }

    if (_0221A680->dataSent || !ov00_021E6790()) {
        return FALSE;
    }

    *((u32 *)_0221A680->sendBuffer) = 0x1 | (_0221A680->inVoiceChatLobby << 8);
    _0221A680->sendBuffer[2] = ++_0221A680->packetsSent;
    MI_CpuCopy8(data, &(_0221A680->sendBuffer[4]), size);
    _0221A680->dataSent = TRUE;

    if (!DWC_SendReliableBitmap(DWC_GetAIDBitmap(), _0221A680->sendBuffer, size + 4)) {
        _0221A680->dataSent = FALSE;
        return FALSE;
    }

    if (_0221A680->dataTransferClientCallback != NULL) {
        _0221A680->dataTransferClientCallback(0, data, size);
    }

    return TRUE;
}

BOOL ov00_021E60E8(void *data, int size);

BOOL ov00_021E60E8(void *data, int size) {
    if (!(size < 256)) {
        return FALSE;
    }

    if (_0221A680->dataSent || !ov00_021E6790()) {
        return FALSE;
    }

    *((u32 *)_0221A680->sendBuffer) = 0x1 | (_0221A680->inVoiceChatLobby << 8);
    _0221A680->sendBuffer[2] = ++_0221A680->packetsSent;
    MI_CpuCopy8(data, &(_0221A680->sendBuffer[4]), size);
    _0221A680->dataSent = 1;

    u16 connectedAidBitmap = DWC_GetAIDBitmap();

    if (connectedAidBitmap != DWC_SendReliableBitmap(connectedAidBitmap, _0221A680->sendBuffer, size + 4)) {
        _0221A680->dataSent = 0;
        return FALSE;
    }

    if (_0221A680->dataTransferClientCallback != NULL) {
        _0221A680->dataTransferClientCallback(DWC_GetMyAID(), data, size);
    }

    return TRUE;
}

void ov00_021E6274(DWCError error, BOOL friendListChanged, void *userParam);

void ov00_021E6298(int arg0, u8 arg1, const char *arg2, void *arg3);

void ov00_021E629C(int arg0, int arg1, void *arg2);

void ov00_021E62D4(int arg0, void *arg1);

void ov00_021E61A8(DWCError error, int profileID, void *userParam);

void ov00_021E61A8(DWCError error, int profileID, void *userParam) {
    if (DWC_CheckDirtyFlag(_0221A680->userData)) {
        DWC_ClearDirtyFlag(_0221A680->userData);
        _0221A680->needsGameSave = 1;
    }

    if (error == DWC_ERROR_NONE) {
        BOOL success = DWC_UpdateServersAsync(0L, ov00_021E6274, _0221A680->userData, ov00_021E6298, userParam, ov00_021E629C, userParam);

        if (success == 0) {
            sub_02039AD8(1);
            return;
        }

        DWC_SetBuddyFriendCallback(ov00_021E62D4, 0L);
    } else {
        _0221A680->state = WFC_STATE_CONNECTION_ERROR;
    }
}

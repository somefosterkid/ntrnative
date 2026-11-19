#include "global.h"

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

extern NintendoWFCManager *_0221A680;

int ov00_021E6690(void);

int ov00_021E6690(void) {

    DWCError errorCode;
    DWCErrorType errorType;
    DWCError error;
    int ret = DWC_ERROR_NONE;

    error = DWC_GetLastErrorEx((int *)&errorCode, &errorType);

    if (error != 0) {
        ret = errorCode;

        if (errorCode == 0 || errorType == DWC_ETYPE_LIGHT) {
            ret = error;
        }

        switch (errorType) {
        case DWC_ETYPE_LIGHT:
            DWC_ClearError();
            break;
        case DWC_ETYPE_SHOW_ERROR:
            DWC_ClearError();
            break;
        case DWC_ETYPE_SHUTDOWN_FM:
        case DWC_ETYPE_SHUTDOWN_GHTTP:
        case DWC_ETYPE_SHUTDOWN_ND:
        case DWC_ETYPE_DISCONNECT:
            if (_0221A680) {
                switch (_0221A680->state) {
                case WFC_STATE_WAIT_FOR_LOGIN:
                case WFC_STATE_READY:
                case WFC_STATE_WAIT_MATCHMAKING:
                case WFC_STATE_FORCE_CANCEL:
                case WFC_STATE_MATCHMAKING_SUCCESS:
                case WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS:
                case WFC_STATE_RESET_AFTER_DISCONNECT:
                case WFC_STATE_CONNECTION_ERROR:
                case WFC_STATE_RECEIVE_TIMED_OUT:
                case WFC_STATE_CLOSE_CONNECTION:
                case WFC_STATE_CONNECTION_CLOSED:
                    DWC_ShutdownFriendsMatch();
                case WFC_STATE_INIT:
                case WFC_STATE_WAIT_CONNECTION:
                case WFC_STATE_INIT_LOGIN:
                    if (sub_0203993C() != 33) {
                        DWC_CleanupInet();
                    }
                default:
                    break;
                }

                DWC_ClearError();
            }

            if (_0221A680) {
                _0221A680->state = WFC_STATE_ERROR_DISCONNECTED;
            }
            break;
        case DWC_ETYPE_FATAL:
            if (_0221A680) {
                _0221A680->state = WFC_STATE_FATAL_ERROR;

                if (_0221A680->fatalErrorCallback != 0L) {
                    _0221A680->fatalErrorCallback(-errorCode);
                }
            }
            break;
        }
    }

    if (_0221A680->dummy_10D2) {
        ret = NINTENDO_WFC_RESULT_DUMMY_6;
    }

    if (error != 0) {
        sub_02038F74(errorCode, errorType, error);
    }

    return ret;
}

BOOL ov00_021E6790(void);

BOOL ov00_021E6790(void) {
    int i; // forward declaration required to match
    BOOL ret = FALSE;

    for (i = 0; i < _0221A680->maxPlayers; i++) {
        if (DWC_GetMyAID() == i) {
            continue;
        }

        if (DWC_IsValidAID(i)) {
            ret = TRUE;

            if (!DWC_IsSendableReliable(i)) {
                return FALSE;
            }
        }
    }

    return ret;
}

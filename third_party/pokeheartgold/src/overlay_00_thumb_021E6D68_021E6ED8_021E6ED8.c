#include "global.h"

#include "overlay_00_thumb.h"
#include "unk_02037C94.h"

typedef void (*NintendoWFCDataTransferCallback)(u16 aid, u16 *data, u16 length);

typedef void (*NintendoWFCConnectionClosedCB)(u16 aid, void *userParam);

typedef BOOL (*NintendoWFCHostMatchCallback)(int hostFriendIdx);

typedef void (*NintendoWFCNewClientCallback)(u16 id, void *userParam);

typedef void (*NintendoWFCFatalErrorCallback)(int errorCode);

BOOL sub_02034098(int param0);

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

enum WFCMatchmakingCancelState {
    MATCHMAKING_NOT_CANCELED = 0,
    MATCHMAKING_CANCEL_REQUESTED,
    MATCHMAKING_CANCELED,
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

void ov00_021E6388(int unused);

void ov00_021E6ED8(DWCError error, BOOL cancelled, BOOL isThisConsole, BOOL hostCancelled, int hostFriendIdx, void *userParam);

void ov00_021E6ED8(DWCError error, BOOL cancelled, BOOL isThisConsole, BOOL hostCancelled, int hostFriendIdx, void *userParam) {
    BOOL refuseNewConnections = 0;

    _0221A680->unused_1094 = 0;

    if (error == DWC_ERROR_NONE) {
        if (!cancelled) {
            if (sub_02034098(sub_0203993C()) && (hostFriendIdx == (-1))) {
                refuseNewConnections = 1;
            }

            if (_0221A680->hostMatchCallback) {
                if (0 == _0221A680->hostMatchCallback(hostFriendIdx)) {
                    refuseNewConnections = 1;
                }
            }

            if (_0221A680->matchmakingCancelState != MATCHMAKING_NOT_CANCELED || refuseNewConnections) {
                u32 newConnections = ~_0221A680->matchmakingAIDBitmap & DWC_GetAIDBitmap();
                u32 connections = DWC_GetAIDBitmap();

                if (newConnections) {
                    DWC_CloseConnectionHardBitmap(&newConnections);

                    if ((newConnections ^ connections) == 0x1) {
                        _0221A680->state = WFC_STATE_FORCE_CANCEL;
                    }

                    return;
                }
            }

            _0221A680->hostFriendIdx = hostFriendIdx;
            _0221A680->matchmakingAIDBitmap = DWC_GetAIDBitmap();

            if (_0221A680->matchmakingAIDBitmap == 0x1) {
                _0221A680->state = WFC_STATE_FORCE_CANCEL;
            } else {
                ov00_021E6388(hostFriendIdx);
            }
        } else if (!isThisConsole) {
            _0221A680->latestNewClientFriendIdx = (-1);
        }
    }
}

#include "global.h"

#include "overlay_00_thumb.h"

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

void ov00_021E6554(DWCError error, BOOL selfClosed, BOOL hostClosed, u8 aid, int closingFriendIdx, void *userParam);

void ov00_021E6554(DWCError error, BOOL selfClosed, BOOL hostClosed, u8 aid, int closingFriendIdx, void *userParam) {
    _0221A680->dataSent = 0;
    _0221A680->timer = 0;
    _0221A680->unused_1094 = 0;

    if (error == DWC_ERROR_NONE) {
        if (_0221A680->disconnectIfAlone == TRUE && DWC_GetNumConnectionHost() == 1) {
            if (_0221A680->state != WFC_STATE_FORCE_CANCEL) {
                _0221A680->state = WFC_STATE_CLOSE_CONNECTION;
            }

            if (_0221A680->voiceChatActive) {
                ov00_021E6A4C();
            }
        }
    }

    if (_0221A680->connectionClosedCallback) {
        _0221A680->connectionClosedCallback(aid, _0221A680->connectionClosedCBArg);
    }
}

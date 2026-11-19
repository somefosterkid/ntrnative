#include "global.h"

#include "constants/heap.h"

#include "overlay_00_thumb.h"
#include "save.h"

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
    struct { struct GTI2Socket *gt2Socket; struct { void *connected; void *received; void *closed; void *ping; } gt2Callbacks; int gt2SendBufSize; int gt2RecvBufSize; void *gpObj; struct DWCstAccUserData *userdata; u32 state; u32 lastState; u8 aid; u8 ownCloseFlag; u16 playerName[26]; u8 unk_64_pad[2]; int profileID; s8 *gameName; s8 *secretKey; void *loginCallback; void *loginParam; void *updateServersCallback; void *updateServersParam; void *matchedCallback; void *matchedParam; void *matchedSCCallback; void *matchedSCParam; void *closedCallback; void *closedParam; struct { void **pGpObj; u32 state; int productID; u32 gamecode; u16 *playerName; void *callback; void *param; struct DWCstAccUserData *userdata; void *bmwork; void *http; u64 startTick; u32 connectFlag; u64 connectTick; struct { u32 id_data; u32 userid_lo32; u32 playerid; } tempLoginId; s8 authToken[256]; s8 partnerChallenge[256]; s8 username[21]; u8 unk_260_pad[3]; struct { u32 result; int profile; s8 uniquenick[21]; u8 unk_1D_tail[3]; } gpconnectresponsearg; } logcnt; struct { u32 state; void **pGpObj; u32 gpProcessCount; u64 lastGpProcess; int friendListLen; void *friendList; u8 buddyUpdateIdx; u8 friendListChanged; u8 buddyUpdateState; u8 svUpdateComplete[1]; u32 persCallbackLevel; int profileID; u16 *playerName; void *updateCallback; void *updateParam; void *statusCallback; void *statusParam; void *deleteCallback; void *deleteParam; void *buddyCallback; void *buddyParam; void *persLoginCallback; void *persLoginParam; void *saveCallback; void *loadCallback; } friendcnt; struct { void **pGpObj; void **pGt2Socket; void *gt2Callbacks; u8 gt2ConnectCount; u8 gt2NumConnection; u8 gt2NumValidConn; u8 pad1; struct qr2_implementation_s *qr2Obj; u8 qr2NNFinishCount[1]; u8 qr2MatchType[1]; u8 qr2NumEntry[1]; u8 qr2IsReserved[1]; u8 qr2ShutdownFlag; u8 pad2; u16 qr2Port; u32 qr2IP; int qr2Reservation; u32 qr2IPList[32]; u16 qr2PortList[32]; struct _ServerBrowser *sbObj; BOOL sbUpdateFlag; u64 sbUpdateTick; int sbPidList[32]; u64 sbUpdateRequestTick; u8 nnRecvCount; u8 nnFailureCount; u16 nnCookieRand; int nnLastCookie; u64 nnFailedTime; u64 nnFinishTime; struct { u8 isQR2; u8 retryCount; u16 port; u32 ip; int cookie; } nnInfo; u32 state; u8 clLinkProgress; u8 friendCount; u8 distantFriend; u8 resvWaitCount; u8 closeState; u8 cancelState; u8 scResvRetryCount; u8 synResendCount; u8 cancelSynResendCount; u8 clWaitTimeoutCount; u8 stopSCFlag; u8 pad3; u16 baseLatency; u16 cancelBaseLatency; u16 searchPort; u16 pad4; u32 searchIP; BOOL cmdResendFlag; u64 cmdResendTick; u32 cmdTimeoutTime; u64 cmdTimeoutStartTick; u32 synAckBit; u32 cancelSynAckBit; u32 friendAcceptBit; u64 lastSynSent; u64 lastCancelSynSent; u64 closedTime; u64 clWaitTime; int profileID; int reqProfileID; int priorProfileID; int cbEventPid; u32 ipList[32]; u16 portList[32]; u8 aidList[32]; u32 validAidBitmap; s8 *gameName; s8 *secretKey; void *friendList; int friendListLen; u8 friendIdxList[64]; int friendIdxListLen; u32 svDataBak[33]; struct { u8 command; u8 count; u16 port; u32 ip; u32 data[32]; int profileID; int len; u64 sendTime; } cmdCnt; void *matchedCallback; void *matchedParam; void *newClientCallback; void *newClientParam; void *evalCallback; void *evalParam; void *stopSCCallback; void *stopSCParam; } matchcnt; struct { struct { u8 *sendBuffer; u8 *recvBuffer; int recvBufferSize; int sendingSize; int recvingSize; int requestSendSize; int requestRecvSize; u8 sendState; u8 recvState; u8 lastRecvState; u8 pads[3]; u16 lastRecvType; u64 previousRecvTick; u32 recvTimeoutTime; } connections[32]; void *sendCallback; void *recvCallback; void *recvTimeoutCallback; void *pingCallback; u16 sendSplitMax; u8 unk_612_tail[2]; } transinfo; } friendsMatchCtl;
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
    void *dataTransferServerCallback;
    void *dataTransferClientCallback;
    void *connectionClosedCallback;
    void *connectionClosedCBArg;
    void *newClientCallback;
    void *newClientCallbackArg;
    void *hostMatchCallback;
    void *fatalErrorCallback;
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
    u8 pad_10E7[1];
} NintendoWFCManager;

extern NintendoWFCManager *_0221A680;

void ov00_021E700C(void);

void ov00_021E700C(void) {
    if (!DWC_CheckHasProfile(_0221A680->userData)) {
        DWCFriendData friendData;
        DWC_CreateExchangeToken(_0221A680->userData, &friendData);
    } else {
        DWCFriendData friendData;
        DWC_CreateExchangeToken(_0221A680->userData, &friendData);
    }

    for (int i = 0; i < 32; i++) {
        DWC_GetFriendDataType(&_0221A680->friends[i]);
        DWC_IsBuddyFriendData(&_0221A680->friends[i]);
    }
}

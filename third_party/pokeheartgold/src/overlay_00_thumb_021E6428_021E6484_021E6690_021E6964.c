#include "global.h"
#include "gs/nonport.h"
#include "overlay_00_nintendo_wfcmanager.h"
#include "constants/heap.h"
#include "save.h"

struct DWCstAccFlag;

struct DWCstAccFriendKey;

struct DWCstAccGsProfileId;

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct DWCstControl;

struct DWCstFriendControl;

struct DWCstInetControl;

struct DWCstLoginControl;

struct DWCstMatchCommandControl;

struct DWCstMatchControl;

struct DWCstNNInfo;

struct DWCstTransportConnection;

struct DWCstTransportInfo;

struct GTI2Connection;

struct GTI2Socket;

struct NintendoWFCManager;

struct SOCInAddr;

struct SOCSockAddr;

struct SOCSockAddrIn;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct _ServerBrowser;

struct qr2_buffer_s;

struct qr2_implementation_s;

struct qr2_keybuffer_s;

struct tagDWCWDSData;

typedef enum {
    DWC_WDS_STATE_IDLE = 0,
    DWC_WDS_STATE_PROCESS = 1,
    DWC_WDS_STATE_COMPLETED = 2,
    DWC_WDS_STATE_FAILED = 3,
    DWC_WDS_STATE_ERROR = 4
} DWCWDSState;

typedef struct tagDWCWDSData {
    u8 ssid[32];
    u8 wep[32];
    int wepMode;
    char apnum[10];
} DWCWDSData;

typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;

} DWCInetControl;

typedef enum {
    DWC_ERROR_NONE = 0,
    DWC_ERROR_DS_MEMORY_ANY,
    DWC_ERROR_AUTH_ANY,
    DWC_ERROR_AUTH_OUT_OF_SERVICE,
    DWC_ERROR_AUTH_STOP_SERVICE,
    DWC_ERROR_AC_ANY,
    DWC_ERROR_NETWORK,
    DWC_ERROR_GHTTP_ANY,
    DWC_ERROR_DISCONNECTED,
    DWC_ERROR_FATAL,
    DWC_ERROR_FRIENDS_SHORTAGE,
    DWC_ERROR_NOT_FRIEND_SERVER,
    DWC_ERROR_MO_SC_CONNECT_BLOCK,
    DWC_ERROR_SERVER_FULL,
    DWC_ERROR_ND_ANY,
    DWC_ERROR_ND_HTTP,
    DWC_ERROR_SVL_ANY,
    DWC_ERROR_SVL_HTTP,
    DWC_ERROR_PROF_ANY,
    DWC_ERROR_PROF_HTTP,
    DWC_ERROR_NUM
} DWCError;

typedef struct DWCstAccFlag {
    u32 flags;
    u32 reserved;
    u32 reserved1;
} DWCAccFlag;

typedef struct DWCstAccLoginId {
    u32 id_data;
    u32 userid_lo32;
    u32 playerid;
} DWCAccLoginId;

typedef struct DWCstAccFriendKey {
    u32 id_data;
    u32 friendkey_lo32;
    u32 friendkey_hi32;
} DWCAccFriendKey;

typedef struct DWCstAccGsProfileId {
    u32 id_data;
    int id;
    u32 reserved;
} DWCAccGsProfileId;

typedef union DWCstAccFriendData {
    DWCAccFlag flags;
    DWCAccLoginId login_id;
    DWCAccFriendKey friend_key;
    DWCAccGsProfileId gs_profile_id;
} DWCAccFriendData;

typedef struct DWCstAccUserData {
    int size;
    DWCAccLoginId pseudo;
    DWCAccLoginId authentic;
    int gs_profile_id;
    int flag;
    u32 gamecode;
    int reserved[5];
    u32 crc32;
} DWCAccUserData;

typedef DWCAccUserData DWCUserData;

typedef DWCAccFriendData DWCFriendData;

typedef enum _GPResult {
    GP_NO_ERROR,
    GP_MEMORY_ERROR,
    GP_PARAMETER_ERROR,
    GP_NETWORK_ERROR,
    GP_SERVER_ERROR
} GPResult;

typedef void *GPConnection;

typedef int GPProfile;

typedef struct {
    GPResult result;
    GPProfile profile;
    char uniquenick[21];
} GPConnectResponseArg;

typedef int GT2Bool;

typedef unsigned char GT2Byte;

typedef struct GTI2Socket *GT2Socket;

typedef struct GTI2Connection *GT2Connection;

typedef enum {
    GT2Success,

    GT2OutOfMemory,
    GT2Rejected,
    GT2NetworkError,
    GT2AddressError,
    GT2DuplicateAddress,
    GT2TimedOut,
    GT2NegotiationError,
} GT2Result;

typedef enum {
    GT2LocalClose,
    GT2RemoteClose,

    GT2CommunicationError,
    GT2SocketError,
    GT2NotEnoughMemory
} GT2CloseReason;

typedef void (*gt2ConnectedCallback)(
    GT2Connection connection,
    GT2Result result,
    GT2Byte *message,
    int len);

typedef void (*gt2ReceivedCallback)(
    GT2Connection connection,
    GT2Byte *message,
    int len,
    GT2Bool reliable);

typedef void (*gt2ClosedCallback)(
    GT2Connection connection,
    GT2CloseReason reason);

typedef void (*gt2PingCallback)(
    GT2Connection connection,
    int latency);

typedef struct {
    gt2ConnectedCallback connected;
    gt2ReceivedCallback received;
    gt2ClosedCallback closed;
    gt2PingCallback ping;
} GT2ConnectionCallbacks;

typedef enum {
    e_qrnoerror,
    e_qrwsockerror,
    e_qrbinderror,
    e_qrdnserror,
    e_qrconnerror,
    e_qrnochallengeerror
} qr2_error_t;

typedef enum {
    key_server,
    key_player,
    key_team
} qr2_key_type;

typedef struct qr2_implementation_s *qr2_t;

typedef struct qr2_keybuffer_s *qr2_keybuffer_t;

typedef struct qr2_buffer_s *qr2_buffer_t;

typedef void (*qr2_serverkeycallback_t)(
    int keyid,
    qr2_buffer_t outbuf,
    void *userdata);

typedef void (*qr2_playerteamkeycallback_t)(
    int keyid,
    int index,
    qr2_buffer_t outbuf,
    void *userdata);

typedef void (*qr2_keylistcallback_t)(
    qr2_key_type keytype,
    qr2_keybuffer_t keybuffer,
    void *userdata);

typedef int (*qr2_countcallback_t)(
    qr2_key_type keytype,
    void *userdata);

typedef void (*qr2_adderrorcallback_t)(
    qr2_error_t error,
    char *errmsg,
    void *userdata);

typedef void (*qr2_natnegcallback_t)(
    int cookie,
    void *userdata);

typedef void (*qr2_clientmessagecallback_t)(
    char *data,
    int len,
    void *userdata);

typedef void (*qr2_publicaddresscallback_t)(
    unsigned int ip,
    unsigned short port,
    void *userdata);

typedef void (*cdkey_process_t)(
    char *buf,
    int len,
    struct SOCSockAddr *fromaddr);

struct qr2_implementation_s {
    SOCKET hbsock;
    char gamename[64];
    char secret_key[64];
    char instance_key[4];
    qr2_serverkeycallback_t server_key_callback;
    qr2_playerteamkeycallback_t player_key_callback;
    qr2_playerteamkeycallback_t team_key_callback;
    qr2_keylistcallback_t key_list_callback;
    qr2_countcallback_t playerteam_count_callback;
    qr2_adderrorcallback_t adderror_callback;
    qr2_natnegcallback_t nn_callback;
    qr2_clientmessagecallback_t cm_callback;
    qr2_publicaddresscallback_t pa_callback;
    gsi_time lastheartbeat;
    gsi_time lastka;
    int userstatechangerequested;
    int listed_state;
    int ispublic;
    int qport;
    int read_socket;
    int nat_negotiate;
    struct SOCSockAddrIn hbaddr;
    cdkey_process_t cdkeyprocess;
    int client_message_keys[10];
    int cur_message_key;
    unsigned int publicip;
    unsigned short publicport;
    void *udata;
};

typedef struct _ServerBrowser *ServerBrowser;

typedef enum {
    DWC_FRIEND_STATE_INIT = 0,
    DWC_FRIEND_STATE_PERS_LOGIN,
    DWC_FRIEND_STATE_LOGON,
    DWC_FRIEND_STATE_NUM
} DWCFriendState;

typedef void (*DWCUpdateServersCallback)(DWCError error, BOOL isChanged, void *param);

typedef void (*DWCFriendStatusCallback)(int index, u8 status, const char *statusString, void *param);

typedef void (*DWCDeleteFriendListCallback)(int deletedIndex, int srcIndex, void *param);

typedef void (*DWCBuddyFriendCallback)(int index, void *param);

typedef void (*DWCStorageLoginCallback)(DWCError error, void *param);

typedef void (*DWCSaveToServerCallback)(BOOL success, BOOL isPublic, void *param);

typedef void (*DWCLoadFromServerCallback)(BOOL success, int index, char *data, int len, void *param);

typedef struct DWCstFriendControl {
    DWCFriendState state;
    GPConnection *pGpObj;
    u32 gpProcessCount;
    OSTick lastGpProcess;
    int friendListLen;
    DWCFriendData *friendList;
    u8 buddyUpdateIdx;
    u8 friendListChanged;
    u8 buddyUpdateState;
    vu8 svUpdateComplete;
    u32 persCallbackLevel;
    int profileID;
    const u16 *playerName;
    DWCUpdateServersCallback updateCallback;
    void *updateParam;
    DWCFriendStatusCallback statusCallback;
    void *statusParam;
    DWCDeleteFriendListCallback deleteCallback;
    void *deleteParam;
    DWCBuddyFriendCallback buddyCallback;
    void *buddyParam;
    DWCStorageLoginCallback persLoginCallback;
    void *persLoginParam;
    DWCSaveToServerCallback saveCallback;
    DWCLoadFromServerCallback loadCallback;
} DWCFriendControl;

typedef enum {
    DWC_LOGIN_STATE_INIT = 0,
    DWC_LOGIN_STATE_REMOTE_AUTH,
    DWC_LOGIN_STATE_CONNECTING,
    DWC_LOGIN_STATE_GPGETINFO,
    DWC_LOGIN_STATE_GPSETINFO,
    DWC_LOGIN_STATE_CONNECTED,
    DWC_LOGIN_STATE_NUM
} DWCLoginState;

typedef void (*DWCLoginCallback)(DWCError error, int profileID, void *param);

typedef struct DWCstLoginControl {
    GPConnection *pGpObj;
    DWCLoginState state;
    int productID;
    u32 gamecode;
    const u16 *playerName;
    DWCLoginCallback callback;
    void *param;
    DWCUserData *userdata;
    void *bmwork;
    void *http;
    OSTick startTick;
    u32 connectFlag;
    OSTick connectTick;
    DWCAccLoginId tempLoginId;
    char authToken[256];
    char partnerChallenge[256];
    char username[(20 + 1)];
    GPConnectResponseArg gpconnectresponsearg;
} DWCLoginControl;

typedef enum {
    DWC_MATCH_STATE_INIT = 0,
    DWC_MATCH_STATE_CL_WAITING,
    DWC_MATCH_STATE_CL_SEARCH_OWN,
    DWC_MATCH_STATE_CL_SEARCH_HOST,
    DWC_MATCH_STATE_CL_WAIT_RESV,
    DWC_MATCH_STATE_CL_SEARCH_NN_HOST,
    DWC_MATCH_STATE_CL_NN,
    DWC_MATCH_STATE_CL_GT2,
    DWC_MATCH_STATE_CL_CANCEL_SYN,
    DWC_MATCH_STATE_CL_SYN,
    DWC_MATCH_STATE_SV_WAITING,
    DWC_MATCH_STATE_SV_OWN_NN,
    DWC_MATCH_STATE_SV_OWN_GT2,
    DWC_MATCH_STATE_SV_WAIT_CL_LINK,
    DWC_MATCH_STATE_SV_CANCEL_SYN,
    DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT,
    DWC_MATCH_STATE_SV_SYN,
    DWC_MATCH_STATE_SV_SYN_WAIT,
    DWC_MATCH_STATE_WAIT_CLOSE,
    DWC_MATCH_STATE_SV_POLL_TIMEOUT,
    DWC_MATCH_STATE_NUM
} DWCMatchState;

typedef void (*DWCMatchedCallback)(DWCError error, BOOL cancel, void *param);

typedef void (*DWCMatchedSCCallback)(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index, void *param);

typedef void (*DWCNewClientCallback)(int index, void *param);

typedef int (*DWCEvalPlayerCallback)(int index, void *param);

typedef void (*DWCStopSCCallback)(void *param);

typedef struct DWCstNNInfo {
    u8 isQR2;
    u8 retryCount;
    u16 port;
    u32 ip;
    int cookie;
} DWCNNInfo;

typedef struct DWCstMatchCommandControl {
    u8 command;
    u8 count;
    u16 port;
    u32 ip;
    u32 data[32];
    int profileID;
    int len;
    OSTick sendTime;
} DWCMatchCommandControl;

typedef struct DWCstMatchControl {
    GPConnection *pGpObj;
    GT2Socket *pGt2Socket;
    GT2ConnectionCallbacks *gt2Callbacks;
    u8 gt2ConnectCount;
    u8 gt2NumConnection;
    u8 gt2NumValidConn;
    u8 pad1;
    qr2_t qr2Obj;
    vu8 qr2NNFinishCount;
    vu8 qr2MatchType;
    vu8 qr2NumEntry;
    vu8 qr2IsReserved;

    u8 qr2ShutdownFlag;
    u8 pad2;

    u16 qr2Port;
    u32 qr2IP;
    volatile int qr2Reservation;
    u32 qr2IPList[32];
    u16 qr2PortList[32];
    ServerBrowser sbObj;
    BOOL sbUpdateFlag;
    OSTick sbUpdateTick;
    int sbPidList[32];
    OSTick sbUpdateRequestTick;
    u8 nnRecvCount;
    u8 nnFailureCount;
    u16 nnCookieRand;
    int nnLastCookie;
    OSTick nnFailedTime;
    OSTick nnFinishTime;
    DWCNNInfo nnInfo;
    DWCMatchState state;
    u8 clLinkProgress;
    u8 friendCount;
    u8 distantFriend;
    u8 resvWaitCount;
    u8 closeState;
    u8 cancelState;
    u8 scResvRetryCount;
    u8 synResendCount;
    u8 cancelSynResendCount;
    u8 clWaitTimeoutCount;

    u8 stopSCFlag;
    u8 pad3;

    u16 baseLatency;
    u16 cancelBaseLatency;
    u16 searchPort;

    u16 pad4;

    u32 searchIP;
    BOOL cmdResendFlag;
    OSTick cmdResendTick;
    u32 cmdTimeoutTime;
    OSTick cmdTimeoutStartTick;
    u32 synAckBit;
    u32 cancelSynAckBit;

    u32 friendAcceptBit;

    OSTick lastSynSent;
    OSTick lastCancelSynSent;
    OSTick closedTime;
    OSTick clWaitTime;
    volatile int profileID;
    int reqProfileID;
    int priorProfileID;
    int cbEventPid;
    u32 ipList[32];
    u16 portList[32];
    u8 aidList[32];
    u32 validAidBitmap;
    const char *gameName;
    const char *secretKey;
    const DWCFriendData *friendList;
    int friendListLen;
    u8 friendIdxList[64];
    int friendIdxListLen;
    u32 svDataBak[32 + 1];
    DWCMatchCommandControl cmdCnt;
    DWCMatchedSCCallback matchedCallback;
    void *matchedParam;
    DWCNewClientCallback newClientCallback;
    void *newClientParam;
    DWCEvalPlayerCallback evalCallback;
    void *evalParam;

    DWCStopSCCallback stopSCCallback;
    void *stopSCParam;

} DWCMatchControl;

typedef struct DWCstTransportInfo DWCTransportInfo;

typedef struct DWCstTransportConnection DWCTransportConnection;

typedef void (*DWCUserSendCallback)(int size, u8 aid);

typedef void (*DWCUserRecvCallback)(u8 aid, u8 *buffer, int size);

typedef void (*DWCUserRecvTimeoutCallback)(u8 aid);

typedef void (*DWCUserPingCallback)(int latency, u8 aid);

struct DWCstTransportConnection {
    const u8 *sendBuffer;
    u8 *recvBuffer;
    int recvBufferSize;
    int sendingSize;
    int recvingSize;
    int requestSendSize;
    int requestRecvSize;
    u8 sendState;
    u8 recvState;
    u8 lastRecvState;
    u8 pads[3];
    u16 lastRecvType;
    OSTick previousRecvTick;
    u32 recvTimeoutTime;
};

struct DWCstTransportInfo {
    DWCTransportConnection connections[32];
    DWCUserSendCallback sendCallback;
    DWCUserRecvCallback recvCallback;
    DWCUserRecvTimeoutCallback recvTimeoutCallback;
    DWCUserPingCallback pingCallback;
    u16 sendSplitMax;
};

typedef enum {
    DWC_STATE_INIT = 0,
    DWC_STATE_AVAILABLE_CHECK,
    DWC_STATE_LOGIN,
    DWC_STATE_ONLINE,
    DWC_STATE_UPDATE_SERVERS,
    DWC_STATE_MATCHING,
    DWC_STATE_CONNECTED,
    DWC_STATE_NUM
} DWCState;

typedef void (*DWCConnectionClosedCallback)(DWCError error, BOOL isLocal, BOOL isServer, u8 aid, int index, void *param);

typedef struct DWCstControl {
    GT2Socket gt2Socket;
    GT2ConnectionCallbacks gt2Callbacks;
    int gt2SendBufSize;
    int gt2RecvBufSize;
    GPConnection gpObj;
    DWCUserData *userdata;
    DWCState state;
    DWCState lastState;
    u8 aid;
    u8 ownCloseFlag;
    u16 playerName[26];
    int profileID;
    const char *gameName;
    const char *secretKey;
    DWCLoginCallback loginCallback;
    void *loginParam;
    DWCUpdateServersCallback updateServersCallback;
    void *updateServersParam;
    DWCMatchedCallback matchedCallback;
    void *matchedParam;
    DWCMatchedSCCallback matchedSCCallback;
    void *matchedSCParam;
    DWCConnectionClosedCallback closedCallback;
    void *closedParam;
    DWCLoginControl logcnt;
    DWCFriendControl friendcnt;
    DWCMatchControl matchcnt;
    DWCTransportInfo transinfo;
} DWCFriendsMatchControl;

extern u8 DWC_GetMyAID(void);

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

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

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

int ov00_021E6964(void);

int ov00_021E6964(void) {
    if (_0221A680) {
        if (_0221A680->state == WFC_STATE_MATCHMAKING_SUCCESS
            || _0221A680->state == WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS
            || _0221A680->state == WFC_STATE_CLOSE_CONNECTION) {
            return DWC_GetMyAID();
        }
    }

    return -1;
}

void ov00_021E6994(void);

void ov00_021E6994(void) {
    *((s32 *)&_0221A680->voiceChatActive) = 0;
}

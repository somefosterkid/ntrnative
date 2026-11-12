// Black overlay 114 NHTTP code: the prototypes of its functions as Black defines them (the file layout of pokebw2 overlay 189).
#ifndef BWPORT_DWC_NHTTP_INTERNAL_H
#define BWPORT_DWC_NHTTP_INTERNAL_H

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"

#define NHTTP_HEADER_SIZE 0x400
#define NHTTP_HEADER_BLOCK_SIZE 0x200
typedef struct NHttpHeaderBlock {
    struct NHttpHeaderBlock *next;
    char data[NHTTP_HEADER_BLOCK_SIZE];
} NHttpHeaderBlock;
typedef void (*NHttpReleaseFunc)(void *buffer, NHttpFreeFunc free, void *param);
typedef struct NHttpResponse {
    // Written by the receiving thread while the caller's thread reads it
    volatile int headerSize;
    int bufferUsed;
    int bodyReceived;
    volatile int contentLength;
    BOOL succeeded;
    BOOL headerDone;
    int statusCode;
    int bufferSize;
    char *headerCopy;
    char *valueCopy;
    char *buffer;
    int unk_2C;
    NHttpReleaseFunc release;
    NHttpHeaderBlock *blocks;
    char header[NHTTP_HEADER_SIZE];
    void *userParam;
} NHttpResponse;
static inline BOOL NHTTPi_isUpper(int c) { return (c >= 'A') & (c <= 'Z'); }
static inline char NHTTPi_toLower(int c) {
    if (NHTTPi_isUpper(c)) {
        c += 'a' - 'A';
    }
    return c;
}

// The structs below are pokebw2's (lib/dwc/src/nhttp/nhttp_internal.h); Black's offsets are the same.
#define NHTTP_SEND_BUFFER_SIZE 0x4000
#define NHTTP_RECV_BUFFER_SIZE 0x8000
#define NHTTP_BOUNDARY_SIZE 0x14

// The methods of a request
#define NHTTP_METHOD_GET 0
#define NHTTP_METHOD_POST 1
#define NHTTP_METHOD_HEAD 2

// A connection's stage, which the thread moves it through
enum {
    NHTTP_STAGE_NONE,
    NHTTP_STAGE_QUEUED,
    NHTTP_STAGE_SENDING,
    NHTTP_STAGE_RECV_HEADER,
    NHTTP_STAGE_RECV_BODY,
    NHTTP_STAGE_DONE,
};

typedef struct NHttpState NHttpState;
typedef struct NHttpRequest NHttpRequest;
typedef struct NHttpConnection NHttpConnection;
typedef struct NHttpThreadInfo NHttpThreadInfo;
typedef struct NHttpMutexInfo NHttpMutexInfo;

// NHTTP_bgnend's: the allocator, the error and what the thread needs to run
struct NHttpState {
    NHttpAllocFunc alloc;
    NHttpFreeFunc free;
    BOOL started;
    int socket;
    int sslError;
    int error;
    BOOL quit;
    void *stack;
    int unk_20;
    int unk_24;
};

// A part of the request's header or post data: its name and value, and for a post the size of the value
typedef struct NHttpHeader {
    struct NHttpHeader *prev;
    struct NHttpHeader *next;
    char *name;
    char *value;
    int length;
    int binary;
} NHttpHeader;

// A request that was sent and waits for the thread, or that the thread runs
typedef struct NHttpQueueNode {
    struct NHttpQueueNode *prev;
    struct NHttpQueueNode *next;
    int id;
    // The comm thread changes it while the caller's thread cancels it
    NHttpRequest *volatile request;
    int socket;
} NHttpQueueNode;

typedef struct NHttpListInfo {
    NHttpQueueNode *head;
    // The next request's ID, taken by the caller's thread while the HTTP thread works through the queue
    volatile int count;
} NHttpListInfo;

typedef struct NHttpReqInfo {
    NHttpQueueNode *current;
} NHttpReqInfo;

// The mutex that guards the connections, and whether it is made
struct NHttpMutexInfo {
    OSMutex mutex;
    BOOL ready;
};

// The thread that runs the requests, with its message queue (which wakes it when a request was sent) and the buffer
// it sends from
struct NHttpThreadInfo {
    OSMessageQueue queue;
    OSMessage messages[3];
    OSThread thread;
    BOOL queueCreated;
    u8 unk_F0[0x10];
    u8 sendBuffer[NHTTP_SEND_BUFFER_SIZE];
};

// The settings that NHttpSslConnect gives the socket library to make a connection secure
typedef struct NHttpSslInfo {
    u8 unk_000[0x800];
    char *hostName;
    u8 unk_804[0xc];
    u32 (*filter)(u32);
    void *rootCerts;
    int rootCertCount;
    u8 unk_81C[0x14];
} NHttpSslInfo;

// What a request is: its url and the parts of it, the headers and post data it carries, and the settings that
// NHTTP_SetRootCA and the like give
struct NHttpRequest {
    BOOL canceled;
    BOOL sent;
    BOOL https;
    BOOL useProxy;
    BOOL rawPost;
    int hostEnd;
    int pathStart;
    int method;
    int port;
    char *url;
    char *host;
    NHttpResponse *response;
    NHttpHeader *headers;
    NHttpHeader *postData;
    char boundary[NHTTP_BOUNDARY_SIZE];
    char authorization[0x5c];
    int authorizationSize;
    NHttpSslInfo *ssl;
    int unk_B0;
    int unk_B4;
    int unk_B8;
    int unk_BC;
    void *rootCerts;
    int rootCertCount;
    int unk_C8;
    int unk_CC;
    int verifyOption;
    int postMode;
    int skipVerify;
    int unk_DC;
    u8 unk_E0[4];
    char proxyHost[0x100];
    int proxyPort;
    char proxyAuthorization[0x5c];
    int proxyAuthorizationSize;
    int unk_248;
    const void *rawData;
    int rawSize;
    int unk_254;
};

// What the caller of NHTTP_CreateRequest has: the request and its response, the stage the thread has brought it to and
// the buffer the thread reads from the socket into. Connections are chained in a list
struct NHttpConnection {
    int stage;
    int status;
    int unk_08;
    BOOL running;
    NHttpRequest *request;
    NHttpResponse *response;
    int id;
    NHttpCallback callback;
    NHttpConnection *next;
    void *userBuffer;
    int userBufferSize;
    int virtualContentLength;
    int unk_30;
    u8 unk_34[0xc];
    u8 recvBuffer[NHTTP_RECV_BUFFER_SIZE];
    u32 recvPos;
    u32 recvSize;
    int waiters;
};

// The library: its state, the requests that wait, the request that the thread runs, the mutex and the thread
typedef struct NHttpRoot {
    NHttpState state;
    NHttpListInfo list;
    NHttpReqInfo req;
    NHttpMutexInfo mutexInfo;
    NHttpThreadInfo threadInfo;
    // No code of Black's reads it; d_nhttp_common.c's static of the type runs 0x10 bytes past threadInfo, up to
    // dwc_ghttp.c's .bss (Black 2 has the same 0x4160 bytes)
    u8 unk_4150[0x10];
} NHttpRoot;

// What the thread keeps on its stack while it runs requests: the request that it takes from the list, the host of the
// last request and its address (kept to connect to the same one again), and how far each step of the request got
typedef struct NHttpThreadWork {
    int id;
    char hostName[0x100];
    char unk_104[0x200];
    char statusLine[0xe];
    u8 unk_312[2];
    u32 address;
    u32 prevAddress;
    int port;
    int prevPort;
    int sendCount;
    int recvCount;
    int contentLength;
    int error;
    int retry;
    BOOL keepAlive;
    BOOL chunked;
} NHttpThreadWork;

// What the callback of a connection gets for NHTTP_EVENT_POST: the name of the part that is sent, and the buffer
// that the callback sets (with its size) to give the part's data. The offset is how much was given before
typedef struct NHttpPostEvent {
    const char *name;
    void *buffer;
    int bufferSize;
    int offset;
} NHttpPostEvent;

// What the callback of a connection gets for the events of the buffer being full and of the body being received: the
// buffer of the answer, its size, and how much of it is filled. The callback may set them to give another buffer
typedef struct NHttpBufferEvent {
    void *buffer;
    int bufferSize;
    int used;
} NHttpBufferEvent;

// nhttp_control.c: the boundary of a multipart post
extern const char data_ov114_021cdfc8[0x18];

#define NHTTP_SOCKET_ERROR (-1001)
#define NHTTP_SOCKET_CANCELED (-1002)
#define NHTTP_BUFFER_FULL (-1003)
// Where the first header field starts in a response's header, after the status line
#define NHTTP_FIRST_FIELD 12

// How the data of a POST is sent: NHTTP_POST_AUTO uses multipart as soon as one part is binary
#define NHTTP_POST_AUTO 0
#define NHTTP_POST_URLENCODED 1
#define NHTTP_POST_MULTIPART 2

// What a part of the post data is sent as
#define NHTTP_SEND_RAW 0
#define NHTTP_SEND_PART 1
#define NHTTP_SEND_ENCODED 2

// Where the thread that waits for a connection to end sleeps
typedef struct NHttpWaitInfo {
    s32 ready;
    OSMutex mutex;
    OSThreadQueue waiters;
    u8 unk_24[0x10];
} NHttpWaitInfo;

// The prototypes as pokebw2 (lib/dwc/src/nhttp/nhttp_internal.h) types them, in address order
void func_ov114_021b9b4c(OSMutex *mutex);
void func_ov114_021b9b58(OSMutex *mutex);
BOOL func_ov114_021b9b64(NHttpThreadInfo *info, int priority, void *stack);
void func_ov114_021b9bd4(NHttpThreadInfo *info, NHttpState *state);
BOOL func_ov114_021b9bf4(OSMessageQueue *queue);
void func_ov114_021b9c08(OSMessageQueue *queue);
void func_ov114_021b9c1c(NHttpThreadInfo *info, BOOL outside);
void func_ov114_021b9c60(void *arg);
u32 func_ov114_021b9c6c(u32 flags);
u32 func_ov114_021b9c78(u32 flags);
void func_ov114_021b9c80(void);
int func_ov114_021b9ca4(int sock);
int func_ov114_021b9ce4(NHttpRequest *request);
int func_ov114_021b9cfc(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock);
int func_ov114_021b9d8c(NHttpState *state, NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock, u32 addr, int port);
int func_ov114_021b9e3c(NHttpState *state, NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock);
int func_ov114_021b9ecc(NHttpConnection *connection, int sock, void *buffer, int size, int flags);
int func_ov114_021b9f94(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock, void *buffer, int size, int flags);
int func_ov114_021ba014(int sock, const void *buffer, int size, int flags);
int func_ov114_021ba020(NHttpRequest *request, int sock, const void *buffer, int size, int flags);
void func_ov114_021ba068(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock);
u32 func_ov114_021ba098(NHttpRequest *request, const char *host);
void func_ov114_021ba100(void *dest, const void *src, u32 size);
int func_ov114_021ba118(const char *s);
int func_ov114_021ba124(const char *s, int max);
int func_ov114_021ba130(const char *a, const char *b);
void func_ov114_021ba13c(void *dest, u32 size);
int func_ov114_021ba150(const char *a, const char *b, int size);
int NHTTPi_getUrlEncodedSize(const char *s);
int func_ov114_021ba25c(const char *s, int length);
int NHTTPi_encodeUrlChar(char *dest, int c);
int func_ov114_021ba35c(const char *s, int size);
int NHTTPi_strToInt(const char *s, int size);
int func_ov114_021ba4e4(char *dest, u32 value);
int func_ov114_021ba590(const char *a, const char *b);
int NHTTPi_strtonum(const char *s, int size);
int NHTTPi_memfind(const char *haystack, int haystackSize, const char *needle, int needleSize);
int func_ov114_021ba714(char *dest, const char *src);
void func_ov114_021ba7dc(NHttpState *state);
void *func_ov114_021ba820(u32 size, int align);
void func_ov114_021ba850(void *ptr);
void func_ov114_021ba874(NHttpState *state, int error);
void func_ov114_021ba87c(NHttpState *state, int error);
int func_ov114_021ba884(NHttpState *state);
BOOL func_ov114_021ba88c(NHttpRoot *root, NHttpAllocFunc alloc, NHttpFreeFunc free, int priority);
void func_ov114_021ba988(NHttpRoot *root, void (*callback)(void));
int func_ov114_021baa14(NHttpState *state);
BOOL addHdrList(NHttpHeader **list, NHttpState *state, const char *name, const char *value);
char func_ov114_021bab24(u8 c);
BOOL func_ov114_021bab58(NHttpRequest *request, const char *data, int length);
NHttpHeader *NHTTPi_getHdrFromList(void *list);
BOOL func_ov114_021bac38(NHttpRequest *request, NHttpState *state, const char *name, const char *value);
BOOL func_ov114_021bac58(NHttpRequest *request, NHttpState *state, const char *name, const char *value);
void func_ov114_021bacec(NHttpListInfo *info);
int func_ov114_021bacfc(NHttpListInfo *info, NHttpRequest *request);
NHttpQueueNode *func_ov114_021bad94(NHttpListInfo *info, int id);
BOOL func_ov114_021bade0(NHttpListInfo *info, NHttpMutexInfo *mutexInfo, int id);
void func_ov114_021bae90(NHttpListInfo *info, NHttpMutexInfo *mutexInfo);
NHttpQueueNode *func_ov114_021baec8(NHttpListInfo *info);
void func_ov114_021baed4(const NHttpResponse *response, int offset, NHttpHeaderBlock **block, int *blockOffset);
char func_ov114_021baf2c(const NHttpResponse *response, NHttpHeaderBlock **block, int *blockOffset);
int func_ov114_021bafa0(const NHttpResponse *response, int start, int end, int *colon, int *lineEnd);
int func_ov114_021bb0bc(const NHttpResponse *response, int start, int end);
int func_ov114_021bb12c(const NHttpResponse *response, int start, int end, const char *token, int terminator);
BOOL func_ov114_021bb210(NHttpResponse *response, char *dest, int start, int size);
BOOL func_ov114_021bb2fc(NHttpResponse *response, u32 size);
int func_ov114_021bb310(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock, int offset, int flags);
int func_ov114_021bb344(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock, int offset, int size, int flags);
void func_ov114_021bb3c0(NHttpReqInfo *info);
NHttpRequest *func_ov114_021bb3cc(NHttpState *state, const char *url, int method, void *buffer, int bufferSize, void *param, int unused, NHttpReleaseFunc release);
void func_ov114_021bb8b4(NHttpRoot *root, NHttpRequest *request);
void func_ov114_021bb8f0(NHttpHeader *list);
BOOL func_ov114_021bb934(NHttpMutexInfo *mutexInfo, NHttpRequest *request);
int func_ov114_021bb97c(NHttpRoot *root, NHttpRequest *request);
BOOL func_ov114_021bba14(NHttpRoot *root, int id);
void func_ov114_021bbaac(NHttpRoot *root);
void func_ov114_021bbb28(NHttpMutexInfo *mutexInfo, NHttpResponse *response);
int func_ov114_021bbbc8(const NHttpResponse *response, const char *name, int *valueStart);
int func_ov114_021bbcf0(NHttpResponse *response, const char *name, char **value);
int func_ov114_021bbdfc(NHttpState *state, NHttpResponse *response, char **header);
void func_ov114_021bbe94(NHttpThreadInfo *info);
void func_ov114_021bbea0(NHttpThreadInfo *info);
void func_ov114_021bbeac(NHttpThreadInfo *info);
BOOL func_ov114_021bbeb8(NHttpThreadInfo *info);
BOOL func_ov114_021bbec0(const char *tail, int count);
int func_ov114_021bbf44(NHttpRequest *request, u8 *sendBuffer, int sock, int *count, const u8 *data, u32 size);
int func_ov114_021bc00c(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock);
BOOL func_ov114_021bc098(NHttpMutexInfo *mutexInfo, NHttpRequest *request, const char *name, int *length, int mode);
int func_ov114_021bc160(NHttpMutexInfo *mutexInfo, NHttpRequest *request, u8 *sendBuffer, const char *name, int sock, int *count, int mode);
BOOL func_ov114_021bc2fc(NHttpMutexInfo *mutexInfo, NHttpResponse *response);
int func_ov114_021bc384(NHttpThreadWork *work, const void *data, int size);
int func_ov114_021bc40c(NHttpThreadWork *work);
int func_ov114_021bc488(NHttpThreadWork *work);
int func_ov114_021bc504(NHttpThreadWork *work);
BOOL func_ov114_021bc740(NHttpThreadWork *work);
int func_ov114_021bc924(NHttpThreadWork *work);
int func_ov114_021bc9f0(NHttpThreadWork *work);
int func_ov114_021bcb74(NHttpThreadWork *work);
int func_ov114_021bcf24(NHttpThreadWork *work);
void func_ov114_021bd220(NHttpThreadWork *work);
void func_ov114_021bd270(NHttpThreadWork *work);
BOOL func_ov114_021bd3b8(NHttpThreadWork *work);
BOOL func_ov114_021bd430(NHttpThreadWork *work);
BOOL func_ov114_021bd548(NHttpThreadWork *work);
int func_ov114_021bd6dc(NHttpThreadWork *work);
int func_ov114_021bd7e8(NHttpThreadWork *work);
BOOL func_ov114_021bdb68(NHttpThreadWork *work);
BOOL func_ov114_021bdd34(NHttpThreadWork *work);
BOOL func_ov114_021bdff8(NHttpThreadWork *work);
void func_ov114_021be4d4(void);
int func_ov114_021be648(NHttpAllocFunc alloc, NHttpFreeFunc free, u32 priority);
void func_ov114_021be680(void (*callback)(void));
void func_ov114_021be698(void);
int func_ov114_021be6ac(void);
int func_ov114_021be6bc(int connection, const char *name, const char *value);
int func_ov114_021be6ec(NHttpRequest *request, const char *name, const char *value);
int func_ov114_021be748(int connection, const char *name, const char *value);
int func_ov114_021be79c(int connection, const void *data, u32 size);
int func_ov114_021be7fc(int connection, const char *name, char **value);
int func_ov114_021be838(int connection, char **header);
int func_ov114_021be880(int connection);
int func_ov114_021be8b8(int connection, int option);
int func_ov114_021be8e8(int connection, const char *user, const char *password);
int func_ov114_021be9c8(int connection, void *certificates, int count);
int func_ov114_021be9f8(NHttpRequest *request, void *certificates, int count);
NHttpWaitInfo *func_ov114_021bea20(void);
void func_ov114_021bea64(NHttpConnection *connection);
void func_ov114_021bea98(NHttpConnection *connection);
NHttpConnection *func_ov114_021beab8(NHttpMutexInfo *mutexInfo, NHttpConnection *key, int mode);
int func_ov114_021beb88(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
int func_ov114_021beba8(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
NHttpResponse *func_ov114_021bebc8(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
NHttpConnection *func_ov114_021bebe0(NHttpMutexInfo *mutexInfo, NHttpRequest *request);
NHttpConnection *func_ov114_021bebf0(NHttpMutexInfo *mutexInfo, NHttpResponse *response);
NHttpConnection *func_ov114_021bec00(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
NHttpRequest *func_ov114_021bec10(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
NHttpResponse *func_ov114_021bec2c(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
int func_ov114_021bec48(void);
int func_ov114_021bec74(NHttpMutexInfo *mutexInfo, NHttpConnection *connection, const char *name, int offset);
void func_ov114_021bed04(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
void func_ov114_021bed90(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
void func_ov114_021bee1c(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
int func_ov114_021bee4c(NHttpMutexInfo *mutexInfo, NHttpConnection *connection, void *buffer, int size);
int func_ov114_021bee74(NHttpMutexInfo *mutexInfo, NHttpConnection *connection, void *buffer, int size, int used);
void func_ov114_021beec4(NHttpRoot *root);
NHttpRoot *func_ov114_021bef08(void);
NHttpState *func_ov114_021bef3c(NHttpRoot *root);
NHttpListInfo *func_ov114_021bef40(NHttpRoot *root);
NHttpReqInfo *func_ov114_021bef48(NHttpRoot *root);
NHttpThreadInfo *func_ov114_021bef50(NHttpRoot *root);
NHttpMutexInfo *func_ov114_021bef58(NHttpRoot *root);
void func_ov114_021bef60(NHttpConnection *connection, int length);
void func_ov114_021bef6c(void);
void func_ov114_021bef98(NHttpMutexInfo *mutexInfo);
void func_ov114_021befa4(NHttpMutexInfo *mutexInfo);
void func_ov114_021befc8(void);
void func_ov114_021befcc(NHttpMutexInfo *mutexInfo);
void func_ov114_021befd8(NHttpMutexInfo *mutexInfo);
int func_ov114_021befe4(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
int func_ov114_021bf018(NHttpMutexInfo *mutexInfo, NHttpConnection *connection);
int func_ov114_021bf04c(const char *url, int method, void *buffer, u32 bufferSize, NHttpCallback callback, void *arg);
int func_ov114_021bf184(int handle);
int func_ov114_021bf1b0(NHttpConnection *connection);
int func_ov114_021bf288(NHttpConnection *connection);
int func_ov114_021bf308(int handle);
int func_ov114_021bf374(int handle);
int func_ov114_021bf3bc(int handle);
int func_ov114_021bf3e4(int handle, char **buffer, int *size);
void *func_ov114_021bf444(int handle);
int func_ov114_021bf488(int handle);
int func_ov114_021bf4b0(int handle, int *received, int *size);

#endif

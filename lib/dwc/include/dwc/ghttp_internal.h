// Black overlay 114 GHTTP code: the prototypes of its functions as Black defines them (the file layout of pokebw2 overlay 189).
#ifndef BWPORT_DWC_GHTTP_INTERNAL_H
#define BWPORT_DWC_GHTTP_INTERNAL_H

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
#include "gfl/ghttpEncryption.h"

#include "dwc/soap.h"

// GameSpy's ghttp structures that Black's files share; the layout is pokebw2's, which the offsets of Black's code agree
// with (the post at 0x164 of a connection, its progress from 0x168)
typedef struct GHIConnection GHIConnection;
typedef struct GHIEncryptor GHIEncryptor;

typedef void (*GHIPostFunc)(int request, int bytesPosted, int totalBytes, int index, int count, void *param);

// What a request posts: the items (form values, files, an XML message) and how they are sent
typedef enum GHIPostDataType {
    GHIString,
    GHIFileDisk,
    GHIFileMemory,
    GHIXml
} GHIPostDataType;

struct GHIPost {
    DArray data;
    GHIPostFunc callback;
    void *param;
    BOOL hasFiles;
    BOOL hasXml;
    BOOL mixed;  // files and XML: a DIME message
    BOOL autoFree;
};

typedef struct GHIPostStringData {
    char *string;
    int len;
    BOOL invalidChars;
    int extendedChars;
} GHIPostStringData;

typedef struct GHIPostFileDiskData {
    char *filename;
    char *reportFilename;
    char *contentType;
} GHIPostFileDiskData;

typedef struct GHIPostFileMemoryData {
    const char *buffer;
    int len;
    char *reportFilename;
    char *contentType;
} GHIPostFileMemoryData;

typedef struct GHIPostXmlData {
    SoapWriter *writer;
} GHIPostXmlData;

typedef struct GHIPostData {
    GHIPostDataType type;
    char *name;
    union {
        GHIPostStringData string;
        GHIPostFileDiskData fileDisk;
        GHIPostFileMemoryData fileMemory;
        GHIPostXmlData xml;
    } data;
} GHIPostData;

// How far an item of the post of a connection has got
typedef struct GHIPostState {
    GHIPostData *data;
    int pos;
    FILE *file;
    int fileSize;
} GHIPostState;

// The progress of a post and the callback that tells the game of it
typedef struct GHIPostingState {
    DArray states;
    int index;
    int bytesPosted;
    int totalBytes;
    GHIPostFunc callback;
    void *param;
    BOOL isXml;
    BOOL finished;
} GHIPostingState;

// A growing (or fixed) text that a connection builds or receives into
typedef struct GHIBuffer {
    GHIConnection *connection;
    char *data;
    int size;
    int length;
    int pos;
    int sizeIncrement;
    BOOL fixed;
    BOOL dontFree;
    BOOL readOnly;
} GHIBuffer;

// What the encryption keeps of a connection: its SSL connection (ghttpEncryption.c) and whether that has started
typedef struct GHIEncryptorState {
    struct GHISslConnection *ssl;
    BOOL started;
} GHIEncryptorState;

// How a certificate that the server sent is checked (the SSL library tells of what it found, and the callback says
// which of the faults to ignore by clearing their bits)
typedef struct GHISslCertInfo {
    u8 unk_000[0x5b0];
    char issuer[0x100];
    char subject[0x100];
    char commonName[0x100];
} GHISslCertInfo;

typedef int (*GHISslAuthFunc)(u32 result, GHISslCertInfo *info, int length);

// The root certificates of a host
typedef struct GHICertList {
    void **certs;
    int count;
} GHICertList;

// What the SSL library's socket takes to make a connection secure
typedef struct GHISslConnection {
    u8 unk_000[0x800];
    int unk_800;
    u8 unk_804[0xc];
    GHISslAuthFunc authCallback;
    void **certs;
    int certCount;
    u8 unk_81C[0x14];
} GHISslConnection;

// The root certificates of a host that a request may be secured with, and how many have asked for them
typedef struct GHIRootCAEntry {
    char *url;
    GHICertList *certList;
    int unk_08;
    int refCount;
} GHIRootCAEntry;

// The callbacks of a request: when it has ended (returns FALSE to keep the buffer) and while it goes on
typedef BOOL (*GHICompletedFunc)(int request, int result, char *buffer, int bufferLen, void *param);
typedef void (*GHIProgressFunc)(int request, int state, const char *buffer, int bufferLen, int bytesReceived,
                                int totalSize, void *param);


// The encryption of a connection (ghttpEncryption.c): its engine, and the functions that start it, end it and
// (en|de)crypt the data
typedef int (*GHIEncryptFunc)(GHIConnection *connection, GHIEncryptor *encryptor, const char *in, int inSize, char *out,
                              int *outSize);
typedef int (*GHIDecryptFunc)(GHIConnection *connection, GHIEncryptor *encryptor, const char *in, int *inSize,
                              char *out, int *outSize);
struct GHIEncryptor {
    GHIEncryptorState *state;
    int engine;
    BOOL open;
    int unk_0C;
    BOOL active;
    int mode;
    BOOL established;
    int unk_1C;
    int (*init)(GHIConnection *connection, GHIEncryptor *encryptor);
    BOOL (*cleanup)(GHIConnection *connection, GHIEncryptor *encryptor);
    int (*start)(GHIConnection *connection, GHIEncryptor *encryptor);
    GHIEncryptFunc encrypt;
    GHIDecryptFunc decrypt;
    BOOL secure;
};

// A request that is in progress: the slot of the connections' list it has, what it asked for and the state it is in
struct GHIConnection {
    BOOL inUse;
    int request;
    int uniqueId;
    int type;
    int state;
    char *url;
    char *serverAddress;
    u32 serverIp;
    u16 serverPort;
    u8 pad_22[2];
    char *requestPath;
    BOOL secure;
    char *headers;
    int unk_30;
    char *unk_34;
    BOOL blocking;
    BOOL keepAlive;
    int result;
    GHIProgressFunc progressCallback;
    GHICompletedFunc completedCallback;
    void *callbackParam;
    int socket;
    int socketError;
    GHIBuffer sendBuffer;
    GHIBuffer encodeBuffer;
    GHIBuffer recvBuffer;
    GHIBuffer decodeBuffer;
    GHIBuffer fileBuffer;
    BOOL userBuffer;
    int httpMajor;
    int httpMinor;
    int statusCode;
    int statusTextPos;
    int headersPos;
    BOOL completed;
    int bytesReceived;
    int totalSize;
    char *redirectUrl;
    int redirectCount;
    BOOL chunked;
    char chunkHeader[12];
    int chunkHeaderLength;
    int chunkRemaining;
    int chunkState;
    BOOL processing;
    BOOL closing;
    int throttle;
    u32 lastThrottleTime;
    GHIPost *post;
    GHIPostingState postingState;
    int maxRecvTime;
    char *proxyAddress;
    u16 proxyPort;
    u8 pad_192[2];
    GHIEncryptor encryptor;
    u32 *hostLookup;
    u32 lastRecvTime;
};


// The callbacks of a request are gfl/ghttpEncryption.h's
typedef struct DWCGHTTPParam {
    void *param;
    DWCGHTTPCompletedFunc completed;
    DWCGHTTPProgressFunc progress;
    int unk_0C;
    BOOL freeBuffer;
} DWCGHTTPParam;

// A request of DWC, in a list: what the game gave, the buffer that DWC made for it and the request's number
typedef struct DWCGHTTPEntry DWCGHTTPEntry;
struct DWCGHTTPEntry {
    DWCGHTTPParam param;
    char *buffer;
    int request;
    DWCGHTTPEntry *next;
};

// The proxy address that every request uses (GameSpy's ghiProxyAddress), or NULL
extern char *data_ov114_021d3b2c;
// Its port (ghiProxyPort)
extern u16 data_ov114_021d3b28;
// The receive buffer size of a throttled connection (ghiThrottleBufferSize, ghttpCommon.c)
extern s32 data_ov114_021ce5b0;

BOOL func_ov114_021bf520(u32 *param_1);
BOOL func_ov114_021bf5d8(int request, int result, char *buffer, int length, void *param);
void func_ov114_021bf67c(int request, int state, const char *buffer, int length, int received, int total, void *param);
s32 func_ov114_021bf6ec(const char *url, int bufferSize, BOOL freeBuffer, GHIPost **post, DWCGHTTPProgressFunc progress, DWCGHTTPCompletedFunc completed, void *param);
void func_ov114_021bf880(s32 param_1);
DWCGHTTPEntry *func_ov114_021bfa08(const DWCGHTTPParam *param);
void func_ov114_021bfa70(void *param_1);
DWCGHTTPEntry *func_ov114_021bfaf0(int request);
void func_ov114_021bfb20(void);
s32 func_ov114_021bfb90(s32 *param_1, s32 param_2);
s32 func_ov114_021bfbd0(s32 param_1, s32 *param_2, s32 param_3, s32 param_4);
s32 func_ov114_021bfc54(s32 param_1, s32 *param_2, s8 *param_3, s32 param_4);
void func_ov114_021bfcb4(s32 *param_1);
u32 func_ov114_021bfcec(s32 *param_1, s8 *param_2, u32 param_3);
s32 func_ov114_021bfdec(s32 *param_1, s8 *param_2, u32 param_3);
BOOL func_ov114_021bff7c(s32 *param_1, s8 *param_2, s8 *param_3);
s32 func_ov114_021c0004(s32 *param_1, s8 param_2);
void func_ov114_021c00c0(s32 *param_1, u32 param_2);
void func_ov114_021c00fc(s32 *param_1);
BOOL func_ov114_021c011c(s32 *param_1);
void func_ov114_021c0244(s32 *param_1);
void func_ov114_021c02c8(s32 *param_1, s32 param_2, s32 param_3);
void func_ov114_021c0318(s32 *param_1);
void func_ov114_021c0368(void);
void func_ov114_021c036c(void);
void func_ov114_021c0370(void);
void func_ov114_021c0374(void);
BOOL func_ov114_021c0378(GHIConnection *connection);
int func_ov114_021c04d4(GHIConnection *connection, char *buffer, int *bufferLen);
int func_ov114_021c0684(GHIConnection *connection, const char *data, int size);
s32 func_ov114_021c07b8(s32 *param_1, u8 *param_2, s32 param_3);
s32 func_ov114_021c08b0(void);
GHIConnection *func_ov114_021c0998(void);
u32 func_ov114_021c0b4c(s32 *param_1);
u32 func_ov114_021c0c14(GHIConnection *connection);
u32 func_ov114_021c0d40(s32 *param_1);
s32 *func_ov114_021c0da4(s32 param_1);
void func_ov114_021c0df8(u8 *param_1);
void func_ov114_021c0e58(void *arg0);
void func_ov114_021c0fb0(void);
s8 *func_ov114_021c1020(s8 *param_1);
u32 func_ov114_021c109c(s32 param_1, s32 param_2);
s32 func_ov114_021c1184(s8 *param_1, s8 *param_2);
// pokebw2's ghiCleanupRootCAList
u32 func_ov114_021c1210(const char *url);
void func_ov114_021c121c(u32 *param_1, u32 *param_2);
GHICertList *func_ov114_021c1230(s8 *param_1);
void func_ov114_021c135c(s32 *param_1);
BOOL func_ov114_021c137c(const char *url, GHICertList *rootCA);
BOOL func_ov114_021c1500(const char *url);
int func_ov114_021c1668(u32 result, GHISslCertInfo *info, int length);
GHISslConnection *func_ov114_021c1804(const char *url);
s32 func_ov114_021c1920(GHIConnection *connection, GHIEncryptor *encryptor);
s32 func_ov114_021c1a78(GHIConnection *connection, GHIEncryptor *encryptor);
int func_ov114_021c1c78(GHIConnection *connection, GHIEncryptor *encryptor, const char *data, int size, int *sent);
int func_ov114_021c1c80(GHIConnection *connection, GHIEncryptor *encryptor, char *buffer, int *size);
u32 func_ov114_021c1c88(void);
u32 func_ov114_021c1c90(void);
u32 func_ov114_021c1c98(u32 param_1, u32 *param_2);
void func_ov114_021c1d30(s32 *param_1);
s32 func_ov114_021c1dec(s32 *param_1);
void func_ov114_021c1ff8(void);
void func_ov114_021c204c(void);
void func_ov114_021c2270(void);
void func_ov114_021c22ec(s32 param_1, s32 param_2);
u32 func_ov114_021c2344(u32 *param_1, s32 param_2);
// pokebw2's ghttpCleanupRootCAList
u32 func_ov114_021c2350(const char *url);
void func_ov114_021c235c(void *element);
GHIPost *func_ov114_021c23e4(void);
void func_ov114_021c245c(GHIPost *post, BOOL autoFree);
void func_ov114_021c2464(GHIPost *post);
void func_ov114_021c2490(int request, GHIPost *post);
BOOL func_ov114_021c2584(GHIPost *post, SoapWriter *writer);
const char *func_ov114_021c25c8(GHIConnection *connection);
int func_ov114_021c2620(GHIConnection *connection);
int func_ov114_021c26b8(GHIConnection *connection);
int func_ov114_021c297c(GHIConnection *connection);
BOOL func_ov114_021c29ac(GHIPostState *state);
void func_ov114_021c2a3c(GHIPostState *state);
BOOL func_ov114_021c2a78(GHIConnection *connection);
void func_ov114_021c2bb0(GHIConnection *connection);
BOOL func_ov114_021c2c28(GHIPostState *state, GHIConnection *connection);
BOOL func_ov114_021c2db0(GHIPostState *state, GHIConnection *connection);
int func_ov114_021c2f10(GHIPostState *state, GHIConnection *connection);
int func_ov114_021c304c(GHIPostState *state, GHIConnection *connection);
int func_ov114_021c3218(GHIPostState *state, GHIConnection *connection, BOOL first, BOOL last);
int func_ov114_021c3834(GHIConnection *connection);
BOOL func_ov114_021c3a38(GHIConnection *connection);
void func_ov114_021c3bac(GHIConnection *connection);
void func_ov114_021c3d10(GHIConnection *connection);
void func_ov114_021c3e10(s32 *param_1);
void func_ov114_021c3ec0(GHIConnection *connection);
void func_ov114_021c4150(GHIConnection *connection);
void func_ov114_021c4384(GHIConnection *connection);
void func_ov114_021c4684(s32 *arg0);
void func_ov114_021c4764(s32 *param_1);
BOOL func_ov114_021c4830(GHIConnection *connection);
void func_ov114_021c492c(GHIConnection *connection);
BOOL func_ov114_021c4b18(GHIConnection *connection, const char *data, int size);
int func_ov114_021c4bd8(GHIConnection *connection);
void func_ov114_021c4c00(GHIConnection *connection, const char *data, int size);
BOOL func_ov114_021c4c50(GHIConnection *connection, const char *data, int size);
void func_ov114_021c4e68(void *pointer);
void func_ov114_021c4e7c(GHIConnection *connection);
void func_ov114_021c5430(GHIConnection *connection);

#endif

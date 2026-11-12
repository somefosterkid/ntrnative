// Black's DWC HTTP client (libdwcauth.a's dwc_http.o): the request's parameters, the handle's fields the callers read,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_HTTP_H
#define BWPORT_DWC_HTTP_H

#include "types.h"
#include "nitro/os.h"
#include "nitrowifi/cps.h"

// The allocator the library is given: the first argument is unused (0)
typedef void *(*DWCHttpAllocFunc)(int name, u32 size);
typedef void (*DWCHttpFreeFunc)(int name, void *ptr, int size);

// What DWC_Http_Create copies into the handle at 0x1004: the url, the buffer size, the allocator and a timeout
typedef struct DWCHttpParam {
    const char *url;
    int unk_04;
    s32 bufSize;
    DWCHttpAllocFunc alloc;
    DWCHttpFreeFunc free;
    int unk_14;
    s32 timeout;
} DWCHttpParam;

#define DWCHTTP_MAXURL 256
#define DWCHTTP_STACKSIZE 4096
#define DWCHTTP_MAX_LABELVALUE 32

// The handle's error (NitroDWC's DWCHttpError)
typedef enum {
    DWCHTTP_E_NOERR,
    DWCHTTP_E_MEMERR,
    DWCHTTP_E_DNSERR,
    DWCHTTP_E_CONERR,
    DWCHTTP_E_SENDTOUT,
    DWCHTTP_E_SENDERR,
    DWCHTTP_E_RECVTOUT,
    DWCHTTP_E_ABORT,
    DWCHTTP_E_FINISH,
    DWCHTTP_E_MAX
} DWCHttpError;

// A growing buffer: where it starts, where the next byte goes, its end and its size
typedef struct DWCHttpBuffer {
    char *buffer;
    char *write_index;
    char *buffer_tail;
    unsigned long length;
} DWCHttpBuffer;

typedef struct DWCHttpLabelValue {
    char *label;
    char *value;
} DWCHttpLabelValue;

typedef struct DWCHttpParseResult {
    DWCHttpLabelValue *entry;
    int len;
    int index;
} DWCHttpParseResult;

// The handle (NitroDWC's include/auth/dwc_http.h, whose offsets match Black's)
typedef struct DWCHttp {
    unsigned char stack[DWCHTTP_STACKSIZE];
    u8 initflag;
    DWCHttpParam param;
    DWCHttpError error;
    char url[DWCHTTP_MAXURL];
    char *hostname;
    char *filepath;
    u32 hostip;
    BOOL ssl_enabled;
    unsigned short port;
    CPSSoc soc;
    CPSSslConnection con;
    unsigned char *lowrecvbuf;
    unsigned char *lowsendbuf;
    u32 lowentropydata[OS_LOW_ENTROPY_DATA_SIZE / sizeof(u32)];
    int num_postitem;
    DWCHttpBuffer req;
    DWCHttpBuffer rep;
    OSMutex content_len_mutex;
    int content_len;
    int receivedbody_len;
    DWCHttpLabelValue labelvalue[DWCHTTP_MAX_LABELVALUE];
    OSThread thread;
    OSMutex mutex;
    BOOL abort;
} DWCHttp;

int DWC_Http_Create(DWCHttp *http, DWCHttpParam *param);
void DWC_Http_Destroy(void *http);
int DWC_Http_Add_HeaderItem(DWCHttp *http, const char *label, const char *data);
int DWC_Http_FinishHeader(DWCHttp *http);
void DWC_Http_StartThread(DWCHttp *http, u32 priority);
void DWC_Http_Abort(void *http);
int DWC_Http_ParseResult(DWCHttp *http, int flag);
char *DWC_Http_GetResult(DWCHttp *http, const char *key);
int DWC_Http_Add_PostBase64Item(DWCHttp *http, const char *key, const char *value, int len);
int DWC_Http_Add_Body(DWCHttp *http, const char *body);
BOOL DWC_Http_GetRawResult(DWCHttp *http, const char *key, char *buf, int size);
int DWC_Http_GetBase64DecodedResult(DWCHttp *http, const char *key, char *buf, int size);

// The strings the callers pass, which Black's linker put in dwc_http.o's data
extern const char data_ov009_021a2700[];
extern const char data_ov009_021a270c[];
extern const char data_ov009_021a272c[];
extern const char data_ov009_021a2734[];
extern const char data_ov009_021a273c[];
extern const char data_ov009_021a274c[];
extern const char data_ov009_021a2754[];
extern const char data_ov009_021a275c[];
extern const char data_ov009_021a2768[];
extern const char data_ov009_021a276c[];
extern const char data_ov009_021a2774[];
// dwc_prof.o's: the keys of the bad-word flags of each region ("prwordsJ" .. "prwordsC", three unused) and its strings
typedef struct DWCProfRegionKeys {
    const char *key[8];
} DWCProfRegionKeys;
extern const DWCProfRegionKeys data_ov009_021a27b8;
extern const char data_ov009_021a27d8[];
extern const char data_ov009_021a27e0[];
extern const char data_ov009_021a27e4[];
extern const char data_ov009_021a27ec[];
extern const char data_ov009_021a27f4[];
extern const char data_ov009_021a2800[];
extern const char data_ov009_021a2808[];
extern const char data_ov009_021a2810[];
extern const char data_ov009_021a2818[];
extern const char data_ov009_021a2820[];
extern const char data_ov009_021a282c[];
extern const char data_ov009_021a2838[];

#endif // BWPORT_DWC_HTTP_H

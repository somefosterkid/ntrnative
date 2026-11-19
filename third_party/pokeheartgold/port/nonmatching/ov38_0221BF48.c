#include "global.h"
enum HttpRequestStatus {
    HTTP_STATUS_NONE,
    HTTP_STATUS_IDLE,
    HTTP_STATUS_INITIALIZED,
    HTTP_STATUS_TOKEN_READY,
    HTTP_STATUS_TOKEN_PENDING,
    HTTP_STATUS_REQUEST_READY,
    HTTP_STATUS_REQUEST_PENDING,
    HTTP_STATUS_COMPLETE,
};
typedef enum {
    GHTTPSuccess,
    GHTTPOutOfMemory,
    GHTTPBufferOverflow,
    GHTTPParseURLFailed,
    GHTTPHostLookupFailed,
    GHTTPSocketFailed,
    GHTTPConnectFailed,
    GHTTPBadResponse,
    GHTTPRequestRejected,
    GHTTPUnauthorized,
    GHTTPForbidden,
    GHTTPFileNotFound,
    GHTTPServerError,
    GHTTPFileWriteFailed,
    GHTTPFileReadFailed,
    GHTTPFileIncomplete,
    GHTTPFileToBig,
    GHTTPEncryptionError
} GHTTPResult;
typedef enum {

    DWCGHTTPSuccess                  = GHTTPSuccess,
    DWCGHTTPOutOfMemory              = GHTTPOutOfMemory,
    DWCGHTTPBufferOverflow           = GHTTPBufferOverflow,
    DWCGHTTPParseURLFailed           = GHTTPParseURLFailed,
    DWCGHTTPHostLookupFailed         = GHTTPHostLookupFailed,
    DWCGHTTPSocketFailed             = GHTTPSocketFailed,
    DWCGHTTPConnectFailed            = GHTTPConnectFailed,
    DWCGHTTPBadResponse              = GHTTPBadResponse,
    DWCGHTTPRequestRejected          = GHTTPRequestRejected,
    DWCGHTTPUnauthorized             = GHTTPUnauthorized,
    DWCGHTTPForbidden                = GHTTPForbidden,
    DWCGHTTPFileNotFound             = GHTTPFileNotFound,
    DWCGHTTPServerError              = GHTTPServerError,
    DWCGHTTPFileWriteFailed          = GHTTPFileWriteFailed,
    DWCGHTTPFileReadFailed           = GHTTPFileReadFailed,
    DWCGHTTPFileIncomplete           = GHTTPFileIncomplete,
    DWCGHTTPFileToBig                = GHTTPFileToBig,
    DWCGHTTPEncryptionError          = GHTTPEncryptionError,
    DWCGHTTPNum,

    DWCGHTTPMemoryError              = DWCGHTTPNum + 1,

    DWC_GHTTP_SUCCESS                = GHTTPSuccess,
    DWC_GHTTP_OUT_OF_MEMORY          = GHTTPOutOfMemory,
    DWC_GHTTP_BUFFER_OVERFLOW        = GHTTPBufferOverflow,
    DWC_GHTTP_PARSE_URL_FAILED       = GHTTPParseURLFailed,
    DWC_GHTTP_HOST_LOOKUP_FAILED     = GHTTPHostLookupFailed,
    DWC_GHTTP_SOCKET_FAILED          = GHTTPSocketFailed,
    DWC_GHTTP_CONNECT_FAILED         = GHTTPConnectFailed,
    DWC_GHTTP_BAD_RESPONSE           = GHTTPBadResponse,
    DWC_GHTTP_REQUEST_REJECTED       = GHTTPRequestRejected,
    DWC_GHTTP_UNAUTHORIZED           = GHTTPUnauthorized,
    DWC_GHTTP_FORBIDDEN              = GHTTPForbidden,
    DWC_GHTTP_FILE_NOT_FOUND         = GHTTPFileNotFound,
    DWC_GHTTP_SERVER_ERROR           = GHTTPServerError,
    DWC_GHTTP_FILE_WRITE_FAILED      = GHTTPFileWriteFailed,
    DWC_GHTTP_FILE_READ_FAILED       = GHTTPFileReadFailed,
    DWC_GHTTP_FILE_INCOMPLETE        = GHTTPFileIncomplete,
    DWC_GHTTP_FILE_TOO_BIG           = GHTTPFileToBig,
    DWC_GHTTP_ENCRYPTION_ERROR       = GHTTPEncryptionError,
    DWC_GHTTP_NUM,

    DWC_GHTTP_MEMORY_ERROR           = DWC_GHTTP_NUM + 1
} DWCGHTTPResult;
typedef void (*DWCGHTTPCompletedCallback)(const char * buf, int len, DWCGHTTPResult result, void * param);
BOOL DWC_ProcessGHTTP(void);
int DWC_GetGHTTPData(const char * url, DWCGHTTPCompletedCallback completedCallback, void * param);
static struct {
    int readyState;
    int requestHandle;
    int errorCode;
    int responseLength;
    int dwcProfileID;
    void *data;
    int dataSize;
    void *response;
    int maxResponseLength;
    char *url;
    char *hashStr;
    char *b64DataStr;
    int encodedDataSize;
} sHttpData;
void ov38_0221BB68(int requestHandle);
void ov38_0221BBD0(const char *responseText, int length, DWCGHTTPResult result, void *unused);

int ov38_0221BF48(void);
int ov38_0221BF48(void)
{
    BOOL v0;

    switch (sHttpData.readyState) {
    case HTTP_STATUS_NONE:
        break;
    case HTTP_STATUS_IDLE:
        break;
    case HTTP_STATUS_INITIALIZED:
        break;
    case HTTP_STATUS_TOKEN_READY:
        sHttpData.requestHandle = DWC_GetGHTTPData(sHttpData.url, ov38_0221BBD0, &sHttpData);
        ov38_0221BB68(sHttpData.requestHandle);

        if (sHttpData.requestHandle >= 0) {
            sHttpData.readyState = 4;
        } else {
            sHttpData.readyState = 1;
        }
        break;
    case HTTP_STATUS_TOKEN_PENDING:
        v0 = DWC_ProcessGHTTP();

        if (!v0) {
            sHttpData.readyState = 1;
            break;
        }
        break;
    case HTTP_STATUS_REQUEST_READY:
        sHttpData.requestHandle = DWC_GetGHTTPData(sHttpData.url, ov38_0221BBD0, &sHttpData);
        ov38_0221BB68(sHttpData.requestHandle);

        if (sHttpData.requestHandle >= 0) {
            sHttpData.readyState = 6;
        } else {
            sHttpData.readyState = 1;
        }
        break;
    case HTTP_STATUS_REQUEST_PENDING:
        v0 = DWC_ProcessGHTTP();

        if (!v0) {
            sHttpData.readyState = 1;
            break;
        }
        break;
    case HTTP_STATUS_COMPLETE:
        break;
    }

    return sHttpData.readyState;
}

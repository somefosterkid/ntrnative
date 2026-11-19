#include "global.h"

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

    DWCGHTTPSuccess = GHTTPSuccess,
    DWCGHTTPOutOfMemory = GHTTPOutOfMemory,
    DWCGHTTPBufferOverflow = GHTTPBufferOverflow,
    DWCGHTTPParseURLFailed = GHTTPParseURLFailed,
    DWCGHTTPHostLookupFailed = GHTTPHostLookupFailed,
    DWCGHTTPSocketFailed = GHTTPSocketFailed,
    DWCGHTTPConnectFailed = GHTTPConnectFailed,
    DWCGHTTPBadResponse = GHTTPBadResponse,
    DWCGHTTPRequestRejected = GHTTPRequestRejected,
    DWCGHTTPUnauthorized = GHTTPUnauthorized,
    DWCGHTTPForbidden = GHTTPForbidden,
    DWCGHTTPFileNotFound = GHTTPFileNotFound,
    DWCGHTTPServerError = GHTTPServerError,
    DWCGHTTPFileWriteFailed = GHTTPFileWriteFailed,
    DWCGHTTPFileReadFailed = GHTTPFileReadFailed,
    DWCGHTTPFileIncomplete = GHTTPFileIncomplete,
    DWCGHTTPFileToBig = GHTTPFileToBig,
    DWCGHTTPEncryptionError = GHTTPEncryptionError,
    DWCGHTTPNum,

    DWCGHTTPMemoryError = DWCGHTTPNum + 1,

    DWC_GHTTP_SUCCESS = GHTTPSuccess,
    DWC_GHTTP_OUT_OF_MEMORY = GHTTPOutOfMemory,
    DWC_GHTTP_BUFFER_OVERFLOW = GHTTPBufferOverflow,
    DWC_GHTTP_PARSE_URL_FAILED = GHTTPParseURLFailed,
    DWC_GHTTP_HOST_LOOKUP_FAILED = GHTTPHostLookupFailed,
    DWC_GHTTP_SOCKET_FAILED = GHTTPSocketFailed,
    DWC_GHTTP_CONNECT_FAILED = GHTTPConnectFailed,
    DWC_GHTTP_BAD_RESPONSE = GHTTPBadResponse,
    DWC_GHTTP_REQUEST_REJECTED = GHTTPRequestRejected,
    DWC_GHTTP_UNAUTHORIZED = GHTTPUnauthorized,
    DWC_GHTTP_FORBIDDEN = GHTTPForbidden,
    DWC_GHTTP_FILE_NOT_FOUND = GHTTPFileNotFound,
    DWC_GHTTP_SERVER_ERROR = GHTTPServerError,
    DWC_GHTTP_FILE_WRITE_FAILED = GHTTPFileWriteFailed,
    DWC_GHTTP_FILE_READ_FAILED = GHTTPFileReadFailed,
    DWC_GHTTP_FILE_INCOMPLETE = GHTTPFileIncomplete,
    DWC_GHTTP_FILE_TOO_BIG = GHTTPFileToBig,
    DWC_GHTTP_ENCRYPTION_ERROR = GHTTPEncryptionError,
    DWC_GHTTP_NUM,

    DWC_GHTTP_MEMORY_ERROR = DWC_GHTTP_NUM + 1
} DWCGHTTPResult;

enum HTTPB64EncodeResult {
    HTTP_B64_ENCODE_SUCCESS = 0,
    HTTP_B64_ENCODE_ERROR_ALLOC_FAIL,
    HTTP_B64_ENCORE_ERROR_INSUFFICIENT_BUFFER_SIZE,
};

enum HTTPB64EncodeResult ov38_0221BA38(u32 dwcProfileID, const u8 *in, int length, u8 *out, int outSize);

extern struct {
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
} _02225040;

void ov38_0221BBD0(const char *responseText, int length, DWCGHTTPResult result, void *unused);

extern const char ov38_0222508C[];

extern const char ov38_02225094[];

extern const char ov38_0222509C[];

extern const char ov38_022250B4[];

extern const char ov38_022250CC[];

extern const char ov38_022250E4[];

extern const char ov38_022250FC[];

extern const char ov38_02225114[];

void ov38_0221BBD0(const char *responseText, int length, DWCGHTTPResult result, void *unused) {
    _02225040.requestHandle = -1;

    if (_02225040.readyState == 1) {
        return;
    }

    if (result == DWC_GHTTP_SUCCESS) {
        switch (_02225040.readyState) {
        case 4:

            if (length == 32) {

                u8 *responseHash = (u8 *)(_02225040.hashStr + 20);
                char saltedResponseText[(20 + 32 + 1)];
                const char hexDigits[] = "0123456789abcdef";

                strcpy(saltedResponseText, "sAdeqWo3voLeC5r16DYv");
                strncat(saltedResponseText, responseText, (u32)length);

                MATH_CalcSHA1((u8 *)responseHash, (const u8 *)saltedResponseText, strlen(saltedResponseText));

                strcat(_02225040.url, ov38_0222508C);

                for (int i = 0; i < 20; i++) {
                    _02225040.hashStr[i * 2] = hexDigits[responseHash[i] >> 4];
                    _02225040.hashStr[i * 2 + 1] = hexDigits[responseHash[i] & 0xf];
                }

                _02225040.hashStr[20 * 2] = '\0';
                strcat(_02225040.url, ov38_02225094);
                enum HTTPB64EncodeResult b64EncodeResult = ov38_0221BA38((u32)_02225040.dwcProfileID, (u8 *)_02225040.data, _02225040.dataSize, (u8 *)_02225040.b64DataStr, _02225040.encodedDataSize);

                switch (b64EncodeResult) {
                case HTTP_B64_ENCODE_SUCCESS:
                    break;
                case HTTP_B64_ENCODE_ERROR_ALLOC_FAIL:
                case HTTP_B64_ENCORE_ERROR_INSUFFICIENT_BUFFER_SIZE:
                    _02225040.readyState = 1;
                    return;
                }

                _02225040.readyState = 5;
            } else {
                _02225040.readyState = 1;
            }
            break;
        case 6:
            if (length == 22) {
                if (strncmp(responseText, ov38_0222509C, 22) == 0) {
                    _02225040.readyState = 1;
                    _02225040.errorCode = 26;
                    break;
                } else if (strncmp(responseText, ov38_022250B4, 22) == 0) {
                    _02225040.readyState = 1;
                    _02225040.errorCode = 27;
                    break;
                } else if (strncmp(responseText, ov38_022250CC, 22) == 0) {
                    _02225040.readyState = 1;
                    _02225040.errorCode = 28;
                    break;
                } else if (strncmp(responseText, ov38_022250E4, 22) == 0) {
                    _02225040.readyState = 1;
                    _02225040.errorCode = 29;
                    break;
                } else if (strncmp(responseText, ov38_022250FC, 22) == 0) {
                    _02225040.readyState = 1;
                    _02225040.errorCode = 30;
                    break;
                } else if (strncmp(responseText, ov38_02225114, 22) == 0) {
                    _02225040.readyState = 1;
                    _02225040.errorCode = 31;
                    break;
                }
            }

            if (length <= _02225040.maxResponseLength) {
                memcpy(_02225040.response, responseText, (u32)length);
                _02225040.readyState = 7;
            } else {
                memcpy(_02225040.response, responseText, (u32)_02225040.maxResponseLength);

                _02225040.readyState = 1;
                _02225040.errorCode = 32;
            }

            _02225040.responseLength = length;
            break;
        }
    } else {
        _02225040.readyState = 1;

        switch (result) {
        case DWC_GHTTP_OUT_OF_MEMORY:
            _02225040.errorCode = 8;
            break;
        case DWC_GHTTP_BUFFER_OVERFLOW:
            _02225040.errorCode = 9;
            break;
        case DWC_GHTTP_PARSE_URL_FAILED:
            _02225040.errorCode = 10;
            break;
        case DWC_GHTTP_HOST_LOOKUP_FAILED:
            _02225040.errorCode = 11;
            break;
        case DWC_GHTTP_SOCKET_FAILED:
            _02225040.errorCode = 12;
            break;
        case DWC_GHTTP_CONNECT_FAILED:
            _02225040.errorCode = 13;
            break;
        case DWC_GHTTP_BAD_RESPONSE:
            _02225040.errorCode = 14;
            break;
        case DWC_GHTTP_REQUEST_REJECTED:
            _02225040.errorCode = 15;
            break;
        case DWC_GHTTP_UNAUTHORIZED:
            _02225040.errorCode = 16;
            break;
        case DWC_GHTTP_FORBIDDEN:
            _02225040.errorCode = 17;
            break;
        case DWC_GHTTP_FILE_NOT_FOUND:
            _02225040.errorCode = 18;
            break;
        case DWC_GHTTP_SERVER_ERROR:
            _02225040.errorCode = 19;
            break;
        case DWC_GHTTP_FILE_INCOMPLETE:
            _02225040.errorCode = 22;
            break;
        case DWC_GHTTP_FILE_TOO_BIG:
            _02225040.errorCode = 23;
            break;
        case DWC_GHTTP_MEMORY_ERROR:
            _02225040.errorCode = 25;
            break;
        }
    }
}

#include "global.h"

int ov38_0221BEA8(const u8 *url, int dwcProfileID, const void *data, int dataSize, u8 *response, int maxResponseLength);

enum HttpPrepareRequestStatus {
    HTTP_PREPARE_STATUS_SUCCESS,
    HTTP_PREPARE_STATUS_NOT_READY,
    HTTP_PREPARE_STATUS_ALLOC_FAILED,
};

enum BattleTowerHttpRequestState {
    HTTP_STATE_INACTIVE,
    HTTP_STATE_IDLE,
    HTTP_STATE_AWAITING_ROOMNUM,
    HTTP_STATE_ERROR_ROOMNUM,
    HTTP_STATE_AWAITING_DOWNLOAD,
    HTTP_STATE_ERROR_DOWNLOAD,
    HTTP_STATE_AWAITING_UPLOAD,
    HTTP_STATE_ERROR_UPLOAD,
    HTTP_STATE_AWAITING_INFO,
    HTTP_STATE_ERROR_INFO,
    HTTP_STATE_AWAITING_SET_PROFILE,
    HTTP_STATE_ERROR_SET_PROFILE,
    HTTP_STATE_COMPLETE,
};

typedef struct BattleTowerHttpState {
    enum BattleTowerHttpRequestState requestState;
    s32 result;
    s32 profileId;
    u64 friendKey;
    u8 requestBody[239];
    u8 responseBuffer[2];
    u8 *responsePtr;
} BattleTowerHttpState;

extern BattleTowerHttpState _0223B820;

BOOL ov72_02237D50(const u8 *url, const void *data, int dataSize, void *response, int maxResponseLength);

enum BattleTowerHttpResult {
    BT_CODE_SUCCESS,
    BT_CODE_UPLOAD_NEEDED,
    BT_CODE_RANK_SELECT_NEEDED,

    BT_CODE_UPLOAD_REJECTED = -1,
    BT_CODE_SERVER_MAINTENANCE = -2,
    BT_CODE_ALREADY_CHALLENGED = -3,
    BT_CODE_HTTP_ERROR = -4,
    BT_CODE_NETWORK_FAILURE = -5,
    BT_CODE_NOT_IN_DCF_PHASE = -6,
    BT_CODE_CONNECTION_FAILED = -7,
};
extern void DWC_ClearError(void);
s32 WCM_GetPhase(void);
enum DwcError {
    DWC_ERROR_GHTTP_IN_ERROR,
    DWC_ERROR_GHTTP_INVALID_POST,
    DWC_ERROR_GHTTP_INSUFFICIENT_MEMORY,
    DWC_ERROR_GHTTP_INVALID_FILE_NAME,
    DWC_ERROR_GHTTP_INVALID_BUFFER_SIZE,
    DWC_ERROR_GHTTP_INVALID_URL,
    DWC_ERROR_GHTTP_UNSPECIFIED_ERROR,
    DWC_ERROR_GHTTP_7,
    DWC_ERROR_GHTTP_OUT_OF_MEMORY,
    DWC_ERROR_GHTTP_BUFFER_OVERFLOW,
    DWC_ERROR_GHTTP_PARSE_URL_FAILED,
    DWC_ERROR_GHTTP_HOST_LOOKUP_FAILED,
    DWC_ERROR_GHTTP_SOCKET_FAILED,
    DWC_ERROR_GHTTP_CONNECT_FAILED,
    DWC_ERROR_GHTTP_BAD_RESPONSE,
    DWC_ERROR_GHTTP_REQUEST_REJECTED,
    DWC_ERROR_GHTTP_UNAUTHORIZED,
    DWC_ERROR_GHTTP_FORBIDDEN,
    DWC_ERROR_GHTTP_FILE_NOT_FOUND,
    DWC_ERROR_GHTTP_SERVER_ERROR,
    DWC_ERROR_GHTTP_20,
    DWC_ERROR_GHTTP_21,
    DWC_ERROR_GHTTP_FILE_INCOMPLETE,
    DWC_ERROR_GHTTP_FILE_TOO_BIG,
    DWC_ERROR_GHTTP_24,
    DWC_ERROR_GHTTP_MEMORY_ERROR,
    DWC_ERROR_GHTTP_26,
    DWC_ERROR_GHTTP_27,
    DWC_ERROR_GHTTP_28,
    DWC_ERROR_GHTTP_29,
    DWC_ERROR_GHTTP_30,
    DWC_ERROR_GHTTP_31,
    DWC_ERROR_GHTTP_32,
};
int ov72_02237D88(int errorCode);

BOOL ov72_02237D50(const u8 *url, const void *data, int dataSize, void *response, int maxResponseLength) {
    switch (ov38_0221BEA8(url, _0223B820.profileId, data, dataSize, (u8 *)response, maxResponseLength)) {
    case HTTP_PREPARE_STATUS_SUCCESS:
        return TRUE;
        break;
    case HTTP_PREPARE_STATUS_NOT_READY:
        break;
    case HTTP_PREPARE_STATUS_ALLOC_FAILED:
        break;
    }

    return FALSE;
}

int ov72_02237D88(int errorCode) {
    int result;

    switch (errorCode) {
    case DWC_ERROR_GHTTP_IN_ERROR:
    case DWC_ERROR_GHTTP_INVALID_POST:
    case DWC_ERROR_GHTTP_INVALID_FILE_NAME:
    case DWC_ERROR_GHTTP_INVALID_BUFFER_SIZE:
    case DWC_ERROR_GHTTP_INVALID_URL:
    case DWC_ERROR_GHTTP_UNSPECIFIED_ERROR:
    case DWC_ERROR_GHTTP_BUFFER_OVERFLOW:
    case DWC_ERROR_GHTTP_PARSE_URL_FAILED:
    case DWC_ERROR_GHTTP_FILE_TOO_BIG:
    case DWC_ERROR_GHTTP_FILE_INCOMPLETE:
    case DWC_ERROR_GHTTP_20:
    case DWC_ERROR_GHTTP_21:
    case DWC_ERROR_GHTTP_BAD_RESPONSE:
    case DWC_ERROR_GHTTP_32:
        result = BT_CODE_NETWORK_FAILURE;
        break;
    case DWC_ERROR_GHTTP_INSUFFICIENT_MEMORY:
    case DWC_ERROR_GHTTP_OUT_OF_MEMORY:
    case DWC_ERROR_GHTTP_MEMORY_ERROR:
        result = BT_CODE_NETWORK_FAILURE;
        break;
    case DWC_ERROR_GHTTP_HOST_LOOKUP_FAILED:
        result = BT_CODE_CONNECTION_FAILED;
        break;
    case DWC_ERROR_GHTTP_SOCKET_FAILED:
    case DWC_ERROR_GHTTP_CONNECT_FAILED:
        result = BT_CODE_CONNECTION_FAILED;
        break;
    case DWC_ERROR_GHTTP_UNAUTHORIZED:
    case DWC_ERROR_GHTTP_FORBIDDEN:
    case DWC_ERROR_GHTTP_FILE_NOT_FOUND:
    case DWC_ERROR_GHTTP_SERVER_ERROR:
    case DWC_ERROR_GHTTP_26:
    case DWC_ERROR_GHTTP_27:
    case DWC_ERROR_GHTTP_28:
    case DWC_ERROR_GHTTP_29:
    case DWC_ERROR_GHTTP_31:
        result = BT_CODE_SERVER_MAINTENANCE;
        break;
    case DWC_ERROR_GHTTP_REQUEST_REJECTED:
    case DWC_ERROR_GHTTP_30:
        result = BT_CODE_SERVER_MAINTENANCE;
        break;
    default:
        result = BT_CODE_NETWORK_FAILURE;
        break;
    }

    if (result != BT_CODE_NETWORK_FAILURE) {
        if (WCM_GetPhase() != 9) {
            result = BT_CODE_NOT_IN_DCF_PHASE;
        }
        DWC_ClearError();
    }

    return result;
}

#include "global.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_70_gtsapplication_state.h"
#include "unk_0202D230.h"
#include "overlay_73_wifi_battle_tower_app_state.h"


typedef u16 charcode_t;

enum BattleTowerUploadResponse {
    BT_UPLOAD_RESPONSE_SUCCESS = 1,
    BT_UPLOAD_RESPONSE_ALREADY_CHALLENGED = 2,
    BT_UPLOAD_RESPONSE_INVALID = 4,
    BT_UPLOAD_RESPONSE_MAINTENANCE = 5,
};

enum BattleTowerInfoResponse {
    BT_INFO_RESPONSE_OK = 1,
    BT_INFO_RESPONSE_UPLOAD_NEEDED = 3,
    BT_INFO_RESPONSE_RANK_SELECT = 4,
    BT_INFO_RESPONSE_MAINTENANCE = 5,
};

enum BattleTowerDownloadResponse {
    BT_DOWNLOAD_RESPONSE_ALREADY_CHALLENGED = 2,
    BT_DOWNLOAD_RESPONSE_MAINTENANCE = 5,
};

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

int ov38_0221BF48(void);

void ov38_0221BFEC(void);

int ov38_0221C014(void);

int ov38_0221C020(void);

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

int ov72_02237D88(int errorCode);

extern BattleTowerHttpState _0223B820;

void ov72_022378DC(void);

BOOL ov72_02237B54(void);
s32 ov72_02237B74(void);
void ov38_0221BE84(void);
BOOL ov72_02237D50(const u8 *url, const void *data, int dataSize, void *response, int maxResponseLength);
void ov72_02237B80(s32 rank);
extern const u8 _0223B4E0[];
extern u8 ov72_0223B834[];
extern u8 ov72_0223B923[];

void ov72_022378DC(void) {
    switch (_0223B820.requestState) {
    case HTTP_STATE_INACTIVE:
        break;
    case HTTP_STATE_IDLE:
        break;
    case HTTP_STATE_AWAITING_ROOMNUM:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;
            _0223B820.result = ov72_02237D88(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;

            if (ov38_0221C020() > 0) {
                _0223B820.result = _0223B820.responseBuffer[0];
            } else {
                _0223B820.result = BT_CODE_SERVER_MAINTENANCE;
            }

            ov38_0221BFEC();
            break;
        }
        break;
    case HTTP_STATE_AWAITING_DOWNLOAD:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;
            _0223B820.result = ov72_02237D88(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;

            if (ov38_0221C020() == sizeof(WifiBattleTowerDownloadBuffer)) {
                _0223B820.result = BT_CODE_SUCCESS;
            } else {
                switch (_0223B820.responsePtr[0]) {
                case BT_DOWNLOAD_RESPONSE_ALREADY_CHALLENGED:
                    _0223B820.result = BT_CODE_ALREADY_CHALLENGED;
                    break;
                case BT_DOWNLOAD_RESPONSE_MAINTENANCE:
                    _0223B820.result = BT_CODE_SERVER_MAINTENANCE;
                    break;
                default:
                    _0223B820.result = BT_CODE_NETWORK_FAILURE;
                    break;
                }
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case HTTP_STATE_AWAITING_UPLOAD:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;
            _0223B820.result = ov72_02237D88(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;

            switch (_0223B820.responseBuffer[0]) {
            case BT_UPLOAD_RESPONSE_SUCCESS:
                _0223B820.result = BT_CODE_SUCCESS;
                break;
            case BT_UPLOAD_RESPONSE_ALREADY_CHALLENGED:
                _0223B820.result = BT_CODE_ALREADY_CHALLENGED;
                break;
            case BT_UPLOAD_RESPONSE_INVALID:
                _0223B820.result = BT_CODE_UPLOAD_REJECTED;
                break;
            case BT_UPLOAD_RESPONSE_MAINTENANCE:
                _0223B820.result = BT_CODE_SERVER_MAINTENANCE;
                break;
            default:
                _0223B820.result = BT_CODE_NETWORK_FAILURE;
                break;
            }

            ov38_0221BFEC();
            break;
        }
        break;
    case HTTP_STATE_AWAITING_INFO:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;
            _0223B820.result = ov72_02237D88(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;

            switch (_0223B820.responseBuffer[0]) {
            case BT_INFO_RESPONSE_OK:
                _0223B820.result = BT_CODE_SUCCESS;
                break;
            case BT_INFO_RESPONSE_UPLOAD_NEEDED:
                _0223B820.result = BT_CODE_UPLOAD_NEEDED;
                break;
            case BT_INFO_RESPONSE_RANK_SELECT:
                _0223B820.result = BT_CODE_RANK_SELECT_NEEDED;
                break;
            case BT_INFO_RESPONSE_MAINTENANCE:
                _0223B820.result = BT_CODE_SERVER_MAINTENANCE;
                break;
            default:
                _0223B820.result = BT_CODE_NETWORK_FAILURE;
                break;
            }

            ov38_0221BFEC();
            break;
        }
        break;
    case HTTP_STATE_AWAITING_SET_PROFILE:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;
            _0223B820.result = ov72_02237D88(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _0223B820.requestState = HTTP_STATE_COMPLETE;

            if (ov38_0221C020() == sizeof(WorldExchangeTrainerError)) {
                _0223B820.result = BT_CODE_SUCCESS;
            } else {
                switch (_0223B820.responseBuffer[0]) {
                case BT_INFO_RESPONSE_OK:
                    _0223B820.result = BT_CODE_SUCCESS;
                    break;
                case BT_INFO_RESPONSE_UPLOAD_NEEDED:
                    _0223B820.result = BT_CODE_UPLOAD_NEEDED;
                    break;
                case BT_INFO_RESPONSE_RANK_SELECT:
                    _0223B820.result = BT_CODE_RANK_SELECT_NEEDED;
                    break;
                case BT_INFO_RESPONSE_MAINTENANCE:
                    _0223B820.result = BT_CODE_SERVER_MAINTENANCE;
                    break;
                default:
                    _0223B820.result = BT_CODE_NETWORK_FAILURE;
                    break;
                }
            }

            ov38_0221BFEC();
            break;
        }
        break;
    case HTTP_STATE_ERROR_ROOMNUM:
    case HTTP_STATE_ERROR_DOWNLOAD:
    case HTTP_STATE_ERROR_UPLOAD:
    case HTTP_STATE_ERROR_INFO:
    case HTTP_STATE_ERROR_SET_PROFILE:
        _0223B820.requestState = HTTP_STATE_COMPLETE;
        _0223B820.result = BT_CODE_HTTP_ERROR;
        ov38_0221BFEC();
        break;
    case HTTP_STATE_COMPLETE:
        break;
    }
}

BOOL ov72_02237B54(void) {
    switch (_0223B820.requestState) {
    case HTTP_STATE_IDLE:
        return TRUE;
    case HTTP_STATE_COMPLETE:
        _0223B820.requestState = HTTP_STATE_IDLE;
        return TRUE;
    default:
        return FALSE;
    }
}

s32 ov72_02237B74(void) {
    return _0223B820.result;
}

void ov72_02237B80(s32 rank) {
    ov38_0221BE84();

    _0223B820.requestBody[0] = (u8)(rank - 1);

    if (ov72_02237D50(_0223B4E0, ov72_0223B834, 1, ov72_0223B923, 2)) {
        _0223B820.requestState = HTTP_STATE_AWAITING_ROOMNUM;
    } else {
        _0223B820.requestState = HTTP_STATE_COMPLETE;
        _0223B820.result = BT_CODE_NETWORK_FAILURE;
        ov38_0221BFEC();
    }
}

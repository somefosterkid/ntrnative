#include "global.h"

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

void ov72_022378C0(s32 profileId, u64 friendKey);

extern BattleTowerHttpState _0223B820;

void ov72_022378C0(s32 profileId, u64 friendKey) {
    _0223B820.requestState = HTTP_STATE_IDLE;
    _0223B820.result = BT_CODE_SUCCESS;
    _0223B820.profileId = profileId;
    _0223B820.friendKey = friendKey;
}

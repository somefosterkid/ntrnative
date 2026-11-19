#include "global.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleTowerHttpState;

struct FrontierPokemon;

struct WifiPlayerProfile;


typedef u16 charcode_t;

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

void ov38_0221BE84(void);

void ov38_0221BFEC(void);

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

BOOL ov72_02237D50(const u8 *url, const void *data, int dataSize, void *response, int maxResponseLength);

extern BattleTowerHttpState _0223B820;

void ov72_02237C30(s32 rank, s32 opponentIdx, s32 ratingTier, const WifiPlayerProfile *profile);

void ov72_02237C30(s32 rank, s32 opponentIdx, s32 ratingTier, const WifiPlayerProfile *profile) {
    ov38_0221BE84();

    memcpy(&_0223B820.requestBody[0], profile, sizeof(WifiPlayerProfile));

    _0223B820.requestBody[sizeof(WifiPlayerProfile)] = (u8)(rank - 1);
    _0223B820.requestBody[sizeof(WifiPlayerProfile) + 1] = (u8)(opponentIdx - 1);
    _0223B820.requestBody[sizeof(WifiPlayerProfile) + 2] = (u8)ratingTier;

    memcpy(&_0223B820.requestBody[sizeof(WifiPlayerProfile) + 3], &_0223B820.friendKey, sizeof(_0223B820.friendKey));

    if (ov72_02237D50((const u8 *)("http://gamestats2.gs.nintendowifi.net/pokemondpds/"
                                   "battletower/upload.asp"),
            _0223B820.requestBody,
            sizeof(WifiPlayerProfile) + 3 + sizeof(_0223B820.friendKey),
            _0223B820.responseBuffer,
            2)) {
        _0223B820.requestState = HTTP_STATE_AWAITING_UPLOAD;
    } else {
        _0223B820.requestState = HTTP_STATE_COMPLETE;
        _0223B820.result = BT_CODE_NETWORK_FAILURE;
        ov38_0221BFEC();
    }
}

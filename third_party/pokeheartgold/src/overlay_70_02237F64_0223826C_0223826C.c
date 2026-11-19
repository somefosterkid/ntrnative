#include "global.h"
#include "overlay_70_gts_networking_state.h"

struct GTSNetworkingState;

enum GTSNetworkResult {
    GTS_RESULT_SUCCESS = 0,
    GTS_RESULT_PROFILE_EXISTS = 1,
    GTS_RESULT_BANNED = 2,
    GTS_RESULT_1_FOUND = 1,
    GTS_RESULT_2_FOUND = 2,
    GTS_RESULT_3_FOUND = 3,
    GTS_RESULT_4_FOUND = 4,
    GTS_RESULT_5_FOUND = 5,
    GTS_RESULT_6_FOUND = 6,
    GTS_RESULT_7_FOUND = 7,

    GTS_RESULT_GTS_CROWDED = -1,
    GTS_RESULT_SERVER_MAINTENANCE = -2,
    GTS_RESULT_ALREADY_TRADED = -3,
    GTS_RESULT_NOT_FOUND = -4,
    GTS_RESULT_ALREADY_DEPOSITED = -5,
    GTS_RESULT_CANNOT_BE_OFFERED_1 = -6,
    GTS_RESULT_CANNOT_BE_OFFERED_2 = -7,
    GTS_RESULT_CANNOT_BE_OFFERED_3 = -8,
    GTS_RESULT_CANNOT_BE_OFFERED_4 = -9,
    GTS_RESULT_CANNOT_BE_OFFERED_5 = -10,
    GTS_RESULT_CANNOT_BE_OFFERED_6 = -11,
    GTS_RESULT_ABORTED = -12,
    GTS_RESULT_ERROR = -13,
    GTS_RESULT_NOT_IN_DCF_PHASE = -14,
    GTS_RESULT_CONNECTION_FAILED = -15,
};

void ov38_0221BE84(void);

void ov38_0221BFEC(void);

BOOL ov70_02238360(const u8 *url, const void *data, int dataSize, void *response, int maxResponseLength);

extern GTSNetworkingState _02246800;

void ov70_0223826C(void);

void ov70_0223826C(void) {
    memcpy(&_02246800.requestData[0], &_02246800.friendKey, 8);

    ov38_0221BE84();

    if (ov70_02238360((const u8 *)"http://gamestats2.gs.nintendowifi.net/pokemondpds/"
                                  "worldexchange/exchange_finish.asp",
            _02246800.requestData,
            8,
            _02246800.responseStatus,
            2)) {
        _02246800.status = NETWORK_STATUS_EXCHANGE_FINISH_PENDING;
    } else {
        _02246800.status = NETWORK_STATUS_COMPLETE;
        _02246800.result = GTS_RESULT_ERROR;
        ov38_0221BFEC();
    }
}

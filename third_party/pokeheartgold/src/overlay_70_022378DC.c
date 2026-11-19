#include "global.h"

#include "overlay_70_gts_networking_state.h"
#include "overlay_70_gtsapplication_state.h"

enum GTSNetworkResponseStatus {
    GTS_SERVER_STATUS_NONE,
    GTS_SERVER_STATUS_SUCCESS,
    GTS_SERVER_STATUS_ALREADY_DEPOSITED,
    GTS_SERVER_STATUS_NOT_FOUND,
    GTS_SERVER_STATUS_NO_MATCH_YET,
    GTS_SERVER_STATUS_ALREADY_TRADED,
    GTS_SERVER_STATUS_PROFILE_EXISTS,
    GTS_SERVER_STATUS_CROWDED,
    GTS_SERVER_STATUS_CANNOT_BE_OFFERED_1,
    GTS_SERVER_STATUS_CANNOT_BE_OFFERED_2,
    GTS_SERVER_STATUS_CANNOT_BE_OFFERED_3,
    GTS_SERVER_STATUS_CANNOT_BE_OFFERED_4,
    GTS_SERVER_STATUS_CANNOT_BE_OFFERED_5,
    GTS_SERVER_STATUS_CANNOT_BE_OFFERED_6,
    GTS_SERVER_STATUS_MAINTENANCE,
};

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

int ov38_0221BF48(void);

void ov38_0221BFEC(void);

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

int ov70_02238398(int errCode);

int ov38_0221C014(void);


void ov70_022378DC(void);

void ov70_022378DC(void) {
    switch (_02246800.status) {
    case NETWORK_STATUS_INACTIVE:
        break;
    case NETWORK_STATUS_IDLE:
        break;
    case NETWORK_STATUS_POST_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            switch (_02246800.responseStatus[0]) {
            case GTS_SERVER_STATUS_SUCCESS:
                _02246800.result = GTS_RESULT_SUCCESS;
                break;
            case GTS_SERVER_STATUS_ALREADY_DEPOSITED:
                _02246800.result = GTS_RESULT_ALREADY_DEPOSITED;
                break;
            case GTS_SERVER_STATUS_NOT_FOUND:
                _02246800.result = GTS_RESULT_NOT_FOUND;
                break;
            case GTS_SERVER_STATUS_CROWDED:
                _02246800.result = GTS_RESULT_GTS_CROWDED;
                break;
            case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_5:
                _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_1;
                break;
            case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_6:
                _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_2;
                break;
            case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_1:
                _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_3;
                break;
            case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_2:
                _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_4;
                break;
            case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_3:
                _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_5;
                break;
            case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_4:
                _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_6;
                break;
            case GTS_SERVER_STATUS_MAINTENANCE:
                _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                break;
            default:
                _02246800.result = GTS_RESULT_ERROR;
                break;
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_POST_FINISH_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            switch (_02246800.responseStatus[0]) {
            case GTS_SERVER_STATUS_SUCCESS:
                _02246800.result = GTS_RESULT_SUCCESS;
                break;
            case GTS_SERVER_STATUS_ALREADY_DEPOSITED:
                _02246800.result = GTS_RESULT_ALREADY_DEPOSITED;
                break;
            case GTS_SERVER_STATUS_NOT_FOUND:
                _02246800.result = GTS_RESULT_NOT_FOUND;
                break;
            case GTS_SERVER_STATUS_ALREADY_TRADED:
                _02246800.result = GTS_RESULT_ALREADY_TRADED;
                break;
            case GTS_SERVER_STATUS_MAINTENANCE:
                _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                break;
            default:
                _02246800.result = GTS_RESULT_ERROR;
                break;
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_GET_LISTED_POKEMON_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            if (ov38_0221C020() == sizeof(GTSPokemonListing)) {
                _02246800.result = GTS_RESULT_SUCCESS;
            } else {
                switch (_02246800.responseData[0]) {
                case GTS_SERVER_STATUS_ALREADY_TRADED:
                    _02246800.result = GTS_RESULT_ALREADY_TRADED;
                    break;
                case GTS_SERVER_STATUS_NOT_FOUND:
                    _02246800.result = GTS_RESULT_NOT_FOUND;
                    break;
                case GTS_SERVER_STATUS_MAINTENANCE:
                    _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                    break;
                default:
                    _02246800.result = GTS_RESULT_ERROR;
                    break;
                }
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_GET_LISTING_STATUS_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            if (ov38_0221C020() == sizeof(GTSPokemonListing)) {
                _02246800.result = GTS_RESULT_PROFILE_EXISTS;
            } else {
                switch (_02246800.responseData[0]) {
                case GTS_SERVER_STATUS_ALREADY_TRADED:
                    _02246800.result = GTS_RESULT_ALREADY_TRADED;
                    break;
                case GTS_SERVER_STATUS_NOT_FOUND:
                    _02246800.result = GTS_RESULT_NOT_FOUND;
                    break;
                case GTS_SERVER_STATUS_NO_MATCH_YET:
                    _02246800.result = GTS_RESULT_SUCCESS;
                    break;
                case GTS_SERVER_STATUS_MAINTENANCE:
                    _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                    break;
                default:
                    _02246800.result = GTS_RESULT_ERROR;
                    break;
                }
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_DELETE_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            switch (_02246800.responseStatus[0]) {
            case GTS_SERVER_STATUS_SUCCESS:
                _02246800.result = GTS_RESULT_SUCCESS;
                break;
            case GTS_SERVER_STATUS_ALREADY_TRADED:
                _02246800.result = GTS_RESULT_ALREADY_TRADED;
                break;
            case GTS_SERVER_STATUS_NOT_FOUND:
                _02246800.result = GTS_RESULT_NOT_FOUND;
                break;
            case GTS_SERVER_STATUS_MAINTENANCE:
                _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                break;
            default:
                _02246800.result = GTS_RESULT_ERROR;
                break;
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_RETURN_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            switch (_02246800.responseStatus[0]) {
            case GTS_SERVER_STATUS_SUCCESS:
                _02246800.result = GTS_RESULT_SUCCESS;
                break;
            case GTS_SERVER_STATUS_ALREADY_TRADED:
                _02246800.result = GTS_RESULT_ALREADY_TRADED;
                break;
            case GTS_SERVER_STATUS_ALREADY_DEPOSITED:
                _02246800.result = GTS_RESULT_ALREADY_DEPOSITED;
                break;
            case GTS_SERVER_STATUS_NOT_FOUND:
                _02246800.result = GTS_RESULT_NOT_FOUND;
                break;
            case GTS_SERVER_STATUS_MAINTENANCE:
                _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                break;
            default:
                _02246800.result = GTS_RESULT_ERROR;
                break;
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_SEARCH_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            {
                int v0 = ov38_0221C020();

                if (v0 >= sizeof(GTSPokemonListing)) {
                    _02246800.result = (s32)(ov38_0221C020() / sizeof(GTSPokemonListing));
                } else if (v0 == 0) {
                    _02246800.result = GTS_RESULT_SUCCESS;
                } else {
                    switch (_02246800.responseData[0]) {
                    case GTS_SERVER_STATUS_MAINTENANCE:
                        _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                        break;
                    default:
                        _02246800.result = GTS_RESULT_ERROR;
                        break;
                    }
                }
            }
            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_EXCHANGE_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            if (ov38_0221C020() == sizeof(GTSPokemonListing)) {
                _02246800.result = GTS_RESULT_SUCCESS;
            } else {
                switch (_02246800.responseData[0]) {
                case GTS_SERVER_STATUS_ALREADY_DEPOSITED:
                    _02246800.result = GTS_RESULT_ALREADY_DEPOSITED;
                    break;
                case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_5:
                    _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_1;
                    break;
                case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_6:
                    _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_2;
                    break;
                case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_1:
                    _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_3;
                    break;
                case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_2:
                    _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_4;
                    break;
                case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_3:
                    _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_5;
                    break;
                case GTS_SERVER_STATUS_CANNOT_BE_OFFERED_4:
                    _02246800.result = GTS_RESULT_CANNOT_BE_OFFERED_6;
                    break;
                case GTS_SERVER_STATUS_MAINTENANCE:
                    _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                    break;
                default:
                    _02246800.result = GTS_RESULT_ERROR;
                    break;
                }
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_EXCHANGE_FINISH_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            switch (_02246800.responseStatus[0]) {
            case GTS_SERVER_STATUS_SUCCESS:
                _02246800.result = GTS_RESULT_SUCCESS;
                break;
            case GTS_SERVER_STATUS_MAINTENANCE:
                _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                break;
            case GTS_SERVER_STATUS_ALREADY_DEPOSITED:
                _02246800.result = GTS_RESULT_ALREADY_DEPOSITED;
                break;
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_INIT_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            switch (_02246800.responseStatus[0]) {
            case GTS_SERVER_STATUS_SUCCESS:
                _02246800.result = GTS_RESULT_SUCCESS;
                break;
            case GTS_SERVER_STATUS_PROFILE_EXISTS:
                _02246800.result = GTS_RESULT_PROFILE_EXISTS;
                break;
            case GTS_SERVER_STATUS_CROWDED:
                _02246800.result = GTS_RESULT_BANNED;
                break;
            case GTS_SERVER_STATUS_MAINTENANCE:
                _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                break;
            default:
                _02246800.result = GTS_RESULT_ERROR;
                break;
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_SET_PROFILE_PENDING:
        switch (ov38_0221BF48()) {
        case HTTP_STATUS_IDLE:
            _02246800.status = NETWORK_STATUS_COMPLETE;
            _02246800.result = ov70_02238398(ov38_0221C014());
            ov38_0221BFEC();
            break;
        case HTTP_STATUS_COMPLETE:
            _02246800.status = NETWORK_STATUS_COMPLETE;

            if (ov38_0221C020() == sizeof(WorldExchangeTrainerError)) {
                _02246800.result = GTS_RESULT_SUCCESS;
            } else {
                switch (_02246800.responseStatus[0]) {
                case GTS_SERVER_STATUS_SUCCESS:
                    _02246800.result = GTS_RESULT_SUCCESS;
                    break;
                case GTS_SERVER_STATUS_PROFILE_EXISTS:
                    _02246800.result = GTS_RESULT_PROFILE_EXISTS;
                    break;
                case GTS_SERVER_STATUS_CROWDED:
                    _02246800.result = GTS_RESULT_BANNED;
                    break;
                case GTS_SERVER_STATUS_MAINTENANCE:
                    _02246800.result = GTS_RESULT_SERVER_MAINTENANCE;
                    break;
                default:
                    _02246800.result = GTS_RESULT_ERROR;
                    break;
                }
            }

            ov38_0221BFEC();
            break;
        }

        break;
    case NETWORK_STATUS_POST_PENDING_ABORTED:
    case NETWORK_STATUS_POST_FINISH_PENDING_ABORTED:
    case NETWORK_STATUS_GET_LISTED_POKEMON_PENDING_ABORTED:
    case NETWORK_STATUS_GET_LISTING_STATUS_PENDING_ABORTED:
    case NETWORK_STATUS_DELETE_PENDING_ABORTED:
    case NETWORK_STATUS_RETURN_PENDING_ABORTED:
    case NETWORK_STATUS_SEARCH_PENDING_ABORTED:
    case NETWORK_STATUS_EXCHANGE_PENDING_ABORTED:
    case NETWORK_STATUS_EXCHANGE_FINISH_PENDING_ABORTED:
    case NETWORK_STATUS_INIT_PENDING_ABORTED:
    case NETWORK_STATUS_SET_PROFILE_PENDING_ABORTED:
        _02246800.status = NETWORK_STATUS_COMPLETE;
        _02246800.result = GTS_RESULT_ABORTED;
        ov38_0221BFEC();
        break;
    case NETWORK_STATUS_COMPLETE:
        break;
    }
}

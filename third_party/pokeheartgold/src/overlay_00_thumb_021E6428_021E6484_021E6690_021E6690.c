#include "global.h"

#include "unk_02037C94.h"
#include "overlay_00_nintendo_wfcmanager.h"

enum NintendoWFCResult {
    NINTENDO_WFC_RESULT_MATCHMAKING_SUCCESS = DWC_ERROR_NUM,
    NINTENDO_WFC_RESULT_CONN_RESET,
    NINTENDO_WFC_RESULT_CONN_RESET_AFTER_HOST_LEFT,
    NINTENDO_WFC_RESULT_RECEIVE_TIMED_OUT,
    NINTENDO_WFC_RESULT_CONNECTION_CLOSED,
    NINTENDO_WFC_RESULT_UNUSED_5,
    NINTENDO_WFC_RESULT_DUMMY_6,
    NINTENDO_WFC_RESULT_CONNECTED_TO_SERVER,
};

extern NintendoWFCManager *_0221A680;

int ov00_021E6690(void);

int ov00_021E6690(void) {

    DWCError errorCode;
    DWCErrorType errorType;
    DWCError error;
    int ret = DWC_ERROR_NONE;

    error = DWC_GetLastErrorEx((int *)&errorCode, &errorType);

    if (error != 0) {
        ret = errorCode;

        if (errorCode == 0 || errorType == DWC_ETYPE_LIGHT) {
            ret = error;
        }

        switch (errorType) {
        case DWC_ETYPE_LIGHT:
            DWC_ClearError();
            break;
        case DWC_ETYPE_SHOW_ERROR:
            DWC_ClearError();
            break;
        case DWC_ETYPE_SHUTDOWN_FM:
        case DWC_ETYPE_SHUTDOWN_GHTTP:
        case DWC_ETYPE_SHUTDOWN_ND:
        case DWC_ETYPE_DISCONNECT:
            if (_0221A680) {
                switch (_0221A680->state) {
                case WFC_STATE_WAIT_FOR_LOGIN:
                case WFC_STATE_READY:
                case WFC_STATE_WAIT_MATCHMAKING:
                case WFC_STATE_FORCE_CANCEL:
                case WFC_STATE_MATCHMAKING_SUCCESS:
                case WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS:
                case WFC_STATE_RESET_AFTER_DISCONNECT:
                case WFC_STATE_CONNECTION_ERROR:
                case WFC_STATE_RECEIVE_TIMED_OUT:
                case WFC_STATE_CLOSE_CONNECTION:
                case WFC_STATE_CONNECTION_CLOSED:
                    DWC_ShutdownFriendsMatch();
                case WFC_STATE_INIT:
                case WFC_STATE_WAIT_CONNECTION:
                case WFC_STATE_INIT_LOGIN:
                    if (sub_0203993C() != 33) {
                        DWC_CleanupInet();
                    }
                default:
                    break;
                }

                DWC_ClearError();
            }

            if (_0221A680) {
                _0221A680->state = WFC_STATE_ERROR_DISCONNECTED;
            }
            break;
        case DWC_ETYPE_FATAL:
            if (_0221A680) {
                _0221A680->state = WFC_STATE_FATAL_ERROR;

                if (_0221A680->fatalErrorCallback != 0L) {
                    _0221A680->fatalErrorCallback(-errorCode);
                }
            }
            break;
        }
    }

    if (_0221A680->dummy_10D2) {
        ret = NINTENDO_WFC_RESULT_DUMMY_6;
    }

    if (error != 0) {
        sub_02038F74(errorCode, errorType, error);
    }

    return ret;
}

BOOL ov00_021E6790(void);

BOOL ov00_021E6790(void) {
    int i; // forward declaration required to match
    BOOL ret = FALSE;

    for (i = 0; i < _0221A680->maxPlayers; i++) {
        if (DWC_GetMyAID() == i) {
            continue;
        }

        if (DWC_IsValidAID(i)) {
            ret = TRUE;

            if (!DWC_IsSendableReliable(i)) {
                return FALSE;
            }
        }
    }

    return ret;
}

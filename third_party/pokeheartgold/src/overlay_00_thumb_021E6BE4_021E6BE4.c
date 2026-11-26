#include "global.h"

#include "overlay_00_thumb.h"
#include "overlay_00_nintendo_wfcmanager.h"

void ov00_021E79CC(void);

extern NintendoWFCManager *_0221A680;

BOOL ov00_021E6BE4(BOOL isClient) {
    if (isClient == FALSE) {
        switch (_0221A680->state) {
        case WFC_STATE_WAIT_MATCHMAKING:
        case WFC_STATE_MATCHMAKING_SUCCESS:
        case WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS:
            if (_0221A680->voiceChatActive) {
                ov00_021E79CC();
            }

            _0221A680->state = WFC_STATE_CLOSE_CONNECTION;
            break;
        case WFC_STATE_READY:
        case WFC_STATE_ERROR_DISCONNECTED:
        case WFC_STATE_CONNECTION_CLOSED:
        case WFC_STATE_RECEIVE_TIMED_OUT:
            return TRUE;
        default:
            break;
        }
    } else {
        switch (_0221A680->state) {
        case WFC_STATE_READY:
        case WFC_STATE_CONNECTION_CLOSED:
        case WFC_STATE_RECEIVE_TIMED_OUT:
            return TRUE;
        default:
            break;
        }
    }

    return FALSE;
}

BOOL ov00_021E6C68(void) {
    if (_0221A680->state == WFC_STATE_CONNECTION_CLOSED
        || _0221A680->state == WFC_STATE_RECEIVE_TIMED_OUT
        || _0221A680->state == WFC_STATE_READY) {
        _0221A680->state = WFC_STATE_READY;
        _0221A680->latestNewClientFriendIdx = -1;
        ov00_021E7130();
        return TRUE;
    }

    return FALSE;
}

#include "global.h"

#include "overlay_00_thumb.h"
#include "overlay_00_nintendo_wfcmanager.h"

BOOL ov00_021E6790(void);

extern NintendoWFCManager *_0221A680;

static BOOL ov00_021E67E0(int timerIdx);

static BOOL ov00_021E67E0(int timerIdx) {
    if (_0221A680->dataSent == FALSE && ov00_021E6790() && ((u16)~1 & DWC_GetAIDBitmap())) {
        _0221A680->dataSent = TRUE;
        *(u32 *)_0221A680->sendBuffer = 0x2 | (_0221A680->inVoiceChatLobby << 8);

        DWC_SendReliableBitmap(DWC_GetAIDBitmap(), _0221A680->sendBuffer, 4);

        _0221A680->clientUpdateTimers[timerIdx] = 0;

        return TRUE;
    }

    return FALSE;
}

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

void ov00_021E765C(void);

void ov00_021E7AE0(void);

void ov00_021E7ACC(void);

BOOL ov00_021E7AF4(int newClientsBitmap, int currentClientID);

void ov00_021E6CE8(void);

void ov00_021E62D8(void);

int ov00_021E6850(void);

int ov00_021E6850(void) {
    DWC_ProcessFriendsMatch();
    ov00_021E6CE8();

    if (_0221A680->voiceChatActive) {
        if (_0221A680->inVoiceChatLobby == TRUE && _0221A680->otherPlayersEnabledVoiceChat == TRUE && _0221A680->voiceChatEnabled_Battles == TRUE) {
            ov00_021E7AE0();
        } else {
            ov00_021E7ACC();
        }

        ov00_021E765C();

        if (_0221A680->voiceConnectedIDs != DWC_GetAIDBitmap()) {
            if (!_0221A680->dummy_10D3 && _0221A680->voiceChatEnabled) {
                if (ov00_021E7AF4(DWC_GetAIDBitmap(), DWC_GetMyAID())) {
                    _0221A680->voiceConnectedIDs = DWC_GetAIDBitmap();
                }
            }
        }
    }

    if (_0221A680->state == WFC_STATE_RECEIVE_TIMED_OUT) {
        int error = ov00_021E6690();

        if (error != 0) {
            return error;
        }

        return NINTENDO_WFC_RESULT_RECEIVE_TIMED_OUT;
    }

    if (_0221A680->state == WFC_STATE_CONNECTION_CLOSED) {
        return NINTENDO_WFC_RESULT_CONNECTION_CLOSED;
    }

    if (_0221A680->state == WFC_STATE_MATCHMAKING_SUCCESS
        || _0221A680->state == WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS) {
        for (int i = 0; i < _0221A680->maxPlayers; i++) {
            if (_0221A680->clientUpdateTimers[i]++ >= 120 && _0221A680->dataSent == FALSE) {
                if (ov00_021E67E0(i)) {
                    ov00_021E62D8();
                    break;
                }
            }
        }
    }

    return ov00_021E6690();
}

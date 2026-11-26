#include "global.h"

#include "overlay_00_thumb.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E6554(DWCError error, BOOL selfClosed, BOOL hostClosed, u8 aid, int closingFriendIdx, void *userParam);

void ov00_021E6554(DWCError error, BOOL selfClosed, BOOL hostClosed, u8 aid, int closingFriendIdx, void *userParam) {
    _0221A680->dataSent = 0;
    _0221A680->timer = 0;
    _0221A680->unused_1094 = 0;

    if (error == DWC_ERROR_NONE) {
        if (_0221A680->disconnectIfAlone == TRUE && DWC_GetNumConnectionHost() == 1) {
            if (_0221A680->state != WFC_STATE_FORCE_CANCEL) {
                _0221A680->state = WFC_STATE_CLOSE_CONNECTION;
            }

            if (_0221A680->voiceChatActive) {
                ov00_021E6A4C();
            }
        }
    }

    if (_0221A680->connectionClosedCallback) {
        _0221A680->connectionClosedCallback(aid, _0221A680->connectionClosedCBArg);
    }
}

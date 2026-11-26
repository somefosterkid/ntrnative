#include "global.h"

#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E6240(u8 aid);

void ov00_021E6240(u8 aid) {
    if (_0221A680->enableTimeout != 0) {
        DWC_CloseAllConnectionsHard();
        _0221A680->latestNewClientFriendIdx = -1;
        _0221A680->state = WFC_STATE_RECEIVE_TIMED_OUT;
    }
    return;
}

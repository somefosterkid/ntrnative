#include "global.h"

#include "wireless_manager.h"

void sub_020339F0(void *wm_callback);

BOOL sub_02033A0C(BOOL enable);

BOOL sub_02033A0C(BOOL enable) {
    _021D4124.sWirelessManager->setEntry = 0;

    if (_021D4124.sWirelessManager->state == WIRELESS_STATE_CONNECTED) {
        if (WM_ERRCODE_OPERATING == WM_SetEntry(sub_020339F0, enable)) {
            return 1;
        }
    }

    return 0;
}

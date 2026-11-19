#include "global.h"

#include "wireless_manager.h"

void sub_0203301C(void *wm_startmpcallback);

void sub_0203301C(void *wm_startmpcallback) {
    WMstartMPCallback *callback = (WMstartMPCallback *)wm_startmpcallback;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        if (callback->errcode == WM_ERRCODE_SEND_FAILED) {
            return;
        } else if (callback->errcode == WM_ERRCODE_TIMEOUT) {
            return;
        } else if (callback->errcode == WM_ERRCODE_INVALID_POLLBITMAP) {
            return;
        }

        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);

        return;
    }

    switch (callback->state) {
    case WM_STATECODE_MP_START:
        if (_021D4124.sWirelessManager->connectionType == WIRELESS_CONNECTION_TRANSMIT_KEY_CLIENT) {
            if (_021D4124.sWirelessManager->state == WIRELESS_STATE_TRANSMIT_KEY) {
                return;
            }
        }

        sub_02032844(WIRELESS_STATE_CONNECTED);
        break;
    case WM_STATECODE_MP_IND:
        break;
    case WM_STATECODE_MPACK_IND:
        break;
    case WM_STATECODE_MPEND_IND:
    default:
        break;
    }
}

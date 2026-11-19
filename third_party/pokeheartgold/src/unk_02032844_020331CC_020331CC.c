#include "global.h"

#include "wireless_manager.h"

void sub_020331CC(void *wm_portrecvcallback);

void sub_020331CC(void *wm_portrecvcallback) {
    WMPortRecvCallback *callback = (WMPortRecvCallback *)wm_portrecvcallback;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
    } else if (_021D4124.sWirelessManager->recvFunc != 0L) {
        if (callback->state == WM_STATECODE_PORT_INIT) {
            ;
        } else if (callback->state == WM_STATECODE_PORT_RECV) {
            (*_021D4124.sWirelessManager->recvFunc)(callback->aid, callback->data, callback->length);
        } else if (callback->state == WM_STATECODE_DISCONNECTED) {
            (*_021D4124.sWirelessManager->recvFunc)(callback->aid, 0L, 0);
        }
    }
}

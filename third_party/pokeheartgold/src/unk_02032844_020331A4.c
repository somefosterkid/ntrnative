#include "global.h"

#include "wireless_manager.h"

void sub_020331A4(void *wm_portsendcallback);

void sub_020331A4(void *wm_portsendcallback) {
    WMPortSendCallback *callback = (WMPortSendCallback *)wm_portsendcallback;

    if ((callback->errcode != WM_ERRCODE_SUCCESS) && (callback->errcode != WM_ERRCODE_SEND_FAILED)) {
        sub_02032858(callback->errcode);
        return;
    }

    if (callback->arg != 0L) {
        WirelessManagerSendFunc sendFunc = (WirelessManagerSendFunc)callback->arg;

        (*sendFunc)(callback->errcode == WM_ERRCODE_SUCCESS);
    }
}

#include "global.h"

#include "wireless_manager.h"
#include "unk_02037C94.h"


void sub_02032F0C(void *wm_connectcallback);

BOOL sub_02032E9C(void);

extern const char _0210F8FC[];

BOOL sub_02032E9C(void) {
    u8 ssid[32];

    if ((_021D4124.sWirelessManager->state == WIRELESS_STATE_CONNECTED) || (_021D4124.sWirelessManager->state == WIRELESS_STATE_TRANSMIT_KEY) || (_021D4124.sWirelessManager->state == WIRELESS_STATE_TRANSMIT_DATA)) {
        return 1;
    }

    sub_02032844(WIRELESS_STATE_BUSY);
    MI_CpuCopy8(_0210F8FC, &ssid[1], sizeof("DP"));

    ssid[0] = sub_0203993C();
    WMErrCode errorCode = WM_StartConnectEx(sub_02032F0C, &_021D4124.sWirelessManager->bssDesc, ssid, 1, 0);

    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return 0;
    }

    return 1;
}

BOOL sub_02039918(void);

BOOL sub_02032FCC(void);

void sub_02032F0C(void *wm_connectcallback) {
    WMStartConnectCallback *callback = (WMStartConnectCallback *)wm_connectcallback;

    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);

        if (callback->errcode == WM_ERRCODE_OVER_MAX_ENTRY) {
            sub_02032844(WIRELESS_STATE_ERROR);
            return;
        } else if (callback->errcode == WM_ERRCODE_NO_ENTRY) {
            sub_02032844(WIRELESS_STATE_ERROR);
            return;
        } else if (callback->errcode == WM_ERRCODE_FAILED) {
            if (sub_02039918()) {
                sub_02032844(WIRELESS_STATE_ERROR);
            } else {
                sub_02032844(WIRELESS_STATE_BAD_CONNECTION);
            }

            return;
        } else {
            sub_02032844(WIRELESS_STATE_ERROR);
        }

        return;
    }

    if (callback->state == WM_STATECODE_BEACON_LOST) {
        return;
    }

    if (callback->state == WM_STATECODE_CONNECTED) {
        if (_021D4124.sWirelessManager->pauseConnectionClient) {
            sub_02032858(WM_ERRCODE_DISCONNECTED);
            sub_02032844(WIRELESS_STATE_ERROR);
            return;
        } else {
            sub_02032844(WIRELESS_STATE_CONNECTED);

            if (!sub_02032FCC()) {
                sub_02032844(WIRELESS_STATE_BUSY);
                return;
            }

            _021D4124.sWirelessManager->aid = callback->aid;
            return;
        }
    } else if (callback->state == WM_STATECODE_CONNECT_START) {
        return;
    } else if (callback->state == WM_STATECODE_DISCONNECTED) {
        sub_02032858(WM_ERRCODE_DISCONNECTED);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    } else if (callback->state == WM_STATECODE_DISCONNECTED_FROM_MYSELF) {
        return;
    }

    sub_02032844(WIRELESS_STATE_ERROR);
}

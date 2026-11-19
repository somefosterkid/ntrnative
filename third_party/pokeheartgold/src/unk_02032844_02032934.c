#include "global.h"

#include "unk_02037C94.h"
#include "wireless_manager.h"

BOOL sub_02032A40(void);

u16 sub_02033264(void);

void sub_02032934(void *wm_startparentcallback);

extern const char _0210F8FC[];

void sub_02032934(void *wm_startparentcallback) {
    WMStartParentCallback *callback = (WMStartParentCallback *)wm_startparentcallback;
    const u16 connected = (u16)(1 << callback->aid);
    WirelessManager *new_var;
    if (callback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(callback->errcode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }
    switch (callback->state) {
    case WM_STATECODE_BEACON_SENT:
        _021D4124.sWirelessManager->sentBeaconCount++;
        break;

    case WM_STATECODE_CONNECTED:
        if (((((_021D4124.sWirelessManager->pauseConnectSystem == 1) || (_021D4124.sWirelessManager->pauseConnection == 1)) || (sub_02033264() >= _021D4124.sWirelessManager->numConnectionsMax)) || (callback->ssid[0] != sub_0203993C())) || (0 != memcmp(_0210F8FC, &callback->ssid[1], sizeof("DP")))) {
            WMErrCode disconnectCode;
            disconnectCode = WM_Disconnect(0L, callback->aid);
            if (disconnectCode != WM_ERRCODE_OPERATING) {
                sub_02032858(disconnectCode);
                sub_02032844(WIRELESS_STATE_ERROR);
            }
            break;
        }
        new_var = _021D4124.sWirelessManager;
        new_var->connectedBitmap |= connected;
        if (_021D4124.sWirelessManager->connectCallback) {
            _021D4124.sWirelessManager->connectCallback(callback->aid);
        }
        break;

    case WM_STATECODE_DISCONNECTED:
        _021D4124.sWirelessManager->connectedBitmap &= ~connected;
        if (_021D4124.sWirelessManager->disconnectCallback) {
            _021D4124.sWirelessManager->disconnectCallback(callback->aid);
        }
        break;

    case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
        break;

    case WM_STATECODE_PARENT_START:
        if (!sub_02032A40()) {
            sub_02032844(WIRELESS_STATE_ERROR);
        }
        break;

    default:
        break;
    }
}

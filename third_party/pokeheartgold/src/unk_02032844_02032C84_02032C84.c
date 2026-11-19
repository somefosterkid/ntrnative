#include "global.h"

#include "wireless_manager.h"

BOOL sub_02039AD8(int error);

void sub_02032D4C(void *callback);

BOOL sub_02032C84(void);

BOOL sub_02032C84(void) {
    u16 channel = WM_GetAllowedChannel();

    if (channel == 0x8000) {
        sub_02032858(WM_ERRCODE_ILLEGAL_STATE);
        sub_02039AD8(1);
        return 0;
    }

    if (channel == 0) {
        sub_02032858(WM_ERRCODE_DISCONNECTED_SERVER);
        sub_02039AD8(1);
        return 0;
    }

    if (_021D4124.sWirelessManager->channel == 0) {
        while (1) {
            _021D4124.sWirelessManager->scanParam.channel++;

            if (_021D4124.sWirelessManager->scanParam.channel > 16) {
                _021D4124.sWirelessManager->scanParam.channel = 1;
            }

            if (channel & (0x1 << (_021D4124.sWirelessManager->scanParam.channel - 1))) {
                break;
            }
        }
    } else {
        _021D4124.sWirelessManager->scanParam.channel = (u16)_021D4124.sWirelessManager->channel;
    }

    _021D4124.sWirelessManager->scanParam.maxChannelTime = WM_GetDispersionScanPeriod() / 3;
    _021D4124.sWirelessManager->scanParam.scanBuf = &_021D4124.sWirelessManager->bssDesc;

    WMErrCode errorCode = WM_StartScan(sub_02032D4C, &_021D4124.sWirelessManager->scanParam);

    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        return 0;
    }

    return 1;
}

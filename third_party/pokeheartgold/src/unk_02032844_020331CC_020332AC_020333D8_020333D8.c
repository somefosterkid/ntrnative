#include "global.h"

#include "wireless_manager.h"

BOOL sub_02039AD8(int error);

u16 sub_0203335C(u16 chanel);

void sub_020333D8(void *wm_measurechannelcallback);

void sub_020333D8(void *wm_measurechannelcallback) {
    WMMeasureChannelCallback *measureChannelCallback = (WMMeasureChannelCallback *)wm_measurechannelcallback;

    if (measureChannelCallback->errcode != WM_ERRCODE_SUCCESS) {
        sub_02032858(measureChannelCallback->errcode);

        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(1);

        return;
    }

    u16 channel = measureChannelCallback->channel;

    if (_021D4124.sWirelessManager->measureChannelBusyRatio > measureChannelCallback->ccaBusyRatio) {
        _021D4124.sWirelessManager->measureChannelBusyRatio = measureChannelCallback->ccaBusyRatio;
        _021D4124.sWirelessManager->leastUsedChannelBitmap = (u16)(1 << (channel - 1));
    } else if (_021D4124.sWirelessManager->measureChannelBusyRatio == measureChannelCallback->ccaBusyRatio) {
        _021D4124.sWirelessManager->leastUsedChannelBitmap |= 1 << (channel - 1);
    }

    u16 errorCode = sub_0203335C(++channel);

    if (errorCode == WM_ERRCODE_FATAL) {
        sub_02032844(WIRELESS_STATE_CHECK_CHANNEL);
        return;
    }

    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032844(WIRELESS_STATE_ERROR);
        return;
    }
}

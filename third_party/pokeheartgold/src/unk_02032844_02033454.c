#include "global.h"

#include "wireless_manager.h"

WMErrCode sub_02033454(WMCallbackFunc callback, u16 channel);

s16 sub_02033494(u16);
u16 sub_02033468(void);

WMErrCode sub_02033454(WMCallbackFunc callback, u16 channel) {
    return WM_MeasureChannel(callback, 3, 17, channel, 30);
}

u16 sub_02033468(void) {
    sub_02032844(1);
    _021D4124.sWirelessManager->measureChannel = sub_02033494(_021D4124.sWirelessManager->leastUsedChannelBitmap);
    return _021D4124.sWirelessManager->measureChannel;
}

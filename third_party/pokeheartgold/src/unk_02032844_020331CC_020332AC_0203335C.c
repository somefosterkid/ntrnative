#include "global.h"

#include "unk_02037C94.h"
#include "wireless_manager.h"

void sub_020333D8(void *callback);

WMErrCode sub_02033454(WMCallbackFunc callback, u16 port);

u16 sub_0203335C(u16 channel);

u16 sub_0203335C(u16 channel) {
    u16 allowedChannel = WM_GetAllowedChannel();

    if (allowedChannel == 0x8000) {
        sub_02032858(WM_ERRCODE_ILLEGAL_STATE);

        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(1);

        return WM_ERRCODE_ILLEGAL_STATE;
    }

    if (allowedChannel == 0) {
        sub_02032858(WM_ERRCODE_DISCONNECTED_SERVER);

        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(1);

        return WM_ERRCODE_FATAL;
    }

    while (((1 << (channel - 1)) & allowedChannel) == 0) {
        channel++;

        if (channel > 16) {
            return WM_ERRCODE_FATAL;
        }
    }

    u16 errorCode = sub_02033454(sub_020333D8, channel);

    return errorCode;
}

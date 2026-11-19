#include "global.h"

#include "wireless_manager.h"

BOOL sub_02039AD8(int error);

u16 sub_0203335C(u16 chanel);

BOOL sub_020332C0(void);

BOOL sub_020332C0(void) {
    u8 macAddress[6];

    OS_GetMacAddress(macAddress);

    _021D4124.sWirelessManager->rand = (u32)(OS_GetVBlankCount() + *(u16 *)&macAddress[0] + *(u16 *)&macAddress[2] + *(u16 *)&macAddress[4]);
    _021D4124.sWirelessManager->rand = _021D4124.sWirelessManager->rand * 69069UL + 12345;
    _021D4124.sWirelessManager->measureChannel = 0;
    _021D4124.sWirelessManager->measureChannelBusyRatio = 100 + 1;

    sub_02032844(WIRELESS_STATE_BUSY);

    u16 errorCode = sub_0203335C(1);

    if (errorCode == WM_ERRCODE_FATAL) {
        sub_02032858(WM_ERRCODE_FATAL);
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02039AD8(1);
        return 0;
    }

    if (errorCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errorCode);
        sub_02032844(WIRELESS_STATE_ERROR);
        return 0;
    }

    return 1;
}

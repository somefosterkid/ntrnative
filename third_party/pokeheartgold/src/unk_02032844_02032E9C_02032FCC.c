#include "global.h"

#include "wireless_manager.h"

void sub_0203301C(void *wm_startmpcallback);

u32 sub_02032FCC(void);

u32 sub_02032FCC(void) {
    WMErrCode WVar1;

    WVar1 = WM_StartMP(sub_0203301C, (u16 *)_021D4124.sWirelessManager->recvBuffer, (u16) _021D4124.sWirelessManager->recvBufferSize, (u16 *)_021D4124.sWirelessManager->sendBuffer, (u16) _021D4124.sWirelessManager->sendBufferSize, 1);
    if (WVar1 != WM_ERRCODE_OPERATING) {
        sub_02032858(WVar1);
        return 0;
    }
    return 1;
}

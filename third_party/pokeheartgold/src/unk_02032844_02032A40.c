#include "global.h"

#include "wireless_manager.h"

void sub_02032AB0(void *arg0);

u32 sub_02032A40(void);

u32 sub_02032A40(void) {
    WMErrCode WVar1;
    u32 ret;
    if (((_021D4124.sWirelessManager->state) - 4U) <= (3 - 1)) {
        ret = 1;
    } else {
        sub_02032844(4);
        WVar1 = WM_StartMP(sub_02032AB0, (u16 *)_021D4124.sWirelessManager->recvBuffer, (u16)(_021D4124.sWirelessManager->recvBufferSize), (u16 *)_021D4124.sWirelessManager->sendBuffer, (u16)(_021D4124.sWirelessManager->sendBufferSize), 1);
        if (WVar1 != WM_ERRCODE_OPERATING) {
            sub_02032858(WVar1);
            ret = 0;
        } else {
            ret = 1;
        }
    }
    return ret;
}

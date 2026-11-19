#include "global.h"

#include "wireless_manager.h"

void sub_020328A4(void *arg);

s32 sub_02032874(void);

s32 sub_02032874(void) {
    sub_02032844(3);
    WMErrCode errCode = WM_SetParentParameter(sub_020328A4, &_021D4124.sWirelessManager->parentParam);
    if (errCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errCode);
        sub_02032844(9);
        return 0;
    }
    return 1;
}

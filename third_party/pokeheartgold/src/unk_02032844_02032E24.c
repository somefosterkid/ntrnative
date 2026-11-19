#include "global.h"

#include "wireless_manager.h"

s32 sub_02032E24(void);

s32 sub_02032E24(void) {
    if (_021D4124.sWirelessManager->state != 2) {
        return 0;
    }
    sub_02032844(3);
    return 1;
}

static void sub_02032E64(void *arg);

s32 sub_02032E48(void);

s32 sub_02032E48(void) {
    WMErrCode errCode = WM_EndScan(sub_02032E64);
    if (errCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errCode);
        return 0;
    }
    return 1;
}

s32 sub_02032E9C(void);

static void sub_02032E64(void *arg0) {
    if (((WMCallback *)arg0)->errcode != 0) {
        sub_02032858(((WMCallback *)arg0)->errcode);
        return;
    }
    sub_02032844(1);
    if ((_021D4124.sWirelessManager->autoConnect != 0) && (sub_02032E9C() == 0)) {
        sub_02032844(9);
    }
}

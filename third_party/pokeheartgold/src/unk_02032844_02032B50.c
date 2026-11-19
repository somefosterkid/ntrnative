#include "global.h"

#include "wireless_manager.h"

static void sub_02032B6C();

u32 sub_02032B50(void);

u32 sub_02032B50(void) {
    WMErrCode WVar1;

    WVar1 = WM_EndParent(sub_02032B6C);
    if (WVar1 != WM_ERRCODE_OPERATING) {
        sub_02032858(WVar1);
        return 0;
    }
    return 1;
}

static void sub_02032B6C(void *param_1);

static void sub_02032B6C(void *param_1) {
    if (((WMCallback *)param_1)->errcode != 0) {
        sub_02032858((u32) * (u16 *)(param_1 + 2));
        return;
    }
    sub_02032844(1);
    return;
}

u32 sub_02032C84(void);

u32 sub_02032B84(u32 param_1, u16 *param_2, u16 param_3);

u32 sub_02032B84(u32 param_1, u16 *param_2, u16 param_3) {
    s32 iVar1;

    _021D4124.sWirelessManager->recvBufferSize = 0x200;
    _021D4124.sWirelessManager->sendBufferSize = 0x40;
    sub_02032844(2);
    _021D4124.sWirelessManager->bssDesc.channel = 1;
    *(u16 *)&_021D4124.sWirelessManager->scanParam.bssid[4] = param_2[2];
    *(u16 *)&_021D4124.sWirelessManager->scanParam.bssid[2] = param_2[1];
    *(u16 *)&_021D4124.sWirelessManager->scanParam.bssid[0] = *param_2;
    _021D4124.sWirelessManager->connectionType = param_1;
    _021D4124.sWirelessManager->scanCallback = NULL;
    _021D4124.sWirelessManager->channel = param_3;
    _021D4124.sWirelessManager->scanParam.channel = 0;
    _021D4124.sWirelessManager->autoConnect = 1;
    iVar1 = sub_02032C84();
    if (iVar1 == 0) {
        sub_02032844(9);
        return 0;
    }
    return 1;
}

u32 sub_02032C1C(WirelessManagerScanFunc scanCallback, u16 *param_2, u16 param_3);

u32 sub_02032C1C(WirelessManagerScanFunc scanCallback, u16 *param_2, u16 param_3) {
    s32 iVar1;

    sub_02032844(2);
    _021D4124.sWirelessManager->scanCallback = scanCallback;
    _021D4124.sWirelessManager->channel = param_3;
    _021D4124.sWirelessManager->scanParam.channel = 0;
    _021D4124.sWirelessManager->autoConnect = 0;
    *(u16 *)&_021D4124.sWirelessManager->scanParam.bssid[4] = param_2[2];
    *(u16 *)&_021D4124.sWirelessManager->scanParam.bssid[2] = param_2[1];
    *(u16 *)&_021D4124.sWirelessManager->scanParam.bssid[0] = *param_2;
    iVar1 = sub_02032C84();
    if (iVar1 == 0) {
        sub_02032844(9);
        return 0;
    }
    return 1;
}

#include "global.h"

#include "wireless_manager.h"

void sub_02032934(void *wm_startparentcallback);

BOOL sub_020328C8(void);

BOOL sub_020328C8(void) {
    struct WMStatus *pWVar1;
    WMErrCode WVar2;
    if (((_021D4124.sWirelessManager->state) - 4U) <= (3 - 1)) {
        return 1;
    }
    pWVar1 = (struct WMStatus *)WMi_GetStatusAddress();
    DC_InvalidateRange(&pWVar1->wep_flag, 4);
    pWVar1->wep_flag = 0;
    DC_FlushRange(&pWVar1->wep_flag, 4);
    WVar2 = WM_StartParent(sub_02032934);
    if (WVar2 != WM_ERRCODE_OPERATING) {
        sub_02032858(WVar2);
        return 0;
    }
    _021D4124.sWirelessManager->aid = 0;
    _021D4124.sWirelessManager->connectedBitmap = 1;
    return 1;
}

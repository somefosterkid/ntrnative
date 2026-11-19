#include "global.h"

#include "wireless_manager.h"

void sub_02033620(void *arg0);

BOOL sub_020335D4(int param_1);

BOOL sub_020335D4(int param_1) {
    WMErrCode WVar1;

    sub_02032844(3);
    if (param_1 == 1) {
        WVar1 = WM_Initialize(_021D4124.sWirelessManager->nitroManagerBuffer, sub_02033620, 2);
    } else {
        WVar1 = WM_InitializeForListening(_021D4124.sWirelessManager->nitroManagerBuffer, sub_02033620, 2, 0);
    }
    if (WVar1 != WM_ERRCODE_OPERATING) {
        sub_02032858(WVar1);
        sub_02032844(10);
        return 0;
    }
    return 1;
}

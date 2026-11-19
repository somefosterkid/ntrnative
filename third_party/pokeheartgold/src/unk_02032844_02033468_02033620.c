#include "global.h"

#include "unk_02037C94.h"
#include "wireless_manager.h"

void sub_020335BC(void *arg);

void sub_02033620(void *arg0);

void sub_02033620(void *arg0) {
    if (((WMCallback *)arg0)->errcode != 0) {
        sub_02032858(((WMCallback *)arg0)->errcode);
        sub_02032844(0xA);
        sub_02039AD8(5);
        return;
    }
    WMErrCode errCode = WM_SetIndCallback(sub_020335BC);
    if (errCode != WM_ERRCODE_SUCCESS) {
        sub_02032858(errCode);
        sub_02032844(0xA);
        sub_02039AD8(5);
        return;
    }
    sub_02032844(1);
}

void sub_02033664(void);

void sub_02033664(void) {
}

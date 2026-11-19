#include "global.h"

#include "wireless_manager.h"

static void sub_02032B30();

u32 sub_02032B0C(void);

u32 sub_02032B0C(void) {
    WMErrCode WVar1;

    sub_02032844(3);
    WVar1 = WM_EndMP(sub_02032B30);
    if (WVar1 != WM_ERRCODE_OPERATING) {
        sub_02032858(WVar1);
        return 0;
    }
    return 1;
}

u32 sub_02032B50(void);

void sub_02033830(void);

static void sub_02032B30(void *param_1);

static void sub_02032B30(void *param_1) {
    s32 iVar1;

    if (((WMCallback *)param_1)->errcode != 0) {
        sub_02032858((u32) * (u16 *)(param_1 + 2));
        sub_02033830();
        return;
    }
    iVar1 = sub_02032B50();
    if (iVar1 == 0) {
        sub_02033830();
    }
    return;
}

#include "global.h"

#include "wireless_manager.h"

u32 sub_020328C8(void);

void sub_020328A4(u8 *param_1);

void sub_020328A4(u8 *param_1) {
    s32 iVar1;

    if (((WMCallback *)param_1)->errcode != 0) {
        sub_02032858((u32) * (u16 *)(param_1 + 2));
        sub_02032844(9);
        return;
    }
    iVar1 = sub_020328C8();
    if (iVar1 == 0) {
        sub_02032844(9);
    }
    return;
}

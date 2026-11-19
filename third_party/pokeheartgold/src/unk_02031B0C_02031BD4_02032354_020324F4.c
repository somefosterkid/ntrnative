#include "global.h"

void sub_020324F4(u8 *param_1, u8 *param_2);

void sub_020324F4(u8 *param_1, u8 *param_2) {
    s32 iVar1;

    iVar1 = 0;
    do {
        param_2[iVar1] = param_1[iVar1 + 3];
        iVar1 = iVar1 + 1;
    } while (iVar1 < 5);
    return;
}

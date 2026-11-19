#include "global.h"

#include "unk_0208B1AC.h"

s32 sub_0208B85C(u8 *param_1, s32 param_2, s32 param_3, s8 param_4, s32 param_5) {
    s32 iVar1;
    iVar1 = 0;
    do {
        if (iVar1 > param_2) {
            *((u8 *)(param_1 + iVar1)) = 0;
        } else if (iVar1 > param_3) {
            *((u8 *)(param_1 + iVar1)) = 1;
        } else {
            *((u8 *)(param_1 + iVar1)) = param_4 + 2;
        }
        iVar1 = iVar1 + 1;
    } while (iVar1 < 5);
    if (0 < param_5) {
        return 1;
    }
    return -((u32)(param_5 < 0));
}

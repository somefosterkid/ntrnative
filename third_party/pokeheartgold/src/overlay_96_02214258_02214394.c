#include "global.h"

#include "error_handling.h"

u32 ov96_02214394(u32 param_1, u32 param_2);

u32 ov96_02214394(u32 param_1, u32 param_2) {
    s32 iVar1;
    u32 ret;

    switch (param_1) {
    case 0:
        iVar1 = 0xd8;
        break;
    case 1:
        iVar1 = 0xdc;
        break;
    case 2:
        iVar1 = 0xe0;
        break;
    case 3:
        iVar1 = 0xe4;
        break;
    default:
        GF_AssertFail();
        return 0;
    }
    if (param_2 >= 4) {
        GF_AssertFail();
        ret = 0;
    } else {
        ret = iVar1 + param_2 & 0xff;
    }
    return ret;
}

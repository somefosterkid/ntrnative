#include "global.h"

#include "error_handling.h"

void ov96_021E8340(u32 param_1, s32 param_2, u32 *param_3);

void ov96_021E8340(u32 param_1, s32 param_2, u32 *param_3) {
    u32 uVar1;
    u32 *p;
    switch (param_1) {
    case 1:
        p = param_3;
        break;
    case 2:
        p = param_3 + 1;
        break;
    case 3:
        p = param_3 + 2;
        break;
    case 4:
        p = param_3 + 3;
        break;
    case 5:
        p = param_3 + 4;
        break;
    case 6:
        p = param_3 + 5;
        break;
    case 7:
        p = param_3 + 6;
        break;
    case 8:
        p = param_3 + 7;
        break;
    default:
        GF_AssertFail();
        return;
    }
    uVar1 = *p;
    *p = uVar1 + param_2;
    if (9999 < (uVar1 + param_2)) {
        *p = 9999;
    }
}

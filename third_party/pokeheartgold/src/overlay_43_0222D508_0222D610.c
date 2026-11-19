#include "global.h"

void ov43_0222DD88(void *);

void ov43_0222DDD0(void *, void *);

void ov43_0222DE10(void *);

void ov43_0222DED0(void *, s32);

void ov43_0222D610(void *param_1, void *param_2, void *param_3);

void ov43_0222D610(void *param_1, void *param_2, void *param_3) {
    s32 iVar1;
    void *iVar2;
    s32 iVar3;

    iVar3 = 0;
    iVar2 = param_1 + 0x14;
    do {
        iVar1 = 0;
        do {
            ov43_0222DED0(iVar2, iVar1);
            iVar1 = iVar1 + 1;
        } while (iVar1 < 8);
        ov43_0222DE10(iVar2);
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + 0x4c;
    } while (iVar3 < 3);
    ov43_0222DDD0(param_1, param_3);
    param_1 += 0xF8;
    ov43_0222DD88(param_1);
    return;
}

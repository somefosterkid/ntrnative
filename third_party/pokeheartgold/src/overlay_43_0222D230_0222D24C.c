#include "global.h"

#include "unk_02005D10.h"

void ov43_0222AD40(u32, u32, u32);

s32 ov43_0222D230(s16 *, s32, u32);

void ov43_0222DB28(s16 *, u32, s32, u32);

void ov43_0222DB94(s16 *, u32, s16);

u32 ov43_0222D24C(s16 *param_1, void *param_2, u32 param_3, u32 param_4);

u32 ov43_0222D24C(s16 *param_1, void *param_2, u32 param_3, u32 param_4) {
    s32 iVar1;

    switch (param_4) {
    case 0:
        iVar1 = ov43_0222D230(param_1, (s32)*param_1, param_3);
        if (iVar1 != 0) {
            PlaySE(0x5dd);
            return 6;
        }
        break;
    case 1:
        PlaySE(0x5dd);
        return 6;
    case 2:
        ov43_0222DB28(param_1, param_3, (s32)*param_1, param_4);
        ov43_0222DB94(param_1, param_3, (s32)*param_1);
        *(s32 *)(param_1 + 4) = 2;
        return 3;
    case 3:
        ov43_0222DB28(param_1, param_3, (s32)*param_1, param_4);
        ov43_0222DB94(param_1, param_3, (s32)*param_1);
        *(s32 *)(param_1 + 4) = 3;
        return 3;
    case 4:
        *(s32 *)(param_1 + 4) = 0;
        param_1[6] = 4;
        ov43_0222AD40(param_3, 2, 1);
        return 5;
    case 5:
        *(s32 *)(param_1 + 4) = 1;
        param_1[6] = 4;
        ov43_0222AD40(param_3, 3, 1);
        return 5;
    }
    return 2;
}

#include "global.h"

#include "touchscreen.h"

s32 ov43_0222D230(s16 *, s32);

u32 ov43_0222D24C(s16 *param_1, void *param_2, u32 param_3, u32 param_4);

void ov43_0222ED18(s16 *, s32);

extern u32 ov43_0222EE42;

extern u32 ov43_0222EE84;

u32 ov43_0222D3B8(s16 *param_1, void *param_2, u32 param_3, u32 *param_4);

u32 ov43_0222D3B8(s16 *param_1, void *param_2, u32 param_3, u32 *param_4) {
    s32 iVar1;
    u32 uVar2;
    s32 iVar3;
    if ((*((s32 *)(param_1 + 8))) != 0) {
        iVar1 = TouchscreenHitbox_FindHitboxAtTouchNew((TouchscreenHitbox *)(&ov43_0222EE84));
    } else {
        iVar1 = TouchscreenHitbox_FindHitboxAtTouchNew((TouchscreenHitbox *)(&ov43_0222EE42));
    }
    if (iVar1 == (-1)) {
        return 2;
    }
    if (iVar1 == 0) {
        *param_4 = 1;
        uVar2 = ov43_0222D24C(param_1, param_2, param_3, 4);
        return uVar2;
    }
    if (iVar1 == 1) {
        *param_4 = 1;
        uVar2 = ov43_0222D24C(param_1, param_2, param_3, 5);
        return uVar2;
    }
    iVar3 = ov43_0222D230(param_1, ((iVar1 + (-2)) * 0x10000) >> 0x10);
    if (iVar3 != 0) {
        *param_1 = (iVar1) + (-2);
        *param_4 = 1;
        uVar2 = ov43_0222D24C(param_1, param_2, param_3, 1);
        return uVar2;
    }
    iVar1 = (((iVar1 + (-2)) - ((s32)(*param_1))) * 0x10000) >> 0x10;
    if (iVar1 == 0) {
        return 2;
    }
    *param_4 = 1;
    ov43_0222ED18(param_1, iVar1);
    if (((-1) + 1) <= iVar1) {
        uVar2 = ov43_0222D24C(param_1, param_2, param_3, 3);
        return uVar2;
    }
    uVar2 = ov43_0222D24C(param_1, param_2, param_3, 2);
    return uVar2;
}

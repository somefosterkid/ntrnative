#include "global.h"

#include "unk_02035900.h"
#include "unk_0208F658.h"

s32 ov65_0221DE24(void *, s32, s32);

s32 ov65_0221DE64(int, Party *, s32);

s32 sub_0208F66C(u32 param_1);

void sub_0208F680(s32 senderNetId, s32 unused, void *data, void *fieldSystemPtr) {
    s32 v0;
    void *v1;

    v1 = *(void **)((u8 *)fieldSystemPtr + 0x88);
    if (senderNetId != sub_0203769C()) {
        memcpy(*(void **)((u8 *)v1 + 0x2228), data, 0x590U);
        v0 = *(s32 *)((u8 *)v1 + 0x64) + 1;
        *(s32 *)((u8 *)v1 + 0x64) = v0;
        if ((s32)(v0 * 0x590) >= 0x590) {
            if (sub_0203769C() == 1) {
                ov65_0221DE24(v1, 0x1B, 0);
            } else {
                ov65_0221DE64(sub_0203769C(), *(void **)((u8 *)v1 + 0x2224), *(s32 *)((u8 *)v1 + 0x5C));
            }
        } else {
            ov65_0221DE64(sub_0203769C(), *(void **)((u8 *)v1 + 0x2224), *(s32 *)((u8 *)v1 + 0x5C));
        }
        *(s32 *)((u8 *)v1 + 0x5C) = (s32)(*(s32 *)((u8 *)v1 + 0x5C) + 1);
    }
}

void sub_0208F6F4(u32 param_1, u32 param_2, u8 *param_3, u8 *param_4) {
    u16 uVar1;
    s32 iVar2;
    u8 *iVar3;

    iVar3 = *(u8 **)(param_4 + 0x88);
    uVar1 = sub_0203769C();
    if (param_1 != uVar1) {
        iVar2 = sub_0208F66C((u32)*param_3);
        *(s32 *)(iVar3 + 0x98) = iVar2;
    }
    return;
}

void sub_0208F714(s32 param_1, u32 param_2, u8 *param_3, u8 *param_4) {
    *(u32 *)(*(u8 **)(param_4 + 0x88) + param_1 * 4 + 0x6c) = (u32)*param_3;
    return;
}

void sub_0208F724(void) {
    return;
}

void sub_0208F728(void) {
    return;
}

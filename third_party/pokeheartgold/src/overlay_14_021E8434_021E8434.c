#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E8434(UnkStruct_0201956C *param_1);

void sub_02019B1C(UnkStruct_0201956C *, u32, s8 *, s8 *);

void ov14_021E8434(UnkStruct_0201956C *param_1) {
    u32 uVar1;
    s8 cStack_1c;
    s8 acStack_1b[2];

    sub_02019B1C(param_1, 3, acStack_1b, &cStack_1c);
    if (acStack_1b[0] != 0x15) {
        uVar1 = 0;
        do {
            sub_020198FC(param_1, uVar1 + 3, -1, 0, (s32)acStack_1b[0] - 0x15U & 0xff);
            uVar1 = uVar1 + 1 & 0xffff;
        } while (uVar1 < 5);
        sub_02019B1C(param_1, 0xb, acStack_1b, &cStack_1c);
        sub_020198FC(param_1, 0xb, 0, -1, (s32)cStack_1c - 0x15U & 0xff);
    }
    return;
}

void ov14_021E84A4(UnkStruct_0201956C *param_1) {
    u32 uVar1;
    s8 cStack_1c;
    s8 acStack_1b[2];

    sub_02019B1C(param_1, 3, acStack_1b, &cStack_1c);
    if (acStack_1b[0] != 0x20) {
        uVar1 = 0;
        do {
            sub_020198FC(param_1, uVar1 + 3, 1, 0, 0x20U - (s32)acStack_1b[0] & 0xff);
            uVar1 = uVar1 + 1 & 0xffff;
        } while (uVar1 < 5);
        sub_02019B1C(param_1, 0xb, acStack_1b, &cStack_1c);
        sub_020198FC(param_1, 0xb, 0, 1, 0x18U - (s32)cStack_1c & 0xff);
    }
    return;
}

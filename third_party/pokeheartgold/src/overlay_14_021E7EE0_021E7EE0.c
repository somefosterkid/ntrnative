#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7EE0(UnkStruct_0201956C *param_1);
void ov14_021E7F4C(UnkStruct_0201956C *param_1);


void ov14_021E7EE0(UnkStruct_0201956C *param_1) {
    s8 cStack_14;
    s8 acStack_13[2];

    sub_02019B1C(param_1, 1, acStack_13, &cStack_14);
    if (cStack_14 != 6) {
        sub_020198FC(param_1, 1, 0, -1, (s32)cStack_14 - 6U & 0xff);
        if (acStack_13[0] == 2) {
            sub_020196E8(param_1, 10, 0x18, (cStack_14 + 0xf) * 0x1000000 >> 0x18);
            sub_020198FC(param_1, 10, 0, -1, (s32)cStack_14 - 6U & 0xff);
        }
    }
    return;
}

void ov14_021E7F4C(UnkStruct_0201956C *param_1) {
    s8 cStack_14;
    s8 acStack_13[2];

    sub_02019B1C(param_1, 1, acStack_13, &cStack_14);
    if (cStack_14 != 0x18) {
        sub_020198FC(param_1, 1, 0, 1, 0x18U - (s32)cStack_14 & 0xff);
        if (acStack_13[0] == 2) {
            sub_020196E8(param_1, 10, 0x18, (cStack_14 + 0xf) * 0x1000000 >> 0x18);
            sub_020198FC(param_1, 10, 0, 1, 0x18U - (s32)cStack_14 & 0xff);
        }
    }
    return;
}

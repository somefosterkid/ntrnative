#include "global.h"

#include "overlay_112_ir_state.h"

u32 ov112_021E5900(u8 *param_1, u32 param_2);

void ov112_021E5938(void);
void ov112_021E594C(void);

u32 ov112_021E5900(u8 *param_1, u32 param_2) {
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;

    uVar2 = 2;
    uVar3 = 0;
    while (uVar3 < param_2) {
        uVar1 = (u32)*param_1;
        param_1 = param_1 + 1;
        if ((uVar3 & 1) != 0) {
            uVar2 = uVar2 + uVar1;
        } else {
            uVar1 = uVar1 * 0x100;
            uVar2 = uVar2 + uVar1;
        }
        uVar3 = uVar3 + 1;
    }
    uVar2 = (uVar2 >> 0x10) + (uVar2 & 0xffff);
    return uVar2 + (uVar2 >> 0x10) & 0xffff;
}

void ov112_021E5938(void) {
    _021FF9E0.unk_18 = 1;
    _021FF9E0.unk_10 = (s32)(_021FF9E0.unk_10 + 1);
}

void ov112_021E594C(void) {
    _021FF9E0.unk_00 = 0;
    _021FF9E0.unk_28 = 0;
    _021FF9E0.unk_2C = 0;
    _021FF9E0.unk_18 = 0;
    _021FF9E0.unk_24 = 0;
    _021FF9E0.unk_08 = 0;
    _021FF9E0.unk_10 = 0;
}

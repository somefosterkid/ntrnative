#include "global.h"

#include "overlay_96_app_0221040c.h"

u32 ov96_02210324(Ov96App_0221040C *param_1, u8 param_2, u16 param_3) {
    BOOL BVar1;
    u32 uVar2;
    unsigned int unaff_r6;
    BVar1 = ov96_021E5F24(param_1->course);
    if ((param_2 == BVar1) && (param_3 != param_1->unk_E6)) {
        if (param_1->unk_E6 > param_3) {
            param_1->unk_E5 = param_1->unk_E5 + 1;
        }
        uVar2 = param_3 + (param_1->unk_E5 * 0x80);
        if (((s32)uVar2) > (1000 - 1)) {
            uVar2 = 999;
        } else if (((s32)uVar2) < 0) {
            uVar2 = 0;
        }
        ov96_0221058C(param_1, uVar2 & 0xff);
        unaff_r6 = (param_3 - param_1->unk_E6) & 0xffff;
        param_1->unk_E6 = param_3;
    }
    return unaff_r6 & 0xff;
}

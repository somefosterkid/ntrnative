#include "global.h"

#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "yes_no_prompt.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

s32 ov73_021E66F0(void *);

s32 ov73_021E67A8(void *arg0);

s32 ov73_021E6890(void *arg0, s32 arg1);

s32 ov73_021E6890(void *arg0, s32 arg1) {
    ov73_021E67A8(arg0);
    *(s32 *)((u8 *)arg0 + 0x318) = 5;
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E670C(void *, s32);

s32 ov73_021E6830(void *);

s32 ov73_021E746C(void);

s32 ov73_021E756C(void *, s32, s32);

s32 ov73_021E781C(void *, s32);

s32 ov73_021E68AC(UnkStruct_020961E8 *arg0, s32 arg1);

s32 ov73_021E68AC(UnkStruct_020961E8 *arg0, s32 arg1) {
    s32 v1;
    s32 v0;
    enum YesNoResponse v2;
    s32 v3;

    if (sub_0203769C() != 0) {
        if (arg0->unk_4AA0 != 0) {
            if (0xC3 & gSystem.newKeys) {
                PlaySE(0x5F2U);
            }
            ov73_021E66F0(arg0);
            return arg1;
        }
        goto block_9;
    }
    if (arg0->unk_4AB4 != 0) {
        if (0xC3 & gSystem.newKeys) {
            PlaySE(0x5F2U);
        }
        ov73_021E66F0(arg0);
        return arg1;
    }
block_9:
    v3 = ov73_021E746C();
    if (v3 != sub_02037454()) {
        ov73_021E66F0(arg0);
        return arg1;
    }
    v2 = YesNoPrompt_HandleInput(arg0->unk_384);
    if (v2 != YESNORESPONSE_WAIT) {
        if (v2 == YESNORESPONSE_NO) {
            if (sub_0203769C() == 0) {
                v1 = 0;
                sub_02037030(0x75, &v1, 1);
                ov73_021E781C(arg0, 1);
            }
            ov73_021E670C(arg0, 0);
        } else if (sub_0203769C() == 0) {
            ov73_021E670C(arg0, 0xB);
            ov73_021E756C(arg0, 0xE, 0);
        } else {
            MI_CpuFill8(&v0, 0U, 4U);
            *(s8 *)((u8 *)(&v0) + 2) = 0;
            *(s8 *)(&v0) = (s8)sub_0203769C();
            arg0->unk_4AB8 = 1;
            arg0->unk_4AC2 = 0;
            arg0->unk_3A8 = 6;
            sub_02037030(0x70, &v0, 4);
        }
        ov73_021E6830(arg0);
    }
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E69D0(void *arg0, s32 arg1);

s32 ov73_021E69D0(void *arg0, s32 arg1) {
    *(s16 *)((u8 *)arg0 + 0x4A32) = 0;
    ov73_021E66F0(arg0);
    return arg1;
}

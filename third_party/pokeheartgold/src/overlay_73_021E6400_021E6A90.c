#include "global.h"

#include "error_handling.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "yes_no_prompt.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

s32 ov73_021E66F0(void *);

s32 ov73_021E67A8(void *arg0);

s32 ov73_021E6A90(void *arg0, s32 arg1);

s32 ov73_021E6A90(void *arg0, s32 arg1) {
    ov73_021E67A8(arg0);
    *(s32 *)((u8 *)arg0 + 0x318) = 0x17;
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E670C(void *, s32);

s32 ov73_021E6830(void *);

s32 ov73_021E71E4(void *, s32, s8);

int ov73_021E746C(void);

s32 ov73_021E781C(void *, s32);

s32 ov73_021E6AAC(UnkStruct_020961E8 *arg0, s32 arg1);

s32 ov73_021E6AAC(UnkStruct_020961E8 *arg0, s32 arg1) {
    s32 v0;
    enum YesNoResponse v1;

    if ((arg0->unk_4AAC != ov73_021E746C()) || (arg0->unk_4AB4 != 0)) {
        if (0xC3 & gSystem.newKeys) {
            PlaySE(0x5F2U);
        }
        ov73_021E66F0(arg0);
        return arg1;
    }
    v1 = YesNoPrompt_HandleInput(arg0->unk_384);
    if (v1 != YESNORESPONSE_WAIT) {
        ov73_021E6830(arg0);
        if (v1 == YESNORESPONSE_NO) {
            v0 = 0;
            sub_02037030(0x75, &v0, 1);
            ov73_021E781C(arg0, 1);
            ov73_021E670C(arg0, 0);
        } else {
            ov73_021E71E4(arg0, 1, 2);
            ov73_021E71E4(arg0, 0, 2);
            if (sub_0203769C() == 0) {
                arg0->unk_3A8 = 0x18;
                arg0->unk_4ABA = ov73_021E746C();
            } else {
                GF_AssertFail();
            }
        }
    }
    ov73_021E66F0(arg0);
    return arg1;
}

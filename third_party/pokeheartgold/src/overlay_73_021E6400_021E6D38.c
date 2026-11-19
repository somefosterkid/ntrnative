#include "global.h"

#include "message_format.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "yes_no_prompt.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

typedef struct UnkStruct_ov73_021E6DF8 {
    u8 unk_0[0x24];
    MessageFormat *unk_24;
    u8 unk_28[0x2FC];
    s32 unk_324;
} UnkStruct_ov73_021E6DF8;

s32 ov73_021E66F0(void *);

s32 ov73_021E67A8(void *arg0);

s32 ov73_021E6D38(void *arg0, s32 arg1);

s32 ov73_021E6D38(void *arg0, s32 arg1) {
    ov73_021E67A8(arg0);
    *(s32 *)((u8 *)arg0 + 0x318) = 0xC;
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E6830(void *);

s32 ov73_021E746C(void);

s32 ov73_021E781C(void *, s32);

s32 ov73_021E6D54(UnkStruct_020961E8 *arg0, s32 arg1);

s32 ov73_021E6D54(UnkStruct_020961E8 *arg0, s32 arg1) {
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
        if (v1 == YESNORESPONSE_NO) {
            v0 = 0;
            arg0->unk_3A8 = 0;
            sub_02037030(0x75, &v0, 1);
            ov73_021E781C(arg0, 1);
        } else {
            arg0->unk_3A8 = 0xD;
            sub_02037030(0x71, NULL, 0);
            BufferPlayersName(arg0->unk_24, 0U, sub_02034818(0U));
        }
        ov73_021E6830(arg0);
    }
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E670C(void *, s32);

s32 ov73_021E756C(void *, s32, s32);

s32 ov73_021E6DF8(UnkStruct_ov73_021E6DF8 *arg0, s32 arg1);

s32 ov73_021E6DF8(UnkStruct_ov73_021E6DF8 *arg0, s32 arg1) {
    if (sub_0203769C() == 0) {
        ov73_021E756C(arg0, 5, 0);
    } else {
        BufferPlayersName(arg0->unk_24, 0U, sub_02034818(0U));
        ov73_021E756C(arg0, 0x10, 0);
    }
    ov73_021E670C(arg0, 0xE);
    arg0->unk_324 = 0;
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E6E48(void *arg0, s32 arg1);

s32 ov73_021E6E48(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x324) = (s32)(*(s32 *)((u8 *)arg0 + 0x324) + 1);
    if ((s32) * (s32 *)((u8 *)arg0 + 0x324) > 0x2D) {
        *(s32 *)((u8 *)arg0 + 0x318) = 0xF;
    }
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E6E6C(void *arg0, s32 arg1);

s32 ov73_021E6E6C(void *arg0, s32 arg1) {
    sub_02037AC0(0xC9U);
    *(s32 *)((u8 *)arg0 + 0x318) = 0x10;
    ov73_021E66F0(arg0);
    return arg1;
}

#include "global.h"

#include "error_handling.h"

typedef struct UnkStruct_ov96_021ED17C {
    u8 unk_0[0x4];
    u8 unk_4;
    u8 unk_5[0x13];
    u8 unk_18;
    u8 unk_19[0x153];
    u8 *unk_16C;
    u8 *unk_170;
} UnkStruct_ov96_021ED17C;

s32 ov96_021ECDEC(void *, void *, void *);

void ov96_021ED17C(UnkStruct_ov96_021ED17C *arg0, s32 arg1);

void ov96_021ED17C(UnkStruct_ov96_021ED17C *arg0, s32 arg1) {
    s32 sp4;
    s32 sp0;
    s32 *var_r4;
    s32 temp_r0;
    u8 *var_r5;
    s32 var_r7;

    sp0 = arg1;
    var_r7 = 0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    sp4 = 0;
    var_r4 = (void *)&arg0->unk_18;
    var_r5 = (u8 *)((void *)&arg0->unk_4);
loop_3:
    if ((((u32)(*var_r4 << 0xB) >> 0x1F) != 0) || (ov96_021ECDEC(var_r5, arg0->unk_16C, arg0->unk_170), var_r7 += 1, (var_r7 < sp0))) {
        var_r4 += 6;
        temp_r0 = sp4 + 1;
        var_r5 += 0x18;
        sp4 = temp_r0;
        if (temp_r0 < 0xF) {
            goto loop_3;
        }
    }
}

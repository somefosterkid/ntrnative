#include "global.h"

typedef struct UnkStruct_ov96_021FEAEC_temp_r4 {
    u8 unk_0[0x4D4];
    s32 unk_4D4;
    void *unk_4D8;
    u16 unk_4DC;
    u8 unk_4DE;
} UnkStruct_ov96_021FEAEC_temp_r4;

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB588(void *, s32 *);

void ov96_021FEAEC(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov96_021FEAEC(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp8[3];
    s32 sp4;
    void *sp0;
    s32 temp_r5;
    s32 var_r5;
    u8 var_r7;
    u8 var_r6;
    UnkStruct_ov96_021FEAEC_temp_r4 *temp_r4;
    sp0 = arg0;
    sp4 = arg1;
    var_r6 = 0;
loop_1:
    temp_r4 = (void *)(sp0 + (var_r6 * 0xC));

    if ((temp_r4->unk_4D4) != 0) {
        var_r7 = temp_r4->unk_4DE;
        temp_r5 = sp4 - (temp_r4->unk_4DC);
        if (((double)temp_r5) >= 768.0) {
            var_r5 = (s32)(80.0 + (1024.0 - ((double)temp_r5)));
        } else if (((double)temp_r5) <= (-768.0)) {
            var_r5 = (s32)(80.0 - (1024.0 + ((double)temp_r5)));
        } else {
            var_r5 = 0x50 - temp_r5;
        }
        sp8[2] = 0;
        sp8[0] = var_r5 << 0xC;
        sp8[1] = (var_r7 + 0x20) << 0xC;
        ov96_021EB588(temp_r4->unk_4D8, sp8);
        if ((var_r5 >= (-0x20)) && (var_r5 <= 0x120)) {
            ov96_021EB52C(temp_r4->unk_4D8, 1, 1);
        } else {
            ov96_021EB52C(temp_r4->unk_4D8, 1, 0);
        }
        var_r6 += 1;
        if (((u32)var_r6) < 0x1EU) {
            goto loop_1;
        }
    }
}

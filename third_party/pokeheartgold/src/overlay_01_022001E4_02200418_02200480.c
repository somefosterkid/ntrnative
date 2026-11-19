#include "global.h"

#include "unk_02023694.h"
#include "unk_020689C8.h"

void ov01_02200480(void *arg0, void *arg1);

typedef struct Ov01Work_ov01_02200480_arg1 {
    s32 unk_00;
    s32 unk_04;
    u8 unk_08[0xc];
    s32 unk_14;
    u8 unk_18[0x4];
    s32 unk_1C;
    u8 unk_20[0x8];
    s32 unk_28;
    u8 unk_2C[0x14];
    Sprite *unk_40;
} Ov01Work_ov01_02200480_arg1; // size 0x44

void ov01_02200480(void *arg0, void *arg1_) {
    Ov01Work_ov01_02200480_arg1 *arg1 = (Ov01Work_ov01_02200480_arg1 *)arg1_;
    VecFx32 sp0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    sub_02068DB8(arg0, &sp0);
    temp_r0_3 = arg1->unk_00;
    switch (temp_r0_3) {
    case 0:
        sub_02023EA4(arg1->unk_40, 1);
        temp_r0_2 = (arg1->unk_1C) + (arg1->unk_28);
        arg1->unk_1C = temp_r0_2;
        if (temp_r0_2 != 0) {
            arg1->unk_28 = (s32)((arg1->unk_28) - 0x2000);
        } else {
            arg1->unk_28 = 0;
            arg1->unk_00 = (s32)((((((arg1->unk_00) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) + 1);
        }
        break;

    case 1:
        temp_r0_3 = (arg1->unk_04) + 1;
        arg1->unk_04 = temp_r0_3;
        if (temp_r0_3 >= 0x1E) {
            arg1->unk_00 = (s32)((arg1->unk_00) + 1);
            arg1->unk_04 = 0;
            arg1->unk_14 = 1;
        }
        break;

    case 2:
        break;
    }

    sub_02068DA8(arg0, &sp0);
}

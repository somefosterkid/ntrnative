#include "global.h"
#include "overlay_59_apricorn_app.h"

void ov59_0223A7FC(Ov59ApricornApp *arg0, s32 arg1);

void ov59_0223A7FC(Ov59ApricornApp *arg0, s32 arg1) {
    u8 temp_r1;
    u8 temp_r1_2;
    u8 temp_r1_3;
    u8 temp_r1_4;
    u8 temp_r1_5;
    u8 temp_r3;

    if (arg1 == 0) {
        arg0->unk_4B = (u8) arg0->unk_4A;
        arg0->unk_49 = 1;
        temp_r1 = arg0->unk_4A;
        switch (temp_r1) {
        case 1:
        case 2:
            temp_r3 = arg0->unk_4C;
            if ((temp_r1 != temp_r3) && ((temp_r1 - 1) != temp_r3)) {
                arg0->unk_4A = 1U;
                return;
            }
            arg0->unk_4A = temp_r3;
            return;
        case 3:
            arg0->unk_4A = 2U;
            return;
        case 0:
        default:
            arg0->unk_4A = 0U;
            return;
        }
    }
    arg0->unk_4C = (u8) arg0->unk_4A;
    arg0->unk_49 = 0;
    temp_r1_2 = arg0->unk_4A;
    switch (temp_r1_2) {
    case 0:
        temp_r1_3 = arg0->unk_4B;
        if ((u32)temp_r1_3 > 1U) {
            arg0->unk_4A = 1U;
            return;
        }
        arg0->unk_4A = temp_r1_3;
        return;
    case 1:
        temp_r1_4 = arg0->unk_4B;
        if (temp_r1_4 == 0) {
            arg0->unk_4A = 1U;
            return;
        }
        if (temp_r1_4 == 3) {
            arg0->unk_4A = 2U;
            return;
        }
        arg0->unk_4A = temp_r1_4;
        return;
    case 2:
        temp_r1_5 = arg0->unk_4B;
        if ((u32)temp_r1_5 < 2U) {
            arg0->unk_4A = 2U;
            return;
        }
        arg0->unk_4A = temp_r1_5;
        return;
    default:
        arg0->unk_4A = 0U;
        return;
    }
}

#include "global.h"
#include "overlay_59_apricorn_app.h"

void ov59_02238C24(Ov59ApricornApp *arg0);

s32 ov59_02238F24(Ov59ApricornApp *arg0);

s32 ov59_02238C40(Ov59ApricornApp *, s32);

s32 ov59_02238D90(Ov59ApricornApp *);

void ov59_02238E98(Ov59ApricornApp *work);

s32 ov59_02238FF4(Ov59ApricornApp *, s32);

s32 ov59_022384E4(Ov59ApricornApp *arg0);

s32 ov59_022384E4(Ov59ApricornApp *arg0) {
    s32 temp_r0;
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        ov59_02238E98(arg0);
        ov59_02238C40(arg0, 8);
        ov59_02238FF4(arg0, 1);
        arg0->state = (u16)(arg0->state + 1);
        break;
    case 1:
        temp_r0 = ov59_02238F24(arg0);
        if (temp_r0 != -1) {
            arg0->state = 0U;
            if (((u32)temp_r0 >= 2U) || (temp_r0 == -2)) {
                ov59_02238C24(arg0);
                return 4;
            }
            if (temp_r0 == 1) {
                ov59_02238C40(arg0, 9);
                ov59_02238D90(arg0);
                ov59_02238FF4(arg0, 1);
                return 0xF;
            }
            arg0->parent->unk_18 = 3;
            arg0->parent->unk_1C = 0;
            arg0->parent->unk_1A = (u8) arg0->unk_4B;
            arg0->parent->unk_1B = (u8) arg0->unk_4C;
            return 1;
        }
        break;
    }
    return 0xB;
}

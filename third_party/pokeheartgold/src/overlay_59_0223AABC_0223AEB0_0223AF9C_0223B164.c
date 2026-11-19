#include "global.h"

#include "overlay_59_juice_feed_app.h"
#include "text.h"
#include "unk_02005D10.h"

extern s32 ov59_0223C99C[];

s32 ov59_0223B164(Ov59JuiceFeedApp *arg0) {
    s32 temp_r4;
    s8 temp_r1_2;
    u16 temp_r1;
    temp_r4 = ov59_0223C99C[arg0->statIndex];
    temp_r1 = arg0->subState;
    switch (temp_r1) {
    case 0:
        arg0->timer = 0U;
        arg0->deltaStep = 0U;
        temp_r1_2 = arg0->deltas[arg0->statIndex];
        if (((s32)temp_r1_2) < 0) {
            arg0->deltaAbs = (s8)(0 - temp_r1_2);
            PlaySE(0x926U);
        } else {
            arg0->deltaAbs = temp_r1_2;
            PlaySE(0x925U);
        }
        arg0->subState = (u16)(arg0->subState + 1);

    case 1:
        temp_r1 = arg0->timer;
        arg0->timer -= 1;
        if (temp_r1 == 0) {
            if (((s32)arg0->deltas[arg0->statIndex]) < 0) {
                ov59_0223BE70(arg0, (u8)temp_r4, 1);
            } else {
                ov59_0223BE70(arg0, (u8)temp_r4, 0);
            }
            if (((u32)arg0->deltaStep) >= ((u32)((u8)arg0->deltaAbs))) {
                arg0->subState = (u16)(arg0->subState + 1);
            } else {
                arg0->timer = 0xFU;
            }
        }
        break;

    case 2:
        if (arg0->taskCount == 0) {
            ov59_0223BD4C(arg0, &arg0->mons[arg0->selected], (u8)temp_r4);
            ov59_0223BBB0(arg0);
            if (((s32)arg0->deltas[arg0->statIndex]) < 0) {
                ov59_0223BBD4(arg0, temp_r4 + 7);
            } else {
                ov59_0223BBD4(arg0, temp_r4 + 2);
            }
            arg0->subState = (u16)(arg0->subState + 1);
        }
        break;

    default:
        if (TextPrinterCheckActive(arg0->printerId) == 0) {
            arg0->subState = 0U;
            return 1;
        }
        break;
    }

    return 0;
}

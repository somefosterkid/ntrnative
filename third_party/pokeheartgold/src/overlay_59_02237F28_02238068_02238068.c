#include "global.h"
#include "overlay_59_apricorn_app.h"

s32 ov59_022385AC(Ov59ApricornApp *arg0);

s32 ov59_02238CFC(Ov59ApricornApp *, s32, u32);

s32 ov59_02238D74(Ov59ApricornApp *);

s32 ov59_02238FF4(Ov59ApricornApp *, s32);

s32 sub_02031BEC(void *, s32);

s32 sub_02031C00(void *);

s32 ov59_02238068(Ov59ApricornApp *arg0);

s32 ov59_02238068(Ov59ApricornApp *arg0) {
    s32 temp_r0;

    if (arg0->state == 0) {
        ov59_02238FF4(arg0, 1);
        temp_r0 = sub_02031C00(arg0->apricornBox);
        switch (temp_r0) {
        case 0:
            ov59_02238CFC(arg0, 0x16, arg0->textFrameDelay);
            break;
        case 2:
            ov59_02238CFC(arg0, 0x17, 0xFFU);
            break;
        case 3:
            ov59_02238CFC(arg0, 0x18, 0xFFU);
            break;
        }
        sub_02031BEC(arg0->apricornBox, 1);
        arg0->state = (u16)(arg0->state + 1);
    } else {
        if (ov59_022385AC(arg0) != 0) {
            ov59_02238FF4(arg0, 0);
            ov59_02238D74(arg0);
            arg0->state = 0U;
            return 4;
        }
    }
    return 2;
}

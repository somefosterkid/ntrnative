#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "unk_02005D10.h"

s32 ov59_022387D0(Ov59ApricornApp *, s32);

s32 ov59_022388C4(Ov59ApricornApp *arg0);

s32 ov59_02238CFC(Ov59ApricornApp *, s32, s32);

s32 ov59_02238FF4(Ov59ApricornApp *, s32);

s32 ov59_0223A194(Ov59ApricornApp *arg0, u32 arg1);

s32 ov59_0223A194(Ov59ApricornApp *arg0, u32 arg1) {
    u8 temp_r1;

    if (arg1 <= 6U) {
        ov59_022388C4(arg0);
        if (arg0->apricornCounts[arg1] == 0) {
            return 4;
        }
        PlaySE(0x5EBU);
        arg0->selected = (s8)arg1;
        if (arg0->menuInputState == 0) {
            return 7;
        }
        return 8;
    }
    if (arg1 == 8) {
        PlaySE(0x5DCU);
        ov59_022387D0(arg0, 1);
        return 1;
    }
    if (arg1 == 7) {
        PlaySE(0x5DCU);
        ov59_022388C4(arg0);
        arg0->selected = (s8)arg1;
        temp_r1 = arg0->unk_14;
        if ((temp_r1 == 0) && (arg0->unk_18 == 0)) {
            ov59_02238CFC(arg0, 0x19, 0xFF);
            ov59_02238FF4(arg0, 1);
            return 0xF;
        }
        if ((temp_r1 == 0) && (arg0->unk_18 != 0)) {
            ov59_02238CFC(arg0, 0x1B, 0xFF);
            ov59_02238FF4(arg0, 1);
            return 0xF;
        }
        if ((temp_r1 != 0) && (arg0->unk_18 != 0)) {
            ov59_02238CFC(arg0, 0x1C, 0xFF);
            ov59_02238FF4(arg0, 1);
            return 0xF;
        }
        return 0xB;
    }
    return 4;
}

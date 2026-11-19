#include "global.h"

#include "sound_02004A44.h"
#include "unk_020210A0.h"
#include "overlay_113.h"

void ov113_021E5D4C(UnownReportData *arg0);

s32 ov113_021E5CF8(UnownReportData *arg);

s32 ov113_021E5B70(UnownReportData *arg0);

s32 ov113_021E5B70(UnownReportData *arg0) {
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        ov113_021E5CF8(arg0);
        arg0->state = (u16)(arg0->state + 1);
        break;
    case 1:
        ov113_021E5D4C(arg0);
        EnableTouchPad();
        InitializeTouchPad(1);
        Sound_SetSceneAndPlayBGM(0x45U, 0U, 0);
        arg0->state = 0U;
        return 1;
    }
    return 0;
}

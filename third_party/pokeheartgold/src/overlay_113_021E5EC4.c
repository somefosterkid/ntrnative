#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "overlay_113.h"

void ov113_021E5EC4(UnownReportData *arg0);

s32 ov113_021E5E64(UnownReportData *, s32);
s32 ov113_021E5ED0(UnownReportData *arg0, s32 arg1);

void ov113_021E5EC4(UnownReportData *arg0) {
    Sprite_IsAnimated(arg0->sprites[0]);
}

s32 ov113_021E5ED0(UnownReportData *arg0, s32 arg1) {
    u8 v0;
    u8 v1;

    switch (arg1) {
    case 0:
        PlaySE(0x5DDU);
        ov113_021E5E64(arg0, 1);
        return 1;
    case 1:
        v0 = arg0->page;
        if (v0 == 0) {
            return 2;
        }
        arg0->page = (u8)(v0 - 1);
        Sprite_SetAnimCtrlSeq(arg0->sprites[1], 3);
        PlaySE(0x5DCU);
        return 3;
    case 2:
        v1 = arg0->page;
        if ((s32)v1 >= (s32)(arg0->numPages - 1)) {
            return 2;
        }
        arg0->page = (u8)(v1 + 1);
        Sprite_SetAnimCtrlSeq(arg0->sprites[2], 6);
        PlaySE(0x5DCU);
        return 3;
    default:
        return 2;
    }
}

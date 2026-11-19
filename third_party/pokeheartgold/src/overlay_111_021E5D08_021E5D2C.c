#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "unk_0208805C.h"

void ov111_021E5D2C(Window *param_1, u8 param_2, u8 param_3);

void ov111_021E5D2C(Window *param_1, u8 param_2, u8 param_3) {
    BgConfig *bgConfig;
    u8 uVar1;
    u8 bgId;
    u8 bVar2;
    u8 uVar3;
    u8 y;
    u16 fillValue;
    s16 unaff_r6;
    u32 uVar4;
    bgConfig = GetWindowBgConfig(param_1);
    bgId = GetWindowBgId(param_1);
    uVar1 = GetWindowX(param_1);
    y = GetWindowY(param_1);
    bVar2 = CalculateHpBarPixelsLength(param_2, param_3, 0x30);
    uVar4 = (u16)bVar2;
    uVar3 = CalculateHpBarColor((u16)param_2, (u16)param_3, 0x30);
    switch (uVar3) {
    case 0:

    case 3:

    case 4:
        unaff_r6 = 1;
        break;

    case 2:
        unaff_r6 = 10;
        break;

    case 1:
        unaff_r6 = 0x13;
        break;

    default:
        GF_AssertFail();
    }

    bVar2 = 0;
    do {
        if (uVar4 >= 8) {
            fillValue = unaff_r6 + 8;
        } else {
            fillValue = unaff_r6 + uVar4;
        }
        FillBgTilemapRect(bgConfig, bgId, fillValue, uVar1 + bVar2, y, 1, 1, 0x11);
        if (uVar4 < 8) {
            uVar4 = 0;
        } else {
            uVar4 = (uVar4 - 8) & 0xff;
        }
        bVar2 = bVar2 + 1;
    } while (bVar2 < 6);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
    return;
}

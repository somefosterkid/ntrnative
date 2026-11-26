#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad_fns.h"
#include "unk_02005D10.h"

BOOL ov43_0222DCC4(PalPadShared *param0, int param1);
void ov43_0222A318(PalPadShared *, s8, s8);
void ov43_0222D4C4(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, u32 param3);
void ov43_0222D654(PalPadDetails *, PalPadShared *, PalPadGfx *, u32);
void ov43_0222D778(PalPadDetails *, PalPadGfx *);
void ov43_0222D87C(PalPadDetails *, PalPadGfx *);
u32 ov43_0222D15C(PalPadDetails *param_1, PalPadShared *param_2, PalPadGfx *param_3, u32 param_4);
void ov43_0222D610(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
void ov43_0222D228(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
s32 ov43_0222D230(PalPadDetails *details, s32 arg1);

u32 ov43_0222D15C(PalPadDetails *param_1, PalPadShared *param_2, PalPadGfx *param_3, u32 param_4) {
    u8 uVar1;
    BOOL BVar2;

    switch (param_2->subState) {
    case 0:
        param_1->friendChangeHeld = 0;
        param_1->page = 0;
    case 1:
        ov43_0222D654(param_1, param_2, param_3, param_4);
        param_2->subState = 2;
        break;
    case 2:
        uVar1 = ov43_0222D47C(param_1, param_2, param_3);
        param_2->subState = uVar1;
        break;
    case 3:
        ov43_0222D4C4(param_1, param_2, param_3, param_4);
        ov43_0222D508(param_1, param_2, param_3, param_4);
        param_2->subState = 4;
        break;
    case 4:
        BVar2 = ov43_0222D508(param_1, param_2, param_3, param_4);
        if (BVar2 == 1) {
            param_2->subState = 2;
        }
        break;
    case 5:
        if ((param_1->friendChangeHeld == 0) || (param_1->friendChangeDelay = param_1->friendChangeDelay - 1, param_1->friendChangeDelay == 0)) {
            BVar2 = ov43_0222DCC4(param_2, param_1->direction);
            if (BVar2 != 0) {
                PlaySE(0x5e5);
                ov43_0222D87C(param_1, param_3);
                ov43_0222A318(param_2, 4, 1);
                param_1->friendChangeHeld = 1;
                return 1;
            }
            param_2->subState = 2;
        }
        break;
    case 6:
        ov43_0222D778(param_1, param_3);
        ov43_0222A318(param_2, 1, 9);
        return 1;
    }
    return 0;
}

void ov43_0222D228(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx) {
    ov43_0222D610(details, shared, gfx);
}

s32 ov43_0222D230(PalPadDetails *details, s32 arg1) {
    s32 v0;

    if (details->hasFrontierPages == 1) {
        v0 = 8;
    } else {
        v0 = 3;
    }
    if (arg1 == (v0 - 1)) {
        return 1;
    }
    return 0;
}

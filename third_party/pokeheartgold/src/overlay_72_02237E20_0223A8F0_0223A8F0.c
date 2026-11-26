#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "overlay_72_wfc_connect.h"

void ov72_022386F4(Sprite *, s32);

void ov72_02238730(Sprite *, s32, u32);

s32 ov72_0223A7F4(NintendoWfcNumberInput *, s32);

u8 ov72_0223A8DC(NintendoWfcNumberInput *);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov72_0223AED0(NintendoWfcConnect *, s8, s8, s8);
#else
s32 ov72_0223AED0(NintendoWfcConnect *, u8, u8, s32);
#endif

extern u8 ov72_0223B478[];

extern u8 ov72_0223B479[];

void ov72_0223A8F0(NintendoWfcConnect *arg0, u32 arg1);

void ov72_0223A8F0(NintendoWfcConnect *arg0, u32 arg1) {
    switch (arg1) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        if (ov72_0223A7F4(&arg0->numberInput, (s32)arg1 - 3) != -1) {
            arg0->numberInput.digits[arg0->numberInput.digitPos] = arg1 - 3;
            if (arg0->numberInput.digitPos < 2) {
                arg0->numberInput.digitPos++;
            }
            ov72_02238730(arg0->sprites[4], arg0->numberInput.digitPos, arg0->twoDigits);
            arg0->numberInput.cursor = arg1 - 3;
            arg0->numberInput.cursor = ov72_0223A8DC(&arg0->numberInput);
            ov72_022386F4(arg0->sprites[3], arg0->numberInput.cursor);
            PlaySE(0x5DC);
            {
                s32 idx = arg1 - 3;
                ov72_0223AED0(arg0, ov72_0223B478[idx * 2], ov72_0223B479[idx * 2], 3);
            }
        } else {
            PlaySE(0x5F2);
        }
        break;
    case 13:
        arg0->numberInput.digits[arg0->numberInput.digitPos] = -1;
        arg0->numberInput.digitPos--;
        if (arg0->numberInput.maxValue == 100) {
            if (arg0->numberInput.digitPos < 0) {
                arg0->numberInput.digitPos = 0;
            }
        } else if (arg0->numberInput.digitPos < 1) {
            arg0->numberInput.digitPos = 1;
        }
        PlaySE(0x5DC);
        ov72_02238730(arg0->sprites[4], arg0->numberInput.digitPos, arg0->twoDigits);
        Sprite_SetAnimCtrlSeq(arg0->sprites[5], 5);
        break;
    case 14:
        arg0->numberInput.cursor = arg1 - 3;
        ov72_022386F4(arg0->sprites[3], arg0->numberInput.cursor);
        arg0->numberInput.value = ov72_0223A7F4(&arg0->numberInput, -1);
        if (arg0->numberInput.value <= 0) {
            arg0->numberInput.value = 1;
        }
        arg0->numberInput.state = 7;
        PlaySE(0x5DC);
        Sprite_SetAnimCtrlSeq(arg0->sprites[6], 5);
        break;
    case 15:
        arg0->numberInput.cursor = arg1 - 3;
        arg0->numberInput.value = -1;
        ov72_022386F4(arg0->sprites[3], arg0->numberInput.cursor);
        arg0->numberInput.state = 7;
        PlaySE(0x5DC);
        Sprite_SetAnimCtrlSeq(arg0->sprites[7], 5);
        break;
    }
}

#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_72_wfc_connect.h"

void ov72_022386F4(Sprite *, s32);

u32 ov72_0223A760(s32, s32, u16 *);

s32 ov72_0223A8F0(NintendoWfcConnect *, s32);

s32 ov72_0223AA9C(NintendoWfcConnect *arg0);

s32 ov72_0223AA9C(NintendoWfcConnect *arg0) {
    if (0x40 & gSystem.newAndRepeatedKeys) {
        arg0->numberInput.cursor = ov72_0223A760(arg0->numberInput.cursor, 0, arg0->padColumn);
        ov72_022386F4(arg0->sprites[3], arg0->numberInput.cursor);
        PlaySE(0x5DCU);
    } else if (0x80 & gSystem.newAndRepeatedKeys) {
        arg0->numberInput.cursor = ov72_0223A760(arg0->numberInput.cursor, 1, arg0->padColumn);
        ov72_022386F4(arg0->sprites[3], arg0->numberInput.cursor);
        PlaySE(0x5DCU);
    } else if (0x20 & gSystem.newAndRepeatedKeys) {
        arg0->numberInput.cursor = ov72_0223A760(arg0->numberInput.cursor, 2, arg0->padColumn);
        ov72_022386F4(arg0->sprites[3], arg0->numberInput.cursor);
        PlaySE(0x5DCU);
    } else if (gSystem.newAndRepeatedKeys & 0x10) {
        arg0->numberInput.cursor = ov72_0223A760(arg0->numberInput.cursor, 3, arg0->padColumn);
        ov72_022386F4(arg0->sprites[3], arg0->numberInput.cursor);
        PlaySE(0x5DCU);
    } else if (1 & gSystem.newKeys) {
        ov72_0223A8F0(arg0, arg0->numberInput.cursor + 3);
    } else if (2 & gSystem.newKeys) {
        ov72_0223A8F0(arg0, 0xD);
    }
    return 0;
}

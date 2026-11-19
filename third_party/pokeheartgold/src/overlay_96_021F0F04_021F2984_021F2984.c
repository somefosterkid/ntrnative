#include "global.h"

#include "bg_window.h"

void ov96_021F2984(u32 arg0, BgConfig *arg1);

void ov96_021F2984(u32 arg0, BgConfig *bgConfig) {
    u32 y;
    u32 x;

    if ((arg0 >= 1U) && (arg0 <= 6U)) {
        x = ((arg0 - 1) * 2) + 0x12;
        y = 2;
    } else if ((arg0 >= 0x10U) && (arg0 <= 0x1CU)) {
        x = 0x1A - ((arg0 - 0x10) * 2);
        y = 0x14;
    } else if ((arg0 >= 0x26U) && (arg0 < 0x2BU)) {
        x = ((arg0 - 0x26) * 2) + 4;
        y = 2;
    } else if ((arg0 >= 7U) && (arg0 <= 0xFU)) {
        x = 0x1C;
        y = ((arg0 - 7) * 2) + 4;
    } else {
        x = 2;
        y = 0x12 - ((arg0 - 0x1D) * 2);
    }
    FillBgTilemapRect(bgConfig, 1U, 0U, x, (u8)(s32)y, 2U, 2U, 0x10U);
}

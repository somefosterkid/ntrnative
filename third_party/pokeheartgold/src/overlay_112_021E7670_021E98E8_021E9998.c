#include "global.h"

u8 ov112_021E98F8(void *, s32, s32);

void ov112_021E9934(u8 *, s32, s32, s32);

extern u8 *ov112_021FF504[];

void ov112_021E9998(void *arg0, void *arg1, u32 arg2, s32 arg3, s32 arg4);

void ov112_021E9998(void *arg0, void *arg1, u32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    s32 y;
    u8 **tblp;
    u8 *tbl;
    u8 j;
    s32 dx;
    u32 dy;
    s32 x;

    i = 0;
    tblp = ov112_021FF504;
    do {
        j = 0;
        tbl = *tblp;
        if (tbl[0] != 0xFF) {
            dx = (arg2 & 0xF) - 8;
            dy = (arg2 & 0xF0) >> 4;
            do {
                x = dx + (arg3 + tbl[j * 2]);
                y = dy + (arg4 + tbl[j * 2 + 1]) - 8;
                if (ov112_021E98F8(arg1, x, y) == 2) {
                    ov112_021E9934(arg0, x, y, 1);
                }
                j++;
            } while (tbl[j * 2] != 0xFF);
        }
        arg2 >>= 8;
        tblp++;
        i++;
    } while (i < 4);
}

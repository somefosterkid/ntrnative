#include "global.h"

u32 *ov96_021FBF90(u8 *, s32, s32);

void ov96_021FC0FC(u8 *arg0, s32 arg1, s32 arg2);

void ov96_021FC0FC(u8 *arg0, s32 arg1, s32 arg2) {
    s32 q;
    s32 r;
    s32 idx;

    q = arg1 / 8;
    if (q != 0) {
        r = arg1 - q * 8;
        idx = q - 1;
        if (idx < 0x200) {
            u8 *row = arg0 + arg2 * 0x200;
            if ((row + idx)[0x618] != 0) {
                s32 *p = (s32 *)(arg0 + 0xC18);
                if (p[arg2] < idx) {
                    p[arg2] = idx;
                    ov96_021FBF90(arg0, arg2, r);
                }
            }
        }
    }
}

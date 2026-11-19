#include "global.h"

typedef struct {
    u8 pad[0x14];
    u32 f14_a : 8;
    u32 f14_a1 : 8;
    u32 b : 8;
    u32 c : 6;
    u32 d : 2;
    u32 e : 11;
} UnkStruct_ov96_0220CD84;

void ov96_0220CD84(UnkStruct_ov96_0220CD84 *arg0, s32 arg1);

void ov96_0220CD84(UnkStruct_ov96_0220CD84 *arg0, s32 arg1) {
    s32 v;
    if (arg1 != 1) {
        if (arg1 == 2) {
            arg0->e = 0;
        }
    } else {
        v = arg0->b - 10;
        if (v > 100) {
            v = 100;
        } else if (v < 0) {
            v = 0;
        }
        arg0->b = v;
        arg0->d++;
        if (arg0->d >= 3) {
            arg0->d = 0;
        }
    }
    arg0->c = arg1;
    arg0->f14_a1 = 0;
}

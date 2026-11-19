#include "global.h"

s32 ov96_021F6524(s32 (*)[0x80], s32, s32, s32);

void ov96_021F6424(s32 (*arg0)[0x80]);

void ov96_021F64A0(s32 (*arg0)[0x80], s32 arg1, s32 *arg2);

void ov96_021F6424(s32 (*arg0)[0x80]) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;
    s32 v6;
    s32 (*v7)[0x80];

    v7 = arg0;
    ov96_021F6524(arg0, 0, 0, 1);
    ov96_021F6524(v7, 1, 1, 2);
    ov96_021F6524(v7, 2, 2, 0);
    v6 = 0;
    do {
        v1 = v7[1][v6];
        v7[0][v6] = v1;
        v0 = v7[2][v6];
        if (v1 > v0) {
            v7[0][v6] = v0;
        }
        v2 = v7[3][v6];
        if (v7[0][v6] > v2) {
            v7[0][v6] = v2;
        }
        v3 = v7[4][v6];
        if (v7[0][v6] > v3) {
            v7[0][v6] = v3;
        }
        v4 = v7[5][v6];
        if (v7[0][v6] > v4) {
            v7[0][v6] = v4;
        }
        v5 = v7[6][v6];
        if (v7[0][v6] > v5) {
            v7[0][v6] = v5;
        }
        v6 += 1;
    } while (v6 < 0x80);
}

void ov96_021F64A0(s32 (*arg0)[0x80], s32 arg1, s32 *arg2) {
    s32 *v7;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;
    s32 v8;
    s32 (*v6)[0x80];

    v6 = arg0;
    v7 = arg2;
    v8 = 0;
    do {
        *v7 = 0;
        if (1 & arg1) {
            v0 = v6[1][v8];
            if ((s32)*v7 > v0) {
                *v7 = v0;
            }
            v1 = v6[4][v8];
            if ((s32)*v7 > v1) {
                *v7 = v1;
            }
        }
        if (2 & arg1) {
            v2 = v6[2][v8];
            if ((s32)*v7 > v2) {
                *v7 = v2;
            }
            v3 = v6[5][v8];
            if ((s32)*v7 > v3) {
                *v7 = v3;
            }
        }
        if (arg1 & 4) {
            v4 = v6[3][v8];
            if ((s32)*v7 > v4) {
                *v7 = v4;
            }
            v5 = v6[6][v8];
            if ((s32)*v7 > v5) {
                *v7 = v5;
            }
        }
        v7 += 1;
        v8 += 1;
    } while (v8 < 0x80);
}

// libdwcutil's foundation.c (Black's ov016 0x021d5fe8-0x021d603c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct tagRECT {
    u16 l;
    u16 t;
    u16 r;
    u16 b;
} RECT;

// The prebuilt's DWCi_R_SCR, the whole screen as a touch rectangle. The prebuilt defines DWCi_A_SCR next to it (an
// AREA of the same bytes), which nothing uses and Black's link drops: the build's link keeps every global, so it is
// left out
const RECT data_ov016_021daa7c = { 0, 0, 256, 192 };

void DWCi_InputCoord(u16 param_1, u16 param_2, u16 *param_3);
void DWCi_InputRect(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u16 *param_5);

void DWCi_InputCoord(u16 x, u16 y, u16 *coord) {
    *coord = x;
    coord[1] = y;
}

void DWCi_InputRect(s32 l, s32 t, s32 r, s32 b, u16 *rect) {
    *rect = l;
    rect[1] = t;
    rect[2] = r;
    rect[3] = b;
}

void func_ov016_021d600c(s16 *param_1, const s16 *param_2, s16 *param_3) {
    u16 val;
    u16 val2;
    u16 val3;
    u16 val4;

    val = *param_1;
    val2 = param_1[1];
        *param_3 = val;
    val3 = *param_2;
    val4 = param_2[1];
    param_3[1] = val2;
    param_3[2] = val + val3;
    param_3[3] = val2 + val4;
}

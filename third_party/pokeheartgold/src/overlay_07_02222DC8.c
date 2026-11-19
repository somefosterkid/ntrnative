#include "global.h"

void ov07_02222DC8(s16 x1, s16 y1, s16 x2, s16 y2, s16 *mx, s16 *my);

void ov07_02222DE4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4);

void ov07_02222E0C(s16 x1, s16 y1, s16 x2, s16 y2, u16 *angle);

void ov07_02222DC8(s16 x1, s16 y1, s16 x2, s16 y2, s16 *mx, s16 *my) {
    *mx = (x1 + x2) / 2;
    *my = (y1 + y2) / 2;
}

void ov07_02222DE4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    s16 v0;
    s16 v1;

    v1 = 0 - (arg1 - arg3);
    v0 = arg0 - arg2;
    *arg4 = FX_Sqrt(((v1 * v1) + (v0 * v0)) << 0xC);
}

void ov07_02222E0C(s16 x1, s16 y1, s16 x2, s16 y2, u16 *angle) {
    s16 dx = x1 - x2;
    s16 ndy = -(y1 - y2);

    *angle = FX_Atan2Idx(ndy * ((fx32)0x0000000000001000L), dx * ((fx32)0x0000000000001000L));

    if (*angle > 0 && ndy < 0) {
        *angle = (*angle - (((180) * 0xFFFF) / 360)) * 0xFFFF;
    }
}

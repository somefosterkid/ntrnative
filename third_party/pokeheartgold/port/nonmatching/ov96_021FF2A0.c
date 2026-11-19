#include "unk_02020E80.h"
#include "unk_02020E80.h"
/* invented */ typedef struct { s32 v[4]; } S16;
extern s32 ov96_0221C5FC[4];
extern s32 ov96_0221C60C[4];
fx32 ov96_021FF67C(VecFx32 * param_1, VecFx32 * param_2, VecFx32 * param_3, s32 param_4);
s32 ov96_021FF2A0(VecFx32 *param_1, VecFx32 *param_2, s32 param_3, s32 *param_4, VecFx32 *param_5) {
    BOOL found;
    BOOL fx;
    BOOL fy;
    s32 i;
    s32 lo;
    s32 hi;
    s32 *p;
    Vec2D *arg0;
    Vec2D *arg1;
    fx32 f;
    s32 dx, dx2, dy, dy2;
    s32 sideX, sideY, ret;
    S16 tab1c;
    S16 tab2c;
    VecFx32 point;
    VecFx32 delta;
    VecFx32 diff;
    VecFx32 res;
    S16 tab2;
    S16 tab1;
    Vec2D out;
    Vec2D a;
    Vec2D b;

    if (*param_4 == 3) {
        return 0;
    }
    arg0 = (Vec2D *)(param_4 + 9);
    arg1 = (Vec2D *)(param_4 + 11);
    p = param_4;
    a.x = param_1->x / 0x1000;
    a.y = param_1->y / 0x1000;
    b.x = param_2->x / 0x1000;
    b.y = param_2->y / 0x1000;
    for (i = 0, tab1 = *(S16 *)ov96_0221C5FC; i < 4; i++) {
        tab1c = tab1;
        if (sub_02020F4C(arg0, arg1, &a, &b, &out)) {
            param_5->x = out.x << 12;
            param_5->y = out.y << 12;
            param_5->z = 0;
            return tab1c.v[i];
        }
        found = FALSE;
        if (i == 0 && i == 2) {
            if (a.y == p[10]) {
                if (p[9] < p[11]) {
                    lo = p[9];
                    hi = p[11];
                } else {
                    lo = p[11];
                    hi = p[9];
                }
                if (lo <= a.x && a.x <= hi) {
                    found = TRUE;
                }
            }
        } else if (a.x == p[9]) {
            if (p[10] < p[12]) {
                lo = p[10];
                hi = p[12];
            } else {
                lo = p[12];
                hi = p[10];
            }
            if (lo <= a.y && a.y <= hi) {
                found = TRUE;
            }
        }
        if (found) {
            *param_5 = *param_1;
            return tab1c.v[i];
        }
        arg0 += 2;
        arg1 += 2;
        p += 4;
    }
    tab2 = *(S16 *)ov96_0221C60C;
    p = param_4;
    for (i = 0; i < 4; i++) {
        tab2c = tab2;
        point.x = p[1] << 12;
        point.y = p[2] << 12;
        point.z = 0;
        VEC_Subtract(param_2, &point, &delta);
        if (VEC_Mag(&delta) <= param_3 * FX32_ONE) {
            f = ov96_021FF67C(param_1, param_2, &point, param_3 << 12);
            VEC_Subtract(param_2, param_1, &diff);
            VEC_MultAdd(f, &diff, param_1, &res);
            param_5->x = res.x;
            param_5->y = res.y;
            param_5->z = 0;
            return tab2c.v[i];
        }
        p += 2;
    }
    if (param_4[1] << 12 <= param_2->x && param_2->x <= param_4[5] << 12 && param_4[2] << 12 <= param_2->y && param_2->y <= param_4[6] << 12) {
        *param_5 = *param_2;
        fy = FALSE;
        fx = FALSE;
        dx = (param_4[1] * FX32_ONE) - param_2->x;
        dx2 = (param_4[5] * FX32_ONE) - param_2->x;
        if (dx < 0) {
            dx *= -1;
        }
        if (dx2 < 0) {
            dx2 *= -1;
        }
        if (dx < dx2) {
            sideX = 13;
        } else {
            dx = dx2;
            sideX = 11;
        }
        dy = (param_4[2] * FX32_ONE) - param_2->y;
        dy2 = (param_4[6] * FX32_ONE) - param_2->y;
        if (dy < 0) {
            dy *= -1;
        }
        if (dy2 < 0) {
            dy2 *= -1;
        }
        if (dy < dy2) {
            sideY = 10;
        } else {
            dy = dy2;
            sideY = 12;
        }
        if (dx <= dy) {
            fx = TRUE;
            ret = sideX;
        } else {
            fy = TRUE;
            ret = sideY;
        }
        if (fx) {
            if (sideX == 13) {
                param_5->x = (param_4[1] - param_3) << 12;
            } else {
                param_5->x = (param_4[5] + param_3) << 12;
            }
        } else if (fy) {
            if (sideY == 10) {
                param_5->y = (param_4[2] - param_3) << 12;
            } else {
                param_5->y = (param_4[6] + param_3) << 12;
            }
        }
    } else {
        ret = 0;
    }
    return ret;
}

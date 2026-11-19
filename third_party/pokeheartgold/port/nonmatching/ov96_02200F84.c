#include "error_handling.h"
#include "math_util.h"
s32 ov96_021E8228(s32, u8, u8, s32, s32);
s32 ov96_021EB03C(s32, s32, s32, s32 *, s32 *);
s32 ov96_021EB0A4(s32, s32, s32, s32 *, s32 *);
s32 ov96_0220144C(void *, s32);
void *ov96_0220146C(void *);
extern s32 ov96_0221C770[];

#define FLD(p, T, o) (*(T *)((u8 *)(p) + (o)))

/* invented */
typedef struct E16 { void *obj; s32 b; s32 x; s32 y; } E16;
typedef struct E20 { s32 a, b, c, d, e; } E20;
typedef struct T20 { s32 a, b, c, d; s16 e; s16 f; } T20;
typedef struct Top {
    E16 v[4];
    E20 w[30];
    u8 lane[4];
    T20 t[16];
    u8 pad[8];
    u8 lim;
} Top;

void ov96_02200F84(s32 arg0, Top *top, u32 idx, s32 arg3) {
    E20 ws[30];
    E16 vs[4];
    s32 sp4C[4];
    s32 sp48, sp44, sp40, sp3C, sp38, sp34;
    s32 score;
    E16 *cur;
    s32 i, j, k, y;
    void *obj;
    void *r;

    obj = top->v[idx].obj;
    if (FLD(obj, u8, 0x9D) != 0 || FLD((u8 *)obj + 0x1C * FLD(obj, u8, 0x8B), u8, 0x30) == 2 || FLD(obj, u8, 0xA9) != 0) {
        return;
    }
    score = 0;
    for (i = 0; i < 4; i++) {
        sp4C[i] = score;
    }
    for (i = 0; i < 4; i++) {
        vs[i] = top->v[i];
        obj = vs[i].obj;
        ov96_021EB0A4(FLD(obj, s32, FLD(obj, u8, 0x8B) * 4), FLD(obj, s32, 0x7C) / 0x1000, FLD(obj, s32, 0x80) / 0x1000, &sp48, &sp44);
        ov96_021EB03C(FLD(obj, s32, FLD(obj, u8, 0x8B) * 4), sp48 << 12, sp44 << 12, &sp40, &sp3C);
        vs[i].x = sp40 / 0x1000;
        vs[i].y = sp3C / 0x1000;
        top->lane[i] = (vs[i].y - 0x20) / 40;
        if (top->lane[i] > 4) {
            GF_AssertFail();
        }
    }
    cur = &vs[idx];
    for (i = 0; i < 30; i++) {
        ws[i] = top->w[i];
    }
    if (cur->x >= 0x360) {
        for (i = 0; i < 4; i++) {
            ov96_0220144C(&vs[i].x, -0xA0);
        }
        for (i = 0; i < 30; i++) {
            ov96_0220144C(&ws[i].b, 0x50);
        }
    }
    for (i = 0; i < 16; i++) {
        s32 m, q;
        top->t[i].f = i;
        m = (i % 4) * 0x28;
        top->t[i].a = cur->x + 0x14 + m;
        q = (i / 4) * 0x28;
        top->t[i].b = q + 0x34;
        top->t[i].c = cur->x + m + LCRandom() % 40;
        top->t[i].d = q + 0x20 + LCRandom() % 40;
    }
    for (i = 0; i < 16; i++) {
        top->t[i].e = ov96_0221C770[i];
    }
    y = 0x20;
    for (k = 0; k < 4; k++) {
        for (j = 0; j < 4; j++) {
            if (j != idx) {
                s32 d;
                if (cur->x + 0xA0 >= vs[j].x) {
                    d = vs[j].x - cur->x;
                    if (d >= 0 && d < 0xA0) {
                        u8 t;
                        if (k == top->lane[j]) {
                            if (k == top->lane[idx]) {
                                sp4C[k] -= 4;
                            } else {
                                sp4C[k] -= 2;
                            }
                        }
                        t = d / 40;
                        if (t >= 4) {
                            GF_AssertFail();
                        } else {
                            top->t[k * 4 + t].e -= 3;
                        }
                    } else if (d >= -0x50 && k == top->lane[j] && k == top->lane[idx]) {
                        sp4C[k] += 2;
                    }
                }
            }
        }
        for (j = 0; j < 30; j++) {
            if (ws[j].a == 0) {
                break;
            }
            if (cur->x + 0xA0 >= ws[j].b) {
                s32 d = ws[j].b - cur->x;
                if (d > 0 && d <= 0xA0) {
                    s32 c = ws[j].c;
                    if (y < c + ws[j].e && c < y + 0x28) {
                        if (ws[j].d == 3) {
                            sp4C[j] -= 1; /* sic: indexed by the obstacle j (0..29), not the lane: the original writes past sp4C[3] into the next stack object */
                        } else {
                            sp4C[j] -= 6;
                        }
                    }
                }
            }
        }
        y += 0x28;
    }
    for (i = 0; i < 16; i++) {
        top->t[i].e += sp4C[i / 4];
    }
    if (arg3 < 0xA8C) {
        for (j = 0, i = 0; j < 16; j += 4) {
            top->t[j + 2].e += 3;
            top->t[j + 3].e += 3;
        }
    }
    obj = cur->obj;
    if (FLD((u8 *)obj + 0x1C * FLD(obj, u8, 0x8B), f32, 0x24) <= 20.0f) {
        for (j = 0, i = 0; j < 16; j += 4) {
            top->t[j + 2].e -= 2;
            top->t[j + 3].e -= 2;
        }
        score += 8;
    }
    obj = cur->obj;
    if (FLD((u8 *)obj + 0x1C * (u8)((FLD(obj, u8, 0x8B) + 1) % 3), u8, 0x30) == 1) {
        score -= 5;
    }
    r = ov96_0220146C(top);
    if (FLD(r, s16, 0x10) >= score) {
        if (idx >= top->lim) {
            ov96_021EB0A4(FLD(cur->obj, s32, FLD(cur->obj, u8, 0x8B) * 4), cur->x, cur->y, &sp38, &sp34);
            FLD(cur->obj, s32, 0xB0) = sp38 << 12;
            FLD(cur->obj, s32, 0xB4) = sp34 << 12;
            FLD(cur->obj, s32, 0xB8) = 0;
            FLD(cur->obj, s32, 0xBC) = FLD(r, s32, 8) << 12;
            FLD(cur->obj, s32, 0xC0) = FLD(r, s32, 0xC) << 12;
            FLD(cur->obj, s32, 0xC4) = 0;
            FLD(cur->obj, u8, 0xAA) = 1;
        }
    } else if (idx >= top->lim) {
        FLD(cur->obj, u8, 0x9D) = 1;
        FLD(cur->obj, u8, 0x9E) = 1;
        ov96_021E8228(arg0, FLD(cur->obj, u8, 0xD0), FLD(cur->obj, u8, 0x8B), 7, 1);
    }
}

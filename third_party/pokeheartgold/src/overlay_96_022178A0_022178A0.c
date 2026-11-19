#include "global.h"

#include "error_handling.h"

#include "overlay_96_ov96_rd_obj.h"

void ov96_021E8228(struct PokeathlonCourseData *data, u8 a, u8 b, s32 c, s32 d);

s32 ov96_02215958(void *, s32);

s32 ov96_0221785C(s32, s32, VecFx32 *);

void ov96_02217868(Ov96RdObj *obj, VecFx32 *dir);

s32 ov96_02217890(s32);

void ov96_02219174(Ov96RdObj *obj, s32 amount);


s32 ov96_022178A0(Ov96RdObj *arg0, Ov96RdObj *arg1, VecFx32 *arg2);

s32 ov96_022178A0(Ov96RdObj *arg0, Ov96RdObj *arg1, VecFx32 *arg2) {
    s32 v7;
    s32 v6;
    s32 ok;
    s32 n;
    u32 r;
    Ov96RdStats *w;

    w = arg1->info->stats;
    v7 = w->s0;
    v6 = w->s1;
    if (ov96_0221785C(arg0->state, arg1->state, arg2) != 0) {
        if (arg1->state == 6) {
            v7 = (s32)(3.0f * (f32)v7);
            v6 = (s32)(2.0f * (f32)v6);
            n = arg1->cnt + 1;
            if (n > 200) {
                n = 200;
            }
            arg1->cnt = n;
            ov96_021E8228(arg1->data, arg1->d, arg1->e, 3, 1);
            ov96_021E8228(arg1->data, arg1->d, arg1->e, 4, 1);
        } else if (arg1->state == 11) {
            w = arg1->info->stats;
            v7 = w->s4;
            v6 = w->s5;
            n = arg1->cnt + 2;
            if (n > 200) {
                n = 200;
            }
            arg1->cnt = n;
            ov96_021E8228(arg1->data, arg1->d, arg1->e, 3, 1);
            ov96_021E8228(arg1->data, arg1->d, arg1->e, 4, 1);
        } else {
            v7 = 0x3000;
        }
        VEC_MultAdd(v7, arg2, &arg0->vel, &arg0->vel);
        ov96_02215958(&arg0->vel, 0xC000);
        ov96_02217868(arg1, arg2);
        if (ov96_02217890(arg1->state) != 0) {
            arg0->b5D = 4;
            arg1->b5D = 4;
            ov96_02219174(arg0, v6);
        }
        if (arg0->link == NULL || ov96_02217890(arg1->state) != 0) {
            arg0->link = arg1;
            if (arg1->link != NULL && arg1->link != arg0) {
                arg0->link = arg1->link;
            }
        }
        arg0->f27 = 1;
        arg0->lastState = arg1->state;
        if (ov96_02217890(arg1->state) != 0) {
            if (arg0->mode != 2) {
                switch (arg1->state) {
                case 6:
                    arg0->mode = 1;
                    break;
                case 11:
                    arg0->mode = 2;
                    break;
                }
            }
            r = arg1->d;
            if (r < arg0->d) {
                r++;
            }
            arg0->a = r;
            arg0->w64 = 4;
        } else {
            arg0->f31 = 1;
        }
        ok = 0;
        if (arg1->state != 3 && arg1->state != 4) {
            ok = 1;
        }
        if (ok == 0) {
            GF_AssertFail();
        }
        ok = 1;
        r = arg0->state - 3;
        if (r <= 8 && (0x103 & (1 << r))) {
            ok = 0;
        }
        if (ok == 0) {
            GF_AssertFail();
        }
        return 1;
    }
    return 0;
}

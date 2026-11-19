#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

#include "overlay_96_ov96_rd_obj.h"

void ov96_02215944(VecFx32 *param_1, fx32 param_2, fx32 param_3);

u32 ov96_022186CC(VecFx32 *param_1);

void ov96_02218784(void *param_1, u8 param_2);

void ov96_022187A8(Ov96RdObj *p, void *param_2);

void ov96_022187A8(Ov96RdObj *p, void *param_2) {
    VecFx32 diff;
    VecFx32 dir;
    VecFx32 step;
    VecFx32 res;
    fx32 mag;
    fx32 speed;
    fx32 a;
    BOOL moving;

    VEC_Subtract(&p->pos20, &p->pos2C, &diff);
    mag = VEC_Mag(&diff);
    moving = mag > 0;
    if (p->f27) {
        if (p->lastState == 0) {
            GF_AssertFail();
        }
        if (p->lastState == 6 || p->lastState == 9) {
            a = 0x1000;
        } else {
            a = 0xB33;
        }
        if (p->timer58 <= 0) {
            VecFx32 norm = { 0, 0, 0 };
            VecFx32 zero = { 0, 0, 0 };
            VEC_Normalize(&p->vel, &norm);
            VEC_MultAdd(a, &norm, &zero, &res);
            res.x = res.x < 0 ? -res.x : res.x;
            res.y = res.y < 0 ? -res.y : res.y;
            ov96_02215944(&p->vel, res.x, res.y);
        }
        VEC_Add(&p->pos2C, &p->vel, &p->pos2C);
        p->pos20 = p->pos2C;
        if (p->vel.x != 0 || p->vel.y != 0) {
            return;
        }
        ov96_02218784(param_2, p->d);
        p->link = NULL;
        p->f27 = 0;
    }
    if (!(p->f28) && p->timer58 <= 0 && moving) {
        VEC_Normalize(&diff, &dir);
        if (!(p->f27)) {
            p->f16 = ov96_022186CC(&dir);
        }
        speed = p->info->stats->s3;
        if (p->info->value < 0x1E000) {
            speed = (fx32)(((s64)speed * 0x800 + 0x800) >> 12);
        }
        if (mag <= speed) {
            step = diff;
        } else {
            VecFx32 zero2 = { 0, 0, 0 };
            VEC_MultAdd(speed, &dir, &zero2, &step);
        }
        VEC_Add(&p->pos2C, &step, &p->pos2C);
        p->dir = dir;
    }
}

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

u32 ov96_0220D6CC(VecFx32 *arg0);

void ov96_0220E960(VecFx32 *param_1, s32 param_2, s32 param_3);

void ov96_0220D744(UnkStruct_ov96_0220E3F0_Unit *p, PokeathlonCourseData *course);

void ov96_0220D744(UnkStruct_ov96_0220E3F0_Unit *p, PokeathlonCourseData *course) {
    fx32 mag;
    fx32 fVar2;
    VecFx32 delta;
    VecFx32 dir;
    VecFx32 step;

    if (p->state != 0) {
        VEC_Subtract(&p->unk_10, &p->unk_1C, &delta);
        mag = VEC_Mag(&delta);
        p->unk_40.bits.unk_28 = (mag > 0);
        if (p->unk_40.bits.unk_26) {
            VecFx32 zero2 = { 0, 0, 0 };
            VecFx32 zero1 = { 0, 0, 0 };
            VecFx32 absA;
            VecFx32 absB;

            VEC_Normalize(&p->unk_34, &zero2);
            VEC_MultAdd(0xccd, &zero2, &zero1, &absA);
            absA.x = MATH_ABS(absA.x);
            absA.y = MATH_ABS(absA.y);
            if (p->unk_40.bits.unk_27) {
                fVar2 = p->cell->unk_4 / 100;
                VEC_Normalize(&delta, &dir);
                VEC_MultAdd(fVar2, &p->unk_28, &zero1, &absB);
                absA.x += MATH_ABS(absB.x);
                absA.y += MATH_ABS(absB.y);
            }
            ov96_0220E960(&p->unk_34, absA.x, absA.y);
            VEC_Add(&p->unk_1C, &p->unk_34, &p->unk_1C);
            p->unk_10 = p->unk_1C;
            if (p->unk_34.x != 0) {
                return;
            }
            if (p->unk_34.y != 0) {
                return;
            }
            p->unk_40.bits.unk_26 = 0;
        }
        if (p->unk_40.bits.unk_28) {
            VEC_Normalize(&delta, &dir);
            if (!p->unk_40.bits.unk_26) {
                p->unk_40.bits.unk_20 = ov96_0220D6CC(&dir);
            }
            fVar2 = p->cell->unk_8;
            if (mag <= fVar2) {
                step = delta;
            } else {
                VecFx32 z3 = { 0, 0, 0 };
                VEC_MultAdd(fVar2, &dir, &z3, &step);
            }
            VEC_Add(&p->unk_1C, &step, &p->unk_1C);
            p->unk_28 = dir;
        }
    }
}

#include "global.h"

#include "overlay_96_ov96_arena_work.h"

s32 ov96_021EAF78(void *, s32, s32, VecFx32 *, s32 *, s32 *);

s32 ov96_021F218C(Ov96ArenaMon *arg0, Ov96ArenaMon *arg1);

s32 ov96_021F218C(Ov96ArenaMon *arg0, Ov96ArenaMon *arg1) {
    VecFx32 v0;
    VecFx32 a;
    VecFx32 b;
    s32 v2;
    int new_var;
    s32 v1;
    ov96_021EAF78(arg0->spr, arg0->pos.x, arg0->pos.y, &a, &a.y, &v2);
    ov96_021EAF78(arg1->spr, arg1->pos.x, arg1->pos.y, &b, &b.y, &v1);
    new_var = (v2 + v1) << 0xC;
    a.z = 0;
    b.z = 0;
    VEC_Subtract(&a, &b, &v0);
    if (VEC_Mag(&v0) <= ((s32)new_var)) {
        return 1;
    }
    return 0;
}

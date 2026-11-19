#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "math_util.h"

#include "overlay_96_app_02210C98.h"

s32 ov96_02212F0C(Ov96App_02210C98_Mover *, Ov96App_02210C98_Mover *);

s32 ov96_0221341C(VecFx32 *, s32, VecFx32 *, s32);

s32 ov96_02213444(Ov96App_02210C98_Mover *);

s32 ov96_022134D4(Ov96App_02210C98 *, Ov96App_02210C98_Mover *, VecFx32 *);

u32 ov96_02213728(VecFx32 *, VecFx32 *, s32, s32, VecFx32 *);

s32 ov96_02213E60(Ov96App_02210C98 *, Ov96App_02210C98_Mover *);

extern u8 ov96_0221D514[][30];

extern u8 ov96_0221D5AA[][30];

extern u8 ov96_0221D640[];

void ov96_02212B94(PokeathlonCourseData *arg0);

void ov96_02212B94(PokeathlonCourseData *arg0) {
    VecFx32 sp1C;
    Ov96App_02210C98 *base;
    Ov96App_02210C98_Mover *e;
    s32 i;
    u8 lim;
    s32 v;
    s32 r;
    u8 c;
    u8 w;
    Ov96App_02210C98_Mover *e0;
    Ov96App_02210C98_Mover *e1;
    u8 t;

    base = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    if (base->unk_62C[1].unk_38 == 0 && base->unk_738 <= 0x384) {
        base->unk_62C[1].unk_39 = 3;
        base->unk_62C[1].unk_38 = 1;
    }
    e = &base->unk_62C[0];
    for (i = 0; i < 2; i++, e++) {
        if (e->unk_38 == 0) {
            continue;
        }
        if (e->unk_39 == 3) {
            e->unk_43 = e->unk_43 + 1;
            if (e->unk_43 >= 8) {
                e->unk_43 = 0;
                e->unk_39 = 0;
            }
        } else if (e->unk_39 == 1) {
            e->unk_3A = e->unk_3A - 1;
            if (e->unk_3A == 0) {
                ov96_02213444(e);
                ov96_02213E60(base, e);
                if (i == 0) {
                    v = base->unk_738;
                    if (v >= 0x4B0) {
                        lim = 0;
                    } else if (v >= 0x4B0 / 2) {
                        lim = 5;
                    } else if (v >= 0x12C) {
                        lim = 10;
                    } else {
                        lim = 25;
                    }
                    lim += base->unk_743;
                    if (lim > 100) {
                        lim = 100;
                    }
                    if (LCRandom() % 100 < lim) {
                        e->unk_38 = 2;
                        base->unk_743 = 0;
                    } else {
                        e->unk_38 = 1;
                        if (base->unk_743 < 100) {
                            base->unk_743 += 1;
                        }
                    }
                } else {
                    e->unk_38 = 1;
                }
            }
        } else if (e->unk_39 == 0 || e->unk_39 == 2) {
            v = e->unk_40 << 12;
            if (v != 0) {
                sp1C = e->unk_8;
                e->unk_8.x = e->unk_8.x + e->unk_14.x;
                e->unk_8.y = e->unk_8.y + e->unk_14.y;
                if (v >= ov96_0221D640[e->unk_46] << 12) {
                    if (e->unk_3E == 2) {
                        e->unk_39 = 2;
                    } else {
                        e->unk_39 = 0;
                    }
                } else {
                    e->unk_39 = 0;
                    e->unk_45 = 0;
                }
                {
                    VecFx32 sp10 = { 0, 0, 0 };
                    VecFx32 sp4;
                    c = e->unk_3C;
                    t = e->unk_3E;
                    if (t == 2) {
                        w = ov96_0221D514[e->unk_41][c];
                    } else if (t == 1) {
                        w = ov96_0221D5AA[e->unk_41][c];
                    } else {
                        GF_AssertFail();
                        w = 0;
                    }
                    e->unk_40 = w;
                    ov96_022134D4(base, e, &e->unk_20);
                    e->unk_3C = e->unk_3C + 1;
                    if (e->unk_3C >= e->unk_3D) {
                        e->unk_14.x = 0;
                        e->unk_14.y = 0;
                        e->unk_20 = sp10;
                        e->unk_3E = 0;
                        e->unk_40 = 0;
                    } else if (e->unk_40 == 0) {
                        e->unk_20 = sp10;
                        e->unk_3C = 0;
                        e->unk_3D = 0;
                        e->unk_3E = 0;
                    }
                    e->unk_48 = 0;
                    r = ov96_02213728(&e->unk_8, &sp1C, 8, 0, &sp4);
                    if (r != 0) {
                        e->unk_8 = sp4;
                        e->unk_48 = 1;
                    }
                    switch (r) {
                    case 3:
                    case 4:
                        e->unk_20.y *= ~0;
                        e->unk_14.y *= ~0;
                        break;
                    case 1:
                    case 2:
                        e->unk_20.x *= ~0;
                        e->unk_14.x *= ~0;
                        break;
                    }
                }
            } else {
                e->unk_39 = 0;
                e->unk_45 = 0;
                e->unk_46 = 0;
                e->unk_3E = 0;
            }
        }
    }
    if (base->unk_62C[0].unk_38 == 0 || base->unk_62C[1].unk_38 == 0) {
        return;
    }
    e0 = &base->unk_62C[0];
    e1 = &base->unk_62C[1];
    if (e0->unk_39 != 1 && e0->unk_39 != 3 && e1->unk_39 != 1 && e1->unk_39 != 3) {
        if (e0->unk_38 == 1) {
            v = 8;
        } else if (e0->unk_38 == 2) {
            v = 12;
        } else {
            GF_AssertFail();
            return;
        }
        if (ov96_0221341C(&e0->unk_8, v << 12, &e1->unk_8, 2 << 14) != 0) {
            ov96_02212F0C(e0, e1);
        }
    }
}

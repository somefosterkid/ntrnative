#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

s32 ov96_021EAF8C(void *);

s32 ov96_021EB06C(void *, s32, s32, s32 *, s32 *);

s32 ov96_0220FBEC(UnkStruct_ov96_0220E3F0_Record *arg0, UnkStruct_ov96_0220E3F0_Entry *arg1, UnkStruct_ov96_0220E3F0_Hit *arg2);

s32 ov96_0220FCB0(s16 *arg0, s32 arg1);

s32 ov96_0220FBEC(UnkStruct_ov96_0220E3F0_Record *arg0, UnkStruct_ov96_0220E3F0_Entry *arg1, UnkStruct_ov96_0220E3F0_Hit *arg2) {
    VecFx32 *new_var;
    VecFx32 v;
    s32 v3;
    s32 v2;
    s32 v1;
    s32 v0;
    UnkStruct_ov96_0220E3F0_Hit *v6;
    UnkStruct_ov96_0220E3F0_Entry *v5;
    UnkStruct_ov96_0220E3F0_Record *v4;
    s16 v8;
    s32 v7;
    s32 v9;
    s32 v10;
    s32 v11;
    s32 v13;
    s32 v14;
    UnkStruct_ov96_0220E3F0_Unit *v12;
    v5 = arg1;
    v8 = arg0->unk_2;
    v4 = arg0;
    v6 = arg2;
    new_var = &v;
    if (((s32)v8) < 0x120) {
        return 0;
    }
    v14 = 0;
    v0 = (s32)((s16)(v8 - 0x120));
loop_3:
    new_var->x = 0;

    new_var->y = 0;
    new_var->z = 0;
    v9 = v14 % 3;
    v7 = v14 / 3;
    v1 = v7;
    v12 = &v5[v7].units[v9];
    if (v12->state == 1) {
        ov96_021EB06C(v12->unk_4, v12->unk_1C.x >> 0xC, v12->unk_1C.y >> 0xC, &v3, &v2);
        v11 = v3 - v4->unk_0;
        v.x = v11;
        v10 = v2 - v0;
        v.x = v11 << 0xC;
        v.y = v10;
        v.y = v10 << 0xC;
        v13 = VEC_Mag(&v);
        if (v13 <= ((s32)((ov96_021EAF8C(v12->unk_4) + 0xE) << 0xC))) {
            v6->entry = (s8)v1;
            v6->unit = (s8)v9;
            v6->x = (s16)v3;
            v6->y = (s16)v2;
            return 1;
        }
    }
    v14 += 1;
    if (v14 >= 0xC) {
        return 0;
    }
    goto loop_3;
}

s32 ov96_0220FCB0(s16 *arg0, s32 arg1) {
    s16 v0;
    s16 v1;
    s16 v2;
    s16 v3;
    s32 v4;
    s32 v5;
    s32 v6;
    s32 v7;
    s32 v8;
    s32 v9;
    s32 v10;
    s32 v11;

    v2 = arg0[1];
    v5 = 1;
    v6 = 1;
    v8 = 0;
    if (v2 == (arg1 - 1)) {
        v3 = arg0[0];
        v10 = 1;
        if (((s32)v3 >= 8) && ((s32)v3 <= 0x17)) {
            v10 = 0;
        }
        if (v10 != 0) {
            v8 = 1;
        }
    }
    if (v8 == 0) {
        v9 = 0;
        if (v2 == (arg1 - 2)) {
            v1 = arg0[0];
            v11 = 1;
            if (((s32)v1 >= 5) && ((s32)v1 <= 0x1A)) {
                v11 = 0;
            }
            if (v11 != 0) {
                v9 = 1;
            }
        }
        if (v9 == 0) {
            v6 = 0;
        }
    }
    if (v6 == 0) {
        v7 = 0;
        if (v2 == (arg1 - 3)) {
            v0 = arg0[0];
            v4 = 1;
            if (((s32)v0 >= 2) && ((s32)v0 <= 0x1D)) {
                v4 = 0;
            }
            if (v4 != 0) {
                v7 = 1;
            }
        }
        if (v7 == 0) {
            v5 = 0;
        }
    }
    return v5;
}

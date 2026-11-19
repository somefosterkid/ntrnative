#include "global.h"

#include "screen_fade_fx.h"

UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *, s32);

s32 sub_02011A44(s32, s32, s32, s32, s32 *, s32 *);

void sub_02011AD8(UnkStruct_02011AFC *arg0);

void sub_02011AD8(UnkStruct_02011AFC *arg0) {
    s32 v1;
    s32 v0;
    s32 v3;
    s32 v4;
    s32 v5;
    UnkStruct_02010FC0 *v2;

    v2 = sub_02010EE0(&arg0->unk_00, 0);
    v5 = 0;
    do {
        v4 = arg0->unk_0C.unk_08;
        if (v5 <= v4) {
            sub_02011A44(arg0->unk_0C.unk_00, arg0->unk_0C.unk_04, v4, v5, &v1, &v0);
        } else {
            v3 = v4 * 2;
            if (v5 <= v3) {
                v1 = v2->unk_300[0][v3 - v5];
                v0 = v2->unk_300[1][(arg0->unk_0C.unk_08 * 2) - v5];
            } else {
                sub_02011A44(arg0->unk_0C.unk_00, arg0->unk_0C.unk_04, v4, v5, &v1, &v0);
            }
        }
        v2->unk_300[0][v5] = v1;
        v2->unk_300[1][v5] = v0;
        v5 += 1;
    } while (v5 < 0xC0);
}

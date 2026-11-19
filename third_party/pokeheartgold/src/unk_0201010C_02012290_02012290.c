#include "global.h"

#include "screen_fade_fx.h"

s32 sub_02010A8C(void *, void *);

void sub_02012290(UnkStruct_0201184C *arg0);

void sub_02012290(UnkStruct_0201184C *arg0) {
    UnkStruct_0201184C_sub1 *v0;
    s32 v2;

    v2 = 0;
    if (arg0->unk_10 > 0) {
        do {
            v0 = arg0->unk_0C;
            sub_02010A8C(&v0[v2].unk_00, &v0[v2].unk_10);
            v2 += 1;
        } while (v2 < arg0->unk_10);
    }
}

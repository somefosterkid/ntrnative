#include "global.h"

s32 sub_02054E00(s32);

extern u16 _020FC5FC;

s32 sub_02054E20(s32 arg0);

s32 sub_02054E20(s32 arg0) {
    s32 v0;
    u16 *v1;

    if (sub_02054E00(arg0) == 0) {
        return 0;
    }
    v1 = &_020FC5FC;
    v0 = 0;
loop_3:
    if (arg0 == *v1) {
        return 1;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 3) {
        return 0;
    }
    goto loop_3;
}

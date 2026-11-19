#include "global.h"

#include "unk_02069660.h"

void sub_02069660(Simple3DModel *model) {
    u32 v1;
    s8 *v0;

    v0 = (s8 *)model;
    v1 = sizeof(Simple3DModel);
    do {
        *v0 = 0;
        v0 += 1;
        v1 -= 1;
    } while (v1 != 0);
}

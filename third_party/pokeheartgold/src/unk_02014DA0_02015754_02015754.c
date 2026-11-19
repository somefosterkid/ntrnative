#include "global.h"

void *sub_02015550(void *, s32);

void sub_02015754(void *arg0, s16 *arg1);

void sub_02015754(void *arg0, s16 *arg1) {
    void *v0;

    v0 = sub_02015550(arg0, 5);
    if (v0 != NULL) {
        *(s16 *)((u8 *)v0 + 0xC) = (s16)*arg1;
    }
}

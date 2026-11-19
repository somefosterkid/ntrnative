#include "global.h"

void *ov41_02248D7C(void *, s32);

void ov41_02248D64(void *arg0, void *arg1, s32 arg2);

void ov41_02248D64(void *arg0, void *arg1, s32 arg2) {
    void *v0;

    v0 = ov41_02248D7C(arg1, arg2);
    *(void **)v0 = arg0;
    *(s32 *)((u8 *)v0 + 4) = 3;
    *(s32 *)((u8 *)v0 + 8) = 0x2C;
}

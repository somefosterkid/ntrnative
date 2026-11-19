#include "global.h"

s32 ov14_021E7D9C(void *, u32);

void ov14_021E7DF8(void *arg0);

void ov14_021E7DF8(void *arg0) {
    u32 v0;

    v0 = 0;
    do {
        ov14_021E7D9C(arg0, v0);
        v0 += 1;
    } while (v0 < 6U);
}

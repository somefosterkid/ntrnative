#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021E7D9C(void *, u16);
#else
s32 ov14_021E7D9C(void *, u32);
#endif

void ov14_021E7DF8(void *arg0);

void ov14_021E7DF8(void *arg0) {
    u32 v0;

    v0 = 0;
    do {
        ov14_021E7D9C(arg0, v0);
        v0 += 1;
    } while (v0 < 6U);
}

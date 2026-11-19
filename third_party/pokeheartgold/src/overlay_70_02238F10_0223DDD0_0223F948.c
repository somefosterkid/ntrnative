#include "global.h"

void ov70_0223F948(void *arg0);

void ov70_0223F948(void *arg0) {
    s32 v0;
    void *v1;

    v0 = 0;
    v1 = arg0;
    do {
        *(s16 *)((u8 *)v1 + 4) = 0;
        *(s16 *)((u8 *)v1 + 0x18) = 0;
        v0 += 1;
        v1 += 2;
    } while (v0 < 0xA);
    *(s16 *)arg0 = 0;
    *(s16 *)((u8 *)arg0 + 2) = 0;
}

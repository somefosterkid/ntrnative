#include "global.h"

void ov96_021E9394(void *arg0);

void ov96_021E9394(void *arg0) {
    s32 v1;
    void *v0;

    v0 = arg0;
    v1 = 0;
    *(s32 *)v0 = 0;
    *(s32 *)((u8 *)v0 + 4) = 0;
    *(s32 *)((u8 *)v0 + 8) = 0;
    *(s32 *)((u8 *)v0 + 0xC) = 0;
    *(s32 *)((u8 *)v0 + 0x10) = 0;
    do {
        v1 += 1;
        *(s32 *)((u8 *)v0 + 0x144) = 0;
        v0 += 4;
    } while (v1 < 0xC);
}

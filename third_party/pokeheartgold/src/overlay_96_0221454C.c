#include "global.h"

s32 ov96_0221454C(void *arg0, void *arg1);

s32 ov96_0221454C(void *arg0, void *arg1) {
    u16 v0;
    u16 v1;
    u16 v2;
    u16 v3;

    v3 = *(u16 *)((u8 *)arg1 + 2);
    v2 = *(u16 *)((u8 *)arg0 + 2);
    if ((u32)v2 > (u32)v3) {
        return -1;
    }
    if ((u32)v2 < (u32)v3) {
        return 1;
    }
    v1 = *(u16 *)arg1;
    v0 = *(u16 *)arg0;
    if ((u32)v0 < (u32)v1) {
        return -1;
    }
    if ((u32)v0 > (u32)v1) {
        return 1;
    }
    return 0;
}

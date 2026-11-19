#include "global.h"

s32 ov43_0222AB20(void *, void *, s32, s32);

s32 ov43_0222C550(void *, void *, s32, s32);

s32 ov43_0222C620(void *arg0);

void ov43_0222C32C(void *arg0, void *arg1, void *arg2, s32 arg3);

void ov43_0222C32C(void *arg0, void *arg1, void *arg2, s32 arg3) {
    ov43_0222AB20(arg2, *(void **)((u8 *)arg1 + 4), *(u8 *)((u8 *)(arg1 + ov43_0222C620(arg0)) + 0x18), arg3);
    ov43_0222C550(arg0, arg2, 0x39, arg3);
}

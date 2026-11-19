#include "global.h"

void ov43_0222ADB8(s32 *, void *, u32);

s32 ov43_0222C5D8(void *arg0, s32 *arg1);

u32 ov43_0222C358(void *param_1, void *param_2, void *param_3);

s32 ov43_0222C3E8(void *, s32, s16);
s32 ov43_0222C7F8(void *, s32, s32);
s32 ov43_0222C844(void *, s32, s32);
s32 ov43_0222C918(void *, void *, s32, void *, s32, void *, s32, s32);
void ov43_0222C378(void *arg0, void *arg1, s32 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6);

u32 ov43_0222C358(void *param_1, void *param_2, void *param_3) {
    s32 iVar1;

    iVar1 = ov43_0222C5D8(param_1, param_2);
    if (iVar1 == 0) {
        return 0;
    }
    ov43_0222ADB8(param_2, param_3, 1);
    return 1;
}

void ov43_0222C378(void *arg0, void *arg1, s32 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 v1;
    void *v0;
    s32 v2;
    void *v3;

    v0 = arg1;
    if (arg5 == 0) {
        *(s16 *)arg0 = arg3;
        *(s16 *)((u8 *)arg0 + 4) = (s16)arg4;
        ov43_0222C3E8(arg0, arg2, arg3);
    }
    v2 = 0x38 * arg5;
    v3 = arg0 + 8;
    v1 = v2;
    ov43_0222C7F8(v3 + v2, arg2, arg2 + 0x200);
    ov43_0222C918(v3 + v2, v0, arg2, *(void **)((u8 *)arg0 + 0xB4), arg2 + 0x200, v0 + 0x10, (s32)arg3, arg6);
    ov43_0222C844(v3 + v2, arg2, arg4);
}

#include "global.h"
#include "overlay_43_unk_struct_ov64_02230904.h"
#include "overlay_43_palpad.h"

s32 ov43_0222DB28(void *, s32, s32, s32);

s32 ov43_0222DF4C(void *, s32, s32, s32, void *, s32, s32);

void ov43_0222DAE8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov43_0222DAE8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if (arg3 == 0) {
        *(s16 *)arg0 = (s16)arg4;
        ov43_0222DB28(arg0, arg2, arg4, arg3);
    }
    ov43_0222DF4C(arg0 + 0x14 + (0x4C * arg3), arg1, arg2, arg4, arg0 + 0x100, *((s32 *)&((PalPadDetails *)arg0)->hasFrontierPages), arg5);
}

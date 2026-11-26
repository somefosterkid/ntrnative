#include "global.h"

#include "sound.h"

void sub_020059E0(u8 param_1);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_02005A10(u8, u16, s32, s32, u8, void *);
#else
s32 sub_02005A10(s32, s32, s32, s32, s32, void *);
#endif
void GF_NowStartMusicId(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void *arg4);

void sub_020059E0(u8 param_1) {
    u8 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x13);
    *puVar1 = param_1;
    return;
}

void GF_NowStartMusicId(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void *arg4) {
    sub_02005A10(4, arg0, arg1, arg2, arg3, arg4);
}

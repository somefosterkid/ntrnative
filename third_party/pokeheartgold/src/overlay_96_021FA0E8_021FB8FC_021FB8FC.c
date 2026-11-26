#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021EB10C(u32 *, f32, f32);
#else
s32 ov96_021EB10C(u32 *, f32, f32);
#endif

void ov96_021FB8FC(void *arg0, s32 arg1);

void ov96_021FB8FC(void *arg0, s32 arg1) {
    f32 temp_r0;
    f32 temp_r0_2;
    f32 var_r5;

    if (arg1 != 0) {
        var_r5 = (f32)(100.0 - (10.0 * (double)(*(u16 *)((u8 *)arg0 + 0x38) - (u8)(*(u8 *)((u8 *)arg0 + 0xC) - 1))));
    } else {
        temp_r0 = (f32)(100.0 - (10.0 * (double)(u8)(*(u8 *)((u8 *)arg0 + 0xC) - 1)));
        var_r5 = temp_r0;
        if (temp_r0 < 0.0f) {
            var_r5 = 10.0f;
        }
    }
    temp_r0_2 = (f32)((double)var_r5 / 100.0);
    ov96_021EB10C(*(void **)arg0, temp_r0_2, temp_r0_2);
}

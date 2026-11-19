#include "global.h"

#include "error_handling.h"

s32 ov99_021E9430(void *, u32, s32);

void ov99_021E9464(void *arg0, s32 *arg1, u16 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov99_021E9464(void *arg0, s32 *arg1, u16 arg2, s32 arg3, s32 arg4, s32 arg5) {
    u16 temp_r0;
    int new_var;
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg3 >= 5) {
        GF_AssertFail();
    }
    if (arg4 >= 3) {
        GF_AssertFail();
    }
    s32 temp_r1;
    temp_r1 = ov99_021E9430(arg0, (u8)arg2, arg5);
    temp_r1 = (arg3 * 8) + temp_r1;
    new_var = arg4 * 2;
    temp_r0 = *((u16 *)(((u8 *)(temp_r1 + new_var)) + 2));
    u32 tmp = ((temp_r1 = 0xFFFFFE00U >> 0x17) & ((0xFFFFFE00U >> 0x16) & temp_r0)) | ((*arg1) & 0xFFFFFE00);
    *arg1 = (0xFFFFC1FF & tmp) | (((u32)((((s32)temp_r0) >> 0xA) << 0x1B)) >> 0x12);
}

#include "global.h"

#include "sys_task.h"
#include "error_handling.h"

void Heap_Free(void *ptr);

void ov99_021E5B74(void *ptr);

s32 ov99_021E5B7C(s32 *param_1, s32 param_2, s32 param_3);

u16 ov99_021E5B90(s32 *arg0, s32 arg1);

void ov99_021E5B74(void *ptr) {
    Heap_Free(ptr);
}

s32 ov99_021E5B7C(s32 *param_1, s32 param_2, s32 param_3) {
    return *param_1 + param_2 * 0x2c + 8 + param_3 * 0xc;
}

u16 ov99_021E5B90(s32 *arg0, s32 arg1) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg1 >= 5) {
        GF_AssertFail();
    }
    return *(u16 *)((u8 *)(*arg0 + (0x2C * arg1)) + 6);
}

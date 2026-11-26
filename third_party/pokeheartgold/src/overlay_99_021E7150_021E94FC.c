#include "global.h"

#include "error_handling.h"

void *ov99_021E9430(void **arg0, u32 arg1, s32 arg2);

s32 ov99_021E94FC(void **arg0, u32 arg1, s32 arg2);

s32 ov99_021E94FC(void **arg0, u32 arg1, s32 arg2) {
    return *(s32 *)((u8 *)(ov99_021E9430(arg0, arg1, arg2)) + 0x28);
}

s32 ov99_021E9508(void **arg0);

s32 ov99_021E9508(void **arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return *(s32 *)((u8 *)(*arg0) + 0xC);
}

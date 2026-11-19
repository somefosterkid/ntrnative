#include "global.h"

#include "error_handling.h"

void *ov99_021E9430(void **arg0, u32 arg1, s32 arg2);

void *ov99_021E9430(void **arg0, u32 arg1, s32 arg2) {
    if (arg1 >= 0xAU) {
        GF_AssertFail();
    }
    if (arg2 != 0) {
        return (void *)(*(u8 **)((u8 *)(*arg0) + 4) + (0xA4 * arg1));
    }
    return arg0 + 1 + (0xB * arg1);
}

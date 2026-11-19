#include "global.h"

#include "error_handling.h"

s32 ov45_0222C4E4(u8 *arg0, u32 arg1);

s32 ov45_0222C4E4(u8 *arg0, u32 arg1) {
    if (arg1 >= 9U) {
        GF_AssertFail();
    }
    return *(s32 *)((u8 *)(arg0 + (arg1 * 4)) + 0x2C);
}

s32 ov45_0222C4FC(u8 *arg0, u32 arg1);

s32 ov45_0222C4FC(u8 *arg0, u32 arg1) {
    if (arg1 >= 9U) {
        GF_AssertFail();
    }
    return *(s32 *)((u8 *)(arg0 + (arg1 * 4)) + 8);
}

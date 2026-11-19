#include "global.h"

#include "error_handling.h"

extern u8 ov99_021E9600;

u8 ov99_021E5C20(s32 arg0, s32 arg1, u32 arg2);

u16 ov99_021E5C4C(s32 *arg0, u8 arg1, u32 arg2);

u8 ov99_021E5C20(s32 arg0, s32 arg1, u32 arg2) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg1 >= 0xA) {
        GF_AssertFail();
    }
    if (arg2 >= 4U) {
        GF_AssertFail();
    }
    return *(u8 *)(arg2 + (&ov99_021E9600 + (arg1 * 4)));
}

u16 ov99_021E5C4C(s32 *arg0, u8 arg1, u32 arg2) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg2 >= 3U) {
        GF_AssertFail();
    }
    if (arg1 >= 5U) {
        GF_AssertFail();
    }
    return (u16)((u32)(*(s32 *)((u8 *)(*arg0 + (0x2C * arg1) + (0xC * arg2)) + 8) << 0x17) >> 0x17);
}

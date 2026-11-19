#include "global.h"

#include "error_handling.h"

u16 ov98_0221F01C(s32, u32);

s32 ov99_021E9430(void *, u32, s32);

void ov99_021E94CC(void *arg0, u16 arg1, u32 arg2, s32 arg3);

void ov99_021E94CC(void *arg0, u16 arg1, u32 arg2, s32 arg3) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg2 >= 5U) {
        GF_AssertFail();
    }
    ov98_0221F01C(ov99_021E9430(arg0, (u8)arg1, arg3), arg2);
}

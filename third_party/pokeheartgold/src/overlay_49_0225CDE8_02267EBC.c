#include "global.h"

#include "error_handling.h"

void ov49_02267EBC(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, u32 arg6);

void ov49_02267EBC(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, u32 arg6) {
    if (arg6 >= 3U) {
        GF_AssertFail();
    }
    *(void **)((u8 *)arg0 + 0xD8) = arg1;
    *(void **)((u8 *)arg0 + 0xDC) = arg2;
    *(void **)((u8 *)arg0 + 0xE0) = arg3;
    *(void **)((u8 *)arg0 + 0xE4) = arg4;
    *(void **)((u8 *)arg0 + 0xE8) = arg5;
    *(s16 *)((u8 *)arg0 + 2) = (s16)arg6;
}

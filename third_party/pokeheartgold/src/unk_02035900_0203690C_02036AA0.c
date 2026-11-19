#include "global.h"

#include "error_handling.h"

extern void *_0210F900;

void sub_02036AA0(s32 arg0);

void sub_02036AA0(s32 arg0) {
    if (arg0 != 0) {
        *(u8 *)((u8 *)(&_0210F900) + 1) = (u8)(*(u8 *)((u8 *)(&_0210F900) + 1) + 1);
        return;
    }
    GF_AssertFail();
}

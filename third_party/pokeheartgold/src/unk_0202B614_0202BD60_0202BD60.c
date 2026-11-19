#include "global.h"

#include "error_handling.h"
#include "unk_0202B614.h"

s32 sub_0202B614(s32);

void sub_0202BD60(void *arg0, s8 arg1) {
    if (sub_0202B614(*(s32 *)arg0) == 0) {
        GF_AssertFail();
    }
    *(s8 *)((u8 *)arg0 + 0x70) = arg1;
}

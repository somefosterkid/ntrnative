#include "global.h"

#include "error_handling.h"
#include "unk_0202B614.h"

s32 sub_0202B614(s32);

u8 sub_0202BE80(void *arg0) {
    if (sub_0202B614(*(s32 *)arg0) == 0) {
        GF_AssertFail();
    }
    return *(u8 *)((u8 *)arg0 + 0x70);
}

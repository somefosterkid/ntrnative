#include "global.h"

#include "error_handling.h"

u16 ov99_021E5B90(s32, s32);

s32 ov99_021E5BB4(s32 arg0, s32 arg1);

s32 ov99_021E5BB4(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (ov99_021E5B90(arg0, arg1) != 0) {
        return 1;
    }
    return 0;
}

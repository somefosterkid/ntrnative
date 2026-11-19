#include "global.h"

#include "error_handling.h"

extern u8 _0210F900;

void sub_02036ABC(s32 arg0);

void sub_02036ABC(s32 arg0) {
    if (arg0 != 0) {
        _0210F900 += 1;
        return;
    }
    GF_AssertFail();
}

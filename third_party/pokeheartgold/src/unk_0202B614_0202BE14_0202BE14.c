#include "global.h"

#include "error_handling.h"
#include "unk_0202B614.h"

s32 sub_0202B614(u32);

u32 *sub_0202BE14(u32 *param_1) {
    s32 iVar1;

    iVar1 = sub_0202B614(*param_1);
    if (iVar1 == 0) {
        GF_AssertFail();
    }
    return param_1 + 1;
}

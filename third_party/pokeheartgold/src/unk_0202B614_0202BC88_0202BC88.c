#include "global.h"

#include "error_handling.h"
#include "unk_0202B614.h"

s32 sub_0202B614(u32);

void sub_0202B6E8(u32 *, void *, void *);

void sub_0202BC88(u32 *param_1, void *param_2, void *param_3) {
    s32 iVar1;

    iVar1 = sub_0202B614(*param_1);
    if (iVar1 == 0) {
        GF_AssertFail();
    }
    sub_0202B6E8(param_1 + 1, param_2, param_3);
    return;
}

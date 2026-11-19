#include "global.h"

#include "error_handling.h"

s32 sub_02031B10(void);

s32 ov96_021E86FC(void *arg0, u32 arg1);

s32 ov96_021E86FC(void *arg0, u32 arg1) {
    u32 new_var;
    if (arg1 >= 4U) {
        GF_AssertFail();
    }
    if ((*((s32 *)(((u8 *)arg0) + 0xD68))) == 0) {
        GF_AssertFail();
    }
    new_var = sub_02031B10();
    return (*((s32 *)(((u8 *)arg0) + 0xD68))) + (new_var * arg1);
}

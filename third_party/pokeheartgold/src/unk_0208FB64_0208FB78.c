#include "global.h"

#include "unk_0208FB64.h"


void sub_0208FB78(s32 arg0, s32 arg1, s32 arg2, void *arg3) {
    s32 *new_var;
    u8 *new_var4;
    void *new_var2;
    int new_var3;
    new_var3 = 1;
    new_var4 = (u8 *)(new_var2 = arg3);
    new_var2 = ((u8 *)arg3) + 0x4A08;
    *((s32 *)new_var2) = (s32)((*(new_var = (s32 *)(new_var4 + 0x4A08))) + new_var3);
}

void sub_0208FB88(s32 param_1, u32 param_2, u8 *param_3, void *param_4) {
    if (param_1 == 0) {
        *(u32 *)((u8 *)param_4 + 0x4a10) = (u32)*param_3;
    }
    return;
}

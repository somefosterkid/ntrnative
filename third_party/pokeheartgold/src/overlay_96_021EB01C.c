#include "global.h"

void ov96_021EAD88(s32 *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_021EB01C(s32 *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_021EB03C(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);

void ov96_021EB01C(s32 *param_1, s32 param_2, s32 param_3, s32 param_4) {
    ov96_021EAD88(param_1, param_2 - param_1[5], param_3 - (param_1[10] + (param_1[6] * 2 - param_1[10]) / 2), param_4);
    return;
}

void ov96_021EB03C(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    int new_var;
    int v1;
    s32 v2;
    u32 v0;
    v1 = *((s32 *)(((u8 *)arg0) + 0x28));
    v2 = (*((s32 *)(((u8 *)arg0) + 0x18))) * 2;
    v0 = v2 - v1;
    new_var = (arg2 - ((v1 + (((s32)(v0 + (v0 >> 0x1F))) >> 1)) << 0xC)) + ((v2 - (*((s32 *)(((u8 *)arg0) + 0x2C)))) << 0xC);
    *arg3 = arg1;
    *arg4 = new_var;
}

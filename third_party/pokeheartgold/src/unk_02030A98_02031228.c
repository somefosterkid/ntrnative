#include "global.h"

#include "save_frontier.h"
#include "unk_02030A98.h"

void sub_02031228(FrontierSave *param_1, s32 param_2, s32 param_3, s16 param_4);

void sub_02031248(FrontierSave *arg0, s32 arg1, s32 arg2, s32 arg3);
u32 sub_020312A4(void);

void sub_02031228(FrontierSave *param_1, s32 param_2, s32 param_3, s16 param_4) {
    s32 new_var;
    int new_var3;
    unsigned int new_var2;
    u16 uVar1;
    new_var = param_3;
    new_var2 = new_var;
    uVar1 = FrontierSave_GetStat(param_1, param_2, new_var2);
    uVar1 = (new_var3 = uVar1 + param_4);
    sub_02031108(param_1, param_2, param_3, uVar1);
    return;
}

void sub_02031248(FrontierSave *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 v0;

    v0 = FrontierSave_GetStat(arg0, arg1, arg2) - arg3;
    if (v0 < 0) {
        v0 = 0;
    }
    sub_02031108(arg0, arg1, arg2, (u16)v0);
}

void sub_0203126C(FrontierSave *a0, u32 a1, u32 a2, u16 a3) {
    u32 v0;

    v0 = (u32)FrontierSave_GetStat(a0, (s32)a1, (s32)a2);
    if (v0 < (u32)a3) {
        sub_02031108(a0, (s32)a1, (s32)a2, a3);
        return;
    }
    if (v0 > 0x270FU) {
        sub_02031108(a0, (s32)a1, (s32)a2, 0x270FU);
    }
}

u32 sub_020312A4(void) {
    return 0xBA0;
}

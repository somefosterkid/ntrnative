#include "global.h"

u32 _d2f(void);

u64 _dadd(s32, s32, s32, s32);

u64 _ddiv(s32, s32, u32, u32);

u64 _dmul(u32, u32, s32, s32);

u64 _dsub(s32, s32, u32, u32);

u64 _f2d(u32);

u32 _fflt(u32);

void ov96_0220404C(u32 param_1, u32 param_2, float *param_3, float *param_4);

void ov96_0220404C(u32 param_1, u32 param_2, float *param_3, float *param_4) {
    double uVar5;
    double uVar4;
    float uVar1;
    float uVar2;
    float uVar3;
    uVar1 = (float)(s32)param_1;
    uVar2 = (float)(s32)param_2;
    uVar3 = 22.53846153846154 + 5.0 * (double)uVar2 / 52.0;
    uVar4 = 0.19375 * (double)uVar1 + (double)uVar2 / 8.0 - 15.5 - 10.0;
    uVar5 = 0.39402173913043476 + (double)uVar2 / 512.0;
    uVar1 = (float)(uVar4 / uVar5);
    *param_4 = uVar3;
    *param_3 = uVar1;
    return;
}

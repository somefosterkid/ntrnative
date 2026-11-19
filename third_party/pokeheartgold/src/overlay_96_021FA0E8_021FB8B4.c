#include "global.h"

u32 _d2f(void);

u64 _dadd(u32, u32, s32, s32);

void _ddiv(s32, s32, u32, u32);

u64 _dflt(s32);

u64 _dmul(u32, u32, s32, s32);

void ov96_021EB10C(u32 *param_1, u32 param_2, u32 param_3);

void ov96_021FB8B4(u32 *param_1, s32 param_2);

void ov96_021FB8B4(u32 *param_1, s32 param_2) {
    u32 uVar1;
    u64 uVar2;

    if (param_2 < 0) {
        param_2 = 0;
    }
    uVar2 = _dflt(param_2);
    uVar2 = _dmul(0, 0x40000000, (s32)uVar2, (s32)((u64)uVar2 >> 0x20));
    uVar2 = _dadd(0, 0x40590000, (s32)uVar2, (s32)((u64)uVar2 >> 0x20));
    _ddiv((s32)uVar2, (s32)((u64)uVar2 >> 0x20), 0, 0x40590000);
    uVar1 = _d2f();
    ov96_021EB10C((u32 *)*param_1, uVar1, uVar1);
    return;
}

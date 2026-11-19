#include "global.h"
#include "pokeathlon/pokeathlon.h"

u32 _d2f(void);

void _ddiv(s32, s32, u32, u32);

u64 _f2d(void);

u32 _fflt(u32);

void ov96_0220329C(PokeathlonCourseData *param_1, u8 *param_2, s32 param_3, s32 param_4, u8 *param_5);

void ov96_0220329C(PokeathlonCourseData *param_1, u8 *param_2, s32 param_3, s32 param_4, u8 *param_5) {
    PokeathlonMonKinds *pbVar1;
    u32 uVar2;
    u64 uVar3;

    pbVar1 = ov96_021E60D8(param_1, param_3, param_4);
    _fflt(*(u32 *)(param_2 + (u32)pbVar1->statIdx[4] * 4));
    uVar3 = _f2d();
    _ddiv((s32)uVar3, (s32)((u64)uVar3 >> 0x20), 0, 0x40240000);
    uVar2 = _d2f();
    *(u32 *)(param_5 + 4) = uVar2;
    _fflt(*(u32 *)(param_2 + (u32)pbVar1->statIdx[0] * 4 + 0x14));
    uVar3 = _f2d();
    _ddiv((s32)uVar3, (s32)((u64)uVar3 >> 0x20), 0, 0x40240000);
    uVar2 = _d2f();
    *(u32 *)(param_5 + 8) = uVar2;
    *(u32 *)(param_5 + 0x10) = *(u32 *)(param_2 + (u32)pbVar1->statIdx[4] * 4 + 0x28);
    uVar2 = _fflt(*(u32 *)(param_2 + (u32)pbVar1->statIdx[3] * 4 + 0x3c));
    *(u32 *)(param_5 + 0xc) = uVar2;
    *(u16 *)(param_5 + 0x14) = 0;
    *(u8 *)(param_5 + 0x18) = 0;
    *(u8 *)(param_5 + 0x19) = 0;
    *(u16 *)(param_5 + 0x16) = 0x78;
    return;
}

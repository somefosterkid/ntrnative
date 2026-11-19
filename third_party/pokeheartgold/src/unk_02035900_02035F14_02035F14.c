#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02037C94.h"

s32 sub_02034F64(s32, s32);

void sub_02035900(s32, s32);

extern u8 _0210F900;

s32 sub_02035F14(s32 arg0, s32 arg1, s32 arg2);

s32 sub_02035F14(s32 arg0, s32 arg1, s32 arg2) {
    u8 bVar1;
    BOOL BVar2;
    s32 iVar3;

    iVar3 = 1;
    bVar1 = sub_0203993C();
    BVar2 = sub_02034044((u32)bVar1);
    if (BVar2 == 0) {
        iVar3 = sub_02034F64(arg0, arg1);
    }
    sub_02035900(arg0, arg2);
    ((u8 *)&_0210F900)[1] = 4;
    return iVar3;
}

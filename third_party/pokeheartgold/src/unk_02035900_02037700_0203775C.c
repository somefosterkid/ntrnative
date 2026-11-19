#include "global.h"

u8 sub_02033FF0(u32);

s32 sub_0203775C(u32 arg0);

s32 sub_0203775C(u32 arg0) {
    s32 iVar1;

    iVar1 = sub_02033FF0(arg0 & 0xffff);
    return iVar1 + 1;
}

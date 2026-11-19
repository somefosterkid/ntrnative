#ifndef POKEHEARTGOLD_UNK_0202B614_H
#define POKEHEARTGOLD_UNK_0202B614_H

#include "global.h"
#include "sys_task.h"

void sub_0202B784(u32 *flags, u8 count, u8 accessoryID);
u8 sub_0202B7C8(const u32 *flags, u8 accessoryID);

void sub_0202BC88(u32 *param_1, void *param_2, void *param_3);

void sub_0202BD60(void *arg0, s8 arg1);

void sub_0202BDC8(u32 *param_1, void *param_2, u32 param_3);

u32 * sub_0202BE14(u32 *param_1);

void sub_0202BE60(u32 *param_1, void *param_2);

u8 sub_0202BE80(void *arg0);

s8 sub_0202BEDC(void *arg0);
u8 sub_0202BEE4(void *arg0);
u8 sub_0202BEEC(void *arg0);

SysTask *sub_0202BEF4(SysTaskFunc callback, void *param, int priority);

s8 sub_0202BF08(void *arg0);

#endif // POKEHEARTGOLD_UNK_0202B614_H

#ifndef POKEHEARTGOLD_UNK_0202068C_H
#define POKEHEARTGOLD_UNK_0202068C_H

#include "global.h"
#include "constants/heap.h"
#include "billboard.h"
#include "sys_task.h"

BillboardListVRAMTransfer *sub_0202068C(int maxElements, enum HeapID heapID);

void sub_020206C8(BillboardListVRAMTransfer *listTransfer);

SysTask *sub_02020738(SysTaskFunc callback, void *param, int priority);

u32 sub_020208DC(NNSG3dResTex *texture, const char *name);

void sub_020209AC(u8 *param_1, u32 param_2);

int sub_020209E0(NNSG3dResTex *texture, const char *name);

#endif // POKEHEARTGOLD_UNK_0202068C_H

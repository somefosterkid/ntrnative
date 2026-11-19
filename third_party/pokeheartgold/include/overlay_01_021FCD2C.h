#ifndef POKEHEARTGOLD_OVERLAY_01_021FCD2C_H
#define POKEHEARTGOLD_OVERLAY_01_021FCD2C_H

#include "global.h"
#include "heap.h"
#include "overlay_01_unk_struct_ov5_021f0fb8.h"
#include "sys_task.h"

struct FieldSystem;

SysTask *ov01_021FCD2C(struct FieldSystem *fieldSystem, enum HeapID heapID);

s32 ov01_021FCD6C(SysTask *arg0);

void ov01_021FCD78(SysTask *task);

void ov01_021FCD8C(SysTask *param0, int param1, fx32 param2, u32 param3);

void ov01_021FCDA8(SysTask *param0, void *param1);

void ov01_021FCDBC(UnkStruct_ov5_021F0FB8 *arg0);

#endif // POKEHEARTGOLD_OVERLAY_01_021FCD2C_H

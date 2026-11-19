#ifndef POKEHEARTGOLD_OVERLAY_85_H
#define POKEHEARTGOLD_OVERLAY_85_H

#include "global.h"
#include "overlay_manager.h"
#include "sys_task.h"
#include "pokemon_types_def.h"
#include "filesystem.h"
struct UnkStruct_ov109_021D5140_t;
struct UnkStruct_ov109_021D0F70_t;

s32 ov85_021E5A34(OverlayManager *arg0);

s32 ov85_021E5AF8(s32 *arg0);
s32 ov85_021E5D84(struct UnkStruct_ov109_021D0F70_t *arg0);

s32 ov85_021E5F84(s32 *arg0);

u32 ov85_021E6594(u32 *param_1);

s32 ov85_021E6644(s32 *arg0);

void ov85_021E6764(void *param0);
void ov85_021E678C(void *arg0);

SysTask *ov85_021E7650(SysTaskFunc callback, void *param, int priority);

void ov85_021E7B8C(SysTask *param0, void *param1);

s32 ov85_021E7BEC(void *arg0);
void ov85_021E7CE0(SysTask *param0, void *param1);
void ov85_021E7E18(SysTask *param0, void *param1);
void ov85_021E7F50(SysTask *param0, void *param1);

void ov85_021E80D0(SysTask *param0, void *param1);

void ov85_021E82E4(SysTask *param0, void *param1);

void ov85_021E84A4(SysTask *param0, void *param1);

void ov85_021E8530(fx32 *param0, fx32 param1);

BOOL ov85_021E87F0(Party *param0);

s32 ov85_021E8B08(OverlayManager *arg0, int *state);

void ov85_021E8BB0(SysTask *param0, void *param1);

void ov85_021E8C14(void *param0);

void ov85_021E8D64(struct UnkStruct_ov109_021D5140_t *arg0, NARC *arg1);
void ov85_021E8E6C(struct UnkStruct_ov109_021D5140_t *param_1, NARC *param_2);

void ov85_021E8F88(struct UnkStruct_ov109_021D5140_t *arg0, NARC *arg1);

void ov85_021E9160(struct UnkStruct_ov109_021D5140_t *arg0);

s32 ov85_021E92F8(s32 arg0, s32 arg1);

s32 ov85_021E9440(s32 arg0, s32 arg1);

s32 ov85_021E94BC(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);

s32 ov85_021E9614(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);

s32 ov85_021E96D0(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);
s32 ov85_021E9700(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);
s32 ov85_021E97E8(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);

s32 ov85_021E9834(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);

s32 ov85_021E9864(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);

s32 ov85_021E98E0(s32 arg0, s32 arg1);

s32 ov85_021E993C(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);
s32 ov85_021E996C(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);

s32 ov85_021E9AF0(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);
s32 ov85_021E9B1C(struct UnkStruct_ov109_021D5140_t *arg0, s32 arg1);

void ov85_021E9E58(u16 *param0);

void ov85_021EA324(NNSG2dCharacterData *param0[2], NNSG2dPaletteData *param1[2], int param2, int param3, int param4);

#endif // POKEHEARTGOLD_OVERLAY_85_H

#ifndef POKEHEARTGOLD_OVERLAY_29_H
#define POKEHEARTGOLD_OVERLAY_29_H

#include "global.h"
#include "sys_task.h"
#include "filesystem.h"
struct NumberEntryData;

struct NumberEntryArgs;
SysTask *ov29_0225D520(void *arg0, s32 arg1, s32 arg2, struct NumberEntryArgs *arg3);
void ov29_0225D5EC(u32 param_1, SysTask *param_2);

struct SpriteResource;
struct GF_2DGfxResMan;
void ov29_0225D880(struct SpriteResource **resObjs, struct GF_2DGfxResMan **resMans, NARC *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB);

void ov29_0225DB7C(struct NumberEntryData *arg0);

void ov29_0225DC44(void);
void ov29_0225DC48(void);

s32 ov29_0225DEB8(struct NumberEntryData *arg0, s32 arg1);

void ov29_0225DEF4(struct NumberEntryData *arg0, s32 arg1);

void ov29_0225E028(struct NumberEntryData *arg0, u8 arg1, u8 arg2, u8 arg3, u16 arg4);
void ov29_0225E078(struct NumberEntryData *arg0, u8 arg1, u8 arg2, s16 arg3);

void ov29_0225E0E0(struct NumberEntryData *arg0);

#endif // POKEHEARTGOLD_OVERLAY_29_H

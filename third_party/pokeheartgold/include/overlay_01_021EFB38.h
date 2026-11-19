#ifndef POKEHEARTGOLD_OVERLAY_01_021EFB38_H
#define POKEHEARTGOLD_OVERLAY_01_021EFB38_H

#include "global.h"
#include "sys_task.h"
#include "filesystem.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "screen_shake_effect.h"

struct FieldSystem;

void ov01_021EFB64(s32 arg0);
void ov01_021EFC04(s32 arg0);
void ov01_021EFC94(s32 effect, struct FieldSystem *fieldSystem, BOOL *done);

void ov01_021EFFD8(SysTask *task, void *data);

struct EncounterEffect;
struct ScreenSliceEffect;
struct ScreenSplitEffect;
struct ScreenSliceEffect *ov01_021EFFEC(void);
void ov01_021F0028(struct ScreenSliceEffect *efx);
void ov01_021F0050(struct EncounterEffect *encEffect, struct ScreenSliceEffect *efx, s32 pixelsPerSlice, s32 numSteps, fx32 startValue, fx32 endValue, fx32 initialRate);
void ov01_021F02C4(struct EncounterEffect *encEffect, struct ScreenSplitEffect *screenSplitEfx, u32 numSteps, fx32 initialSpeedX, fx32 initialSpeedY);

void ov01_021F0614(NARC *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

VecFx32 ov01_021F074C(fx32 param0, fx32 param1, fx32 param2);

void *ov01_021F0768(enum HeapID arg0);

void ov01_021F0788(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, u8 arg9);

void ov01_021F0868(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, u8 arg9);

void ov01_021F0960(Window *param0, s32 param1, s32 param2, s32 param3, s32 param4, u8 param5);

void *ov01_021F09BC(s32 arg0);
void ov01_021F09EC(void *arg0);

struct UnkStruct_ov5_021DED04;
void ov01_021F0B5C(struct UnkStruct_ov5_021DED04 *arg0);

void ov01_021F0C40(SysTask *param0, void *param1);

void ov01_021F0C7C(SysTask *param0, void *param1);

void ov01_021F0DC8(int *param0);

void ov01_021F0E74(void);
u8 ov01_021F0E90(void);
void ov01_021F0EAC(void);
void ov01_021F0EC0(void);

void ov01_021F0F08(NARC *param0, u32 param1);

void ov01_021F0FB8(u32 param0);

s32 ov01_021F1008(void);
s32 ov01_021F1044(void);
void ov01_021F1060(void);

u32 ov01_021F1224(u32 param0, BOOL param1);
u32 ov01_021F1250(u32 param0, BOOL param1);

void ov01_021F12B4(ScreenShakeEffect *screenShake, enum HeapID heapID);

#endif // POKEHEARTGOLD_OVERLAY_01_021EFB38_H

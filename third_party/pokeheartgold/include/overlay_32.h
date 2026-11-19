#ifndef POKEHEARTGOLD_OVERLAY_32_H
#define POKEHEARTGOLD_OVERLAY_32_H

#include "global.h"
#include "sprite_resources_header.h"
#include "sys_task.h"

struct BgConfig;
struct GF_2DGfxResMan;
struct NamePickerData;
struct NARC;
struct SpriteResource;

SysTask *ov32_0225D520(void *arg0, s32 arg1, void *arg2, s32 *arg3);

void ov32_0225D5CC(u32 param_1, SysTask *param_2);

void ov32_0225D60C(void);
void ov32_0225D634(struct BgConfig *bgConfig);
void ov32_0225D6C4(struct BgConfig *bgConfig);
void ov32_0225D6E0(struct NamePickerData *data, struct NARC *narc);
void ov32_0225D748(struct NamePickerData *data);
void ov32_0225D76C(struct NamePickerData *data);
void ov32_0225D788(struct NamePickerData *data);
void ov32_0225D834(struct NamePickerData *data);
void ov32_0225D84C(struct NamePickerData *data);
void ov32_0225D988(struct NamePickerData *data);
void ov32_0225DA88(struct NamePickerData *data, struct NARC *narc);
void ov32_0225DAC0(struct NamePickerData *data);
void ov32_0225DADC(struct NamePickerData *data);
void ov32_0225DB00(struct NamePickerData *data);
void ov32_0225DB1C(struct SpriteResource **resObjs, struct GF_2DGfxResMan **resMans, struct NARC *narc, s32 fileId, s32 arg4, s32 arg5, s32 arg6, s32 plttNum, s32 id, s32 arg9, s32 argA, s32 argB);
void ov32_0225DBAC(struct SpriteResource **resObjs, struct GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int vramTransfer);
void ov32_0225DC0C(struct NamePickerData *data, int index, SpriteResourcesHeader *header, const u8 *spec);
void ov32_0225DC68(struct NamePickerData *data, struct NARC *narc);
void ov32_0225DCD4(struct NamePickerData *data);

void ov32_0225DD04(struct NamePickerData *data);
void ov32_0225DD74(struct NamePickerData *data);

void ov32_0225DDAC(struct NamePickerData *data);
void ov32_0225DDB8(void);
void ov32_0225DDBC(void);
void ov32_0225DDC4(SysTask *task, void *param);

u32 ov32_0225DE34(struct NamePickerData *data);

u8 ov32_0225DF80(struct NamePickerData *data);

s32 ov32_0225DF9C(struct NamePickerData *data, s32 dir);

void ov32_0225DFE8(struct NamePickerData *data, u8 seq);

s32 ov32_0225E048(struct NamePickerData *data);

s32 ov32_0225E0A8(struct NamePickerData *data, u8 target, u8 seqA, u8 seqB, u8 nextState);

s32 ov32_0225E0FC(struct NamePickerData *data, u8 x, u8 y, u8 nextState);

#endif // POKEHEARTGOLD_OVERLAY_32_H

#ifndef POKEHEARTGOLD_OVERLAY_106_H
#define POKEHEARTGOLD_OVERLAY_106_H

#include "global.h"
#include "sys_task.h"
#include "camera.h"
#include "filesystem_files_def.h"
#include "overlay_106_motion_blur.h"

struct ArceusCinematic;
struct LegendaryCinematic;
struct LegendaryCinematicCamera;
struct LegendaryCinematicModelDesc;
struct ModelAttributes;

void ov106_021E5EE4(SysTask *task, void *param);
void ov106_021E5F24(SysTask *task, void *param);

void ov106_021E5900(struct LegendaryCinematic *cinematic);
void ov106_021E594C(struct LegendaryCinematic *cinematic);
void ov106_021E5954(struct LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState);
void ov106_021E597C(struct LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState);
void ov106_021E59A4(struct LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState);
void ov106_021E59D0(struct LegendaryCinematic *cinematic, s32 steps, s32 framesPerStep, s32 nextState);
void ov106_021E59FC(struct LegendaryCinematic *cinematic, const struct LegendaryCinematicCamera *params);
void ov106_021E5A44(const struct ModelAttributes *attrs);
void ov106_021E5A74(GXRgb *edgeColors);
void ov106_021E5AA8(struct LegendaryCinematic *cinematic, NarcId narcId, const struct LegendaryCinematicModelDesc *descs, u32 count);
void ov106_021E5BBC(struct LegendaryCinematic *cinematic);
void ov106_021E5C30(struct LegendaryCinematic *cinematic);
s32 ov106_021E5CD0(struct LegendaryCinematic *cinematic, s32 index);
s32 ov106_021E5D08(struct LegendaryCinematic *cinematic);
void ov106_021E5D70(struct LegendaryCinematic *cinematic);
void ov106_021E5DFC(struct LegendaryCinematic *cinematic);
void ov106_021E6064(MotionBlur *capture);

s32 ov106_021E6104(struct LegendaryCinematic *arg0);
s32 ov106_021E6118(struct LegendaryCinematic *arg0);
u32 ov106_021E6130(struct LegendaryCinematic *param_1);

s32 ov106_021E61F8(struct LegendaryCinematic *arg0);

s32 ov106_021E6280(struct LegendaryCinematic *arg0);
s32 ov106_021E62C8(struct LegendaryCinematic *arg0);
s32 ov106_021E62DC(struct LegendaryCinematic *arg0);

void ov106_021E6668(struct ArceusCinematic *arceus);

void ov106_021E66FC(struct LegendaryCinematic *arg0);

s32 ov106_021E6814(struct LegendaryCinematic *arg0);
s32 ov106_021E68A8(struct LegendaryCinematic *arg0);
s32 ov106_021E68DC(struct LegendaryCinematic *arg0);

s32 ov106_021E68F4(struct LegendaryCinematic *arg0);

void ov106_021E6A34(struct ArceusCinematic *arceus);

s32 ov106_021E6A80(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6AB0(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6AD0(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6AF0(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6B10(struct LegendaryCinematic *arg0, s32 *arg1);

s32 ov106_021E6B50(struct LegendaryCinematic *arg0, s32 *arg1);

u32 ov106_021E6BB0(struct LegendaryCinematic *param_1, s32 *param_2);

s32 ov106_021E6BD4(struct LegendaryCinematic *arg0, s32 *arg1);

u32 ov106_021E6BF8(struct LegendaryCinematic *param_1, s32 *param_2);

s32 ov106_021E6C28(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6C5C(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6C88(struct LegendaryCinematic *arg0, s32 *arg1);

u32 ov106_021E6CA8(struct LegendaryCinematic *param_1, s32 *param_2);

s32 ov106_021E6CC4(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6CF8(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6D1C(struct LegendaryCinematic *arg0, s32 *arg1);
s32 ov106_021E6D48(struct LegendaryCinematic *arg0, s32 *arg1);

#endif // POKEHEARTGOLD_OVERLAY_106_H

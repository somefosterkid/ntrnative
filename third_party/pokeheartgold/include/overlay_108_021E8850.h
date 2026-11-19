#ifndef POKEHEARTGOLD_OVERLAY_108_021E8850_H
#define POKEHEARTGOLD_OVERLAY_108_021E8850_H

#include "global.h"

#include "overlay_manager.h"
#include "sys_task.h"
#include "constants/heap.h"
struct SafariDecorationApp;
struct SafariDecorationEntry;

BOOL SafariDecoration_Exit(OverlayManager *man, int *state);

BOOL SafariDecoration_Main(OverlayManager *man, int *state);

s32 ov108_021E8B68(struct SafariDecorationApp *arg0);
s32 ov108_021E8BC0(struct SafariDecorationApp *arg0);
s32 ov108_021E8C18(struct SafariDecorationApp *arg0);

struct SafariDecoration3dView;
struct SafariDecoration3dModel;
struct SafariDecoration3dAnim;
struct SafariObjectConfig;

void ov108_021E8CD4(struct SafariDecoration3dView *view, u8 timeOfDay, enum HeapID heapID);
void ov108_021E8E10(struct SafariDecoration3dView *view);
void ov108_021E8E60(struct SafariDecoration3dView *view);
void ov108_021E8EA4(struct SafariDecoration3dView *view, s32 id, const struct SafariObjectConfig *config);
void ov108_021E8ED8(struct SafariDecoration3dView *view, u8 enabled);
void ov108_021E8F00(struct SafariDecoration3dView *view, s32 id, const struct SafariObjectConfig *config);
void ov108_021E90C4(struct SafariDecoration3dView *view, struct SafariDecoration3dModel *model);
void ov108_021E9144(struct SafariDecoration3dModel *model);
void ov108_021E9198(struct SafariDecoration3dView *view, struct SafariDecoration3dModel *model, struct SafariDecoration3dAnim *anim, s32 fileId);
void ov108_021E91D4(struct SafariDecoration3dView *view, struct SafariDecoration3dAnim *anim);
void ov108_021E91F8(struct SafariDecoration3dModel *model, struct SafariDecoration3dAnim *anim);
void ov108_021E9204(struct SafariDecoration3dModel *model, struct SafariDecoration3dAnim *anim);
void ov108_021E9210(struct SafariDecoration3dAnim *anim);
void ov108_021E9230(struct SafariDecoration3dView *view, enum HeapID heapID);

u32 ov108_021E9388(struct SafariDecorationApp *arg0);

s32 ov108_021E93A8(struct SafariDecorationApp *arg0);

void ov108_021E9488(struct SafariDecorationApp *app);
u32 ov108_021E94E8(struct SafariDecorationApp *app);
s32 ov108_021E9528(struct SafariDecorationApp *app, u32 arg1);
void ov108_021E979C(void *data);
void ov108_021E9A60(struct SafariDecorationApp *arg0);

void ov108_021E9C14(struct SafariDecorationApp *param_1);

void ov108_021E9C3C(struct SafariDecorationApp *arg0);

void ov108_021E9E10(struct SafariDecorationApp *arg0);

void ov108_021E9EB8(struct SafariDecorationApp *param_1);

void ov108_021EA418(struct SafariDecorationApp *arg0, s32 arg1);

void ov108_021EA50C(struct SafariDecorationApp *arg0);

void ov108_021EA5E4(SysTask *arg0, void *arg1);
void ov108_021EA624(struct SafariDecorationApp *arg0);

struct SafariDecorationEntry *ov108_021EA63C(u32 arg0, s32 arg1, u8 arg2, u8 *arg3, enum HeapID heapID);

void ov108_021E8A88(struct SafariDecorationApp *arg0);
void ov108_021E8AD4(void);
u32 ov108_021E8B24(struct SafariDecorationApp *param_1);
void ov108_021E8C64(struct SafariDecorationApp *arg0);
u32 ov108_021E929C(struct SafariDecorationApp *param_1);
u32 ov108_021E9304(struct SafariDecorationApp *param_1);
void ov108_021E940C(struct SafariDecorationApp *arg0, s32 arg1);
s32 ov108_021E95AC(struct SafariDecorationApp *arg0);
s32 ov108_021E96FC(struct SafariDecorationApp *arg0, s32 *arg1);
void ov108_021E9830(void);
void ov108_021E9850(struct SafariDecorationApp *param0);
void ov108_021E9A08(struct SafariDecorationApp *arg0);
void ov108_021E9BD4(struct SafariDecorationApp *arg0);
void ov108_021E9C2C(struct SafariDecorationApp *app);
void ov108_021E9CD0(struct SafariDecorationApp *arg0);
void ov108_021E9F94(struct SafariDecorationApp *param_1, s32 param_2);
void ov108_021EA040(struct SafariDecorationApp *arg0);
void ov108_021EA260(struct SafariDecorationApp *arg0);
void ov108_021EA2EC(struct SafariDecorationApp *arg0, u8 arg1);
void ov108_021EA334(struct SafariDecorationApp *arg0, s32 arg1);
void ov108_021EA47C(struct SafariDecorationApp *arg0);
s32 ov108_021EA52C(struct SafariDecorationApp *arg0, u8 arg1);
void ov108_021EA584(struct SafariDecorationApp *arg0, u8 arg1, s32 *arg2, u8 arg3);
u8 ov108_021EA700(s32 arg0, s32 arg1);
void ov108_021E9DE0(struct SafariDecorationApp *param_1);
void ov108_021E9E80(struct SafariDecorationApp *arg0);
void ov108_021E9F04(struct SafariDecorationApp *arg0);
void ov108_021E9F20(struct SafariDecorationApp *param_1, s32 param_2);

#endif // POKEHEARTGOLD_OVERLAY_108_021E8850_H

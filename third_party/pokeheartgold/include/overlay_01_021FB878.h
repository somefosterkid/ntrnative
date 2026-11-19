#ifndef POKEHEARTGOLD_OVERLAY_01_021FB878_H
#define POKEHEARTGOLD_OVERLAY_01_021FB878_H

#include "global.h"

#include "field/area_data.h"

#include "sys_task.h"

void ov01_021FB878(void *resourceFile, NNSG3dResTex *texture);

s32 ov01_021FBA00(AreaDataManager *arg0);

void ov01_021FBD8C(SysTask *task, void *param);

struct Field3dModel;
void ov01_021FBDFC(struct Field3dModel *model);

struct Field3dObject;
void Field3dObject_SetXRotation(struct Field3dObject *arg0, s16 arg1, s32 arg2);

#endif // POKEHEARTGOLD_OVERLAY_01_021FB878_H

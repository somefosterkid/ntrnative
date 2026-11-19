#ifndef POKEHEARTGOLD_OVERLAY_01_021FC05C_H
#define POKEHEARTGOLD_OVERLAY_01_021FC05C_H

#include "global.h"
#include "field/field_3d_object_task.h"
#include "sys_task.h"

// Work of the honey / sweet scent field effect (GetHoneySweetScentWorkSize bytes, 0xC on the DS: the task is a pointer).
typedef struct HoneyScentTaskEnv {
    Field3dObjectTask *task;
    int unk_4;
    u16 state;
    s16 timer;
} HoneyScentTaskEnv;

void ov01_021FC05C(u32 param_1, s32 param_2, u8 *param_3);

void ov01_021FC0DC(Field3dObjectTask *task, struct FieldSystem *fieldSystem, void *data);

void ov01_021FC10C(Field3dObjectTask *task, struct FieldSystem *fieldSystem, void *data);

void ov01_021FC124(Field3dObjectTask *task);

s32 ov01_021FC12C(Field3dObjectTask *arg0);

void ov01_021FC1FC(void *arg0);
void ov01_021FC260(void *arg0);

u32 ov01_021FC2C8(struct FieldSystem *fieldSystem);

void ov01_021FC2F0(HoneyScentTaskEnv *env);

#endif // POKEHEARTGOLD_OVERLAY_01_021FC05C_H

#ifndef POKEHEARTGOLD_OVERLAY_01_021FC05C_H
#define POKEHEARTGOLD_OVERLAY_01_021FC05C_H

#include "global.h"
#include "field/field_3d_object_task.h"
#include "linear_interpolation_task.h"
#include "sys_task.h"

// Work of the honey / sweet scent field effect (GetHoneySweetScentWorkSize bytes, 0xC on the DS: the task is a pointer).
typedef struct HoneyScentTaskEnv {
    Field3dObjectTask *task;
    int unk_4;
    u16 state;
    s16 timer;
} HoneyScentTaskEnv;

// Data of the honey / sweet scent screen fade task (0x18 bytes, Field3dObjectTask data).
typedef struct HoneyScentFadeData {
    LinearInterpolationTaskS32 lerp; // 0x00
    BOOL done;                       // 0x14
} HoneyScentFadeData;

void ov01_021FC05C(Field3dObjectTask *task, struct FieldSystem *fieldSystem, void *data);

void ov01_021FC0DC(Field3dObjectTask *task, struct FieldSystem *fieldSystem, void *data);

void ov01_021FC10C(Field3dObjectTask *task, struct FieldSystem *fieldSystem, void *data);

void ov01_021FC124(Field3dObjectTask *task);

s32 ov01_021FC12C(Field3dObjectTask *arg0);

void ov01_021FC1FC(struct FieldSystem *arg0);
void ov01_021FC260(struct FieldSystem *arg0);

u32 ov01_021FC2C8(struct FieldSystem *fieldSystem);

void ov01_021FC2F0(HoneyScentTaskEnv *env);

#endif // POKEHEARTGOLD_OVERLAY_01_021FC05C_H

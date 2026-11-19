#ifndef POKEHEARTGOLD_OV_01_021F6830_H
#define POKEHEARTGOLD_OV_01_021F6830_H

#include "script.h"

#include "poke_overlay.h"
#include "sys_task.h"

// One sub-application of the field's touch screen (touch-save / start-menu / Pokegear ... apps), run by the task
// ov01_021F69C0. ov01_02206C60 is the table of them, indexed by SubAppTaskData::appIdx.
typedef struct FieldSubAppTemplate {
    SysTask *(*init)(BgConfig *bgConfig, void *touchInput, FieldSystem *fieldSystem, void *args);
    void (*start)(BgConfig *bgConfig, SysTask *appTask);
    BOOL (*main)(BgConfig *bgConfig);
    FSOverlayID overlayId; // 0xFFFFFFFF: no overlay to load
} FieldSubAppTemplate;

// Data of the sub-app manager task (FieldSystem::unk_D8). sizeof == 0x10 on the DS.
typedef struct SubAppTaskData {
    u8 appIdx;
    u8 state;
    u8 nextAppIdx;
    SysTask *appTask;
    FieldSystem *fieldSystem;
    void *args;
} SubAppTaskData;

extern const FieldSubAppTemplate ov01_02206C60[];

void ov01_021F69C0(SysTask *task, void *taskData);
void ov01_021F6830(FieldSystem *fieldSystem, int a1, int a2);
void ov01_021F6B50(FieldSystem *fieldSystem);
void ov01_021F6B64(FieldSystem *fieldSystem, int arg1);

#endif

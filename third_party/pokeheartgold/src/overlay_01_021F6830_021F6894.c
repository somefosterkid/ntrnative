#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "poke_overlay.h"
#include "sys_task.h"

SysTask *ov01_021F68DC(BgConfig *bgConfig, void *touchInput, FieldSystem *fieldSystem);

FS_EXTERN_OVERLAY(OVY_27);

void ov01_021F6894(FieldSystem *fieldSystem);

void ov01_021F690C(FieldSystem *fieldSystem);
void ov01_021F68B8(FieldSystem *fieldSystem);

void ov01_021F6894(FieldSystem *fieldSystem) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_27), OVY_LOAD_ASYNC);
    fieldSystem->unk_D8 = ov01_021F68DC(fieldSystem->bgConfig, &fieldSystem->lastTouchMenuInput, fieldSystem);
}

void ov01_021F68B8(FieldSystem *fieldSystem) {
    ov01_021F690C(fieldSystem);
}

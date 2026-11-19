#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "poke_overlay.h"
#include "sys_task.h"
#include "touch_save_app.h"

FS_EXTERN_OVERLAY(OVY_27);

void ov01_021F6840(FieldSystem *fieldSystem);

void ov01_021F6840(FieldSystem *fieldSystem) {
    SysTask *temp_r0;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_27), OVY_LOAD_ASYNC);
    temp_r0 = ov30_0225D520(fieldSystem->bgConfig, &fieldSystem->lastTouchMenuInput, fieldSystem, NULL);
    fieldSystem->unk_D4 = temp_r0;
}

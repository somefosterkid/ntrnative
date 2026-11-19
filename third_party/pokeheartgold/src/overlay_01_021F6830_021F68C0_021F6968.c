#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "poke_overlay.h"

void ov01_021F6968(SubAppTaskData *data);

void ov01_021F6968(SubAppTaskData *data) {
    FSOverlayID ovyId;
    FieldSystem *fieldSystem;
    SysTask *appTask;

    ovyId = ov01_02206C60[data->appIdx].overlayId;
    if (ovyId != 0xffffffff) {
        HandleLoadOverlay(ovyId, OVY_LOAD_ASYNC);
    }
    fieldSystem = data->fieldSystem;
    appTask = ov01_02206C60[data->appIdx].init(fieldSystem->bgConfig, &fieldSystem->lastTouchMenuInput, fieldSystem, data->args);
    data->appTask = appTask;
    return;
}

#include "global.h"

#include "bg_window.h"
#include "overlay_67_app.h"
#include "sys_task.h"

SysTask *ov67_021E6A00(void);

void ov67_021E5BE0(void *data);

void ov67_021E5BE0(void *data) {
    Ov67App *app = data;

    ov67_021E6A00();
    DoScheduledBgGpuUpdates(app->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

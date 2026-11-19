#include "global.h"

#include "error_handling.h"
#include "system.h"
#include "overlay_01_021FB4C0.h"
#include "sys_task.h"

#include "field/hblank_system_priv.h"

void HBlankSystem_Stop(HBlankSystem *hBlankSystem);

void ov01_021FB584(HBlankTask *task);

void HBlankSystem_Stop(HBlankSystem *hBlankSystem) {
    BOOL result = Main_SetHBlankIntrCB(NULL, NULL);
    GF_ASSERT(result == TRUE);

    hBlankSystem->active = FALSE;
}

HBlankTask *ov01_021FB530(HBlankSystem *hBlankSystem, HBlankCallback callback, void *param);

HBlankTask *ov01_021FB530(HBlankSystem *hBlankSystem, HBlankCallback callback, void *param) {
    HBlankTask *task = ov01_021FB5B4(hBlankSystem);
    GF_ASSERT(task);

    if (task) {
        task->param = param;
        task->callback = callback;
        task->used = TRUE;
    }

    return task;
}

void ov01_021FB554(HBlankTask *task) {
    ov01_021FB584(task);
}

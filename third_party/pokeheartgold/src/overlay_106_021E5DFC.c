#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "gf_3d_render.h"

#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

void ov106_021E5F24(SysTask *task, void *param);
void ov106_021E5EE4(SysTask *task, void *param);
void ov106_021E5F84(MotionBlurParams *params);
MotionBlur *ov106_021E5E0C(MotionBlurParams *params);

void ov106_021E5DFC(LegendaryCinematic *cinematic) {
    GF3dRender_DeleteSimpleManager();
    Camera_Delete(cinematic->camera);
}

MotionBlur *ov106_021E5E0C(MotionBlurParams *params) {
    SysTask *task = CreateSysTaskAndEnvironment(ov106_021E5EE4, sizeof(MotionBlur), 5, params->heapID);
    MotionBlur *motionBlur = SysTask_GetData(task);

    motionBlur->params = *params;
    motionBlur->task = task;
    motionBlur->initialized = 0;
    motionBlur->lcdcVRamBank = GX_GetBankForLCDC();

    ov106_021E5F84(&motionBlur->params);
    SysTask_CreateOnVWaitQueue(ov106_021E5F24, motionBlur, 0);

    return motionBlur;
}

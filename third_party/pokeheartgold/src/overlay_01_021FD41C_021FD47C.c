#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_021FD41C.h"

#include "overlay_01_021F1348.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov01_021FD47C(UnkStruct_ov5_021F1388 *param0);
void ov01_021FD488(UnkStruct_ov5_021F1388 *param0, VecFx32 *param1);

void ov01_021FD47C(UnkStruct_ov5_021F1388 *param0) {
    SysTask_Destroy(param0->unk_24);
}

void ov01_021FD488(UnkStruct_ov5_021F1388 *param0, VecFx32 *param1) {
    *param1 = param0->unk_14;
}

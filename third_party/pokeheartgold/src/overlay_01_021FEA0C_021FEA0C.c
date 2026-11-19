#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

#include "sys_task.h"
#include "overlay_01_021FEA0C.h"

void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);

void *ov01_021FEA0C(FieldEffectManager *param0) {
    UnkStruct_ov5_021F2874 *v0 = ov01_021F1430(param0, (sizeof(UnkStruct_ov5_021F2874)), 0, 0);
    v0->unk_10 = param0;

    return v0;
}

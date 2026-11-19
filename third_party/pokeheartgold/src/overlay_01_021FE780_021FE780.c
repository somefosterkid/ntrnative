#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"
#include "overlay_01_021FE780.h"

void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);
void ov01_021FE7AC(UnkStruct_ov5_021F4DD8 *param0);

void *ov01_021FE780(FieldEffectManager *param0) {
    UnkStruct_ov5_021F4DD8 *v0 = ov01_021F1430(param0, (sizeof(UnkStruct_ov5_021F4DD8)), 0, 0);
    v0->unk_00 = param0;

    ov01_021FE7AC(v0);

    return v0;
}

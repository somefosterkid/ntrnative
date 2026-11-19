#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"
#include "overlay_01_021FED9C.h"

void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);
void ov01_021FEDC8(UnkStruct_ov5_021F23FC *param0);

void *ov01_021FED9C(FieldEffectManager *param0) {
    UnkStruct_ov5_021F23FC *v0 = ov01_021F1430(param0, (sizeof(UnkStruct_ov5_021F23FC)), 0, 0);
    v0->unk_00 = param0;

    ov01_021FEDC8(v0);
    return v0;
}

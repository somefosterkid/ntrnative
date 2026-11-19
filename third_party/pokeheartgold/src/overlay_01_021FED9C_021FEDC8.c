#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"


struct Simple3DAnimation;

struct Simple3DModel;



void ov01_021F19F4(FieldEffectManager *fieldEffMan, Simple3DModel *model, u32 modelIndex, u32 memberIdx, BOOL allocAtEnd);

void ov01_021F1A18(FieldEffectManager *fieldEffMan, Simple3DAnimation *anim, u32 unused, u32 memberIndex, BOOL allocAtEnd);


void ov01_021FEDC8(UnkStruct_ov5_021F23FC *param0);

void ov01_021FEDC8(UnkStruct_ov5_021F23FC *param0) {
    ov01_021F19F4(param0->unk_00, &param0->unk_04, 0, 30, 0);
    ov01_021F1A18(param0->unk_00, &param0->unk_18, 0, 135, 0);
}

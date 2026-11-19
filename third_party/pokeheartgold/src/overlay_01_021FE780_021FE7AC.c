#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

#include "overlay_01_021F1348.h"
#include "unk_0206979C.h"

void sub_02069784(Simple3DModel *model);
void ov01_021FE7D0(UnkStruct_ov5_021F4DD8 *param0);

void ov01_021FE7AC(UnkStruct_ov5_021F4DD8 *param_1) {
    ov01_021F19F4(param_1->unk_00, &param_1->unk_04, 0, 0x56, 0);
    sub_02069978(&param_1->unk_18.g3DRenderObj, &param_1->unk_04);
}

void ov01_021FE7D0(UnkStruct_ov5_021F4DD8 *param0) {
    sub_02069784(&param0->unk_04);
}

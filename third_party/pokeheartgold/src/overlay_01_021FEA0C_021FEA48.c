#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

#include "overlay_01_021F1348.h"
#include "unk_0206979C.h"
#include "sys_task.h"

void sub_02069784(Simple3DModel *model);
void ov01_021FEA7C(UnkStruct_ov5_021F2874 *param0);

void ov01_021FEA48(UnkStruct_ov5_021F2874 *arg0) {
    if (arg0->unk_04 == 0) {
        ov01_021F19F4(arg0->unk_10, &arg0->unk_14, 0, 0x57, 1);
        sub_02069978(&arg0->unk_28.g3DRenderObj, &arg0->unk_14);
        arg0->unk_04 = 1;
    }
}

void ov01_021FEA7C(UnkStruct_ov5_021F2874 *param0) {
    if (param0->unk_04 == 1) {
        param0->unk_04 = 0;
        sub_02069784(&param0->unk_14);
    }
}

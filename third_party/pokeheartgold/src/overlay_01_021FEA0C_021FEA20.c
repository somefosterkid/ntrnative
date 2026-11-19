#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FEA0C.h"
#include "sys_task.h"

void ov01_021FEA30(UnkStruct_ov5_021F2874 *param0);
void ov01_021FEA38(UnkStruct_ov5_021F2874 *param0);

void ov01_021FEA20(void *a0) {
    ov01_021FEA7C(a0);
    ov01_021F1448(a0);
}

void ov01_021FEA30(UnkStruct_ov5_021F2874 *param0) {
    param0->unk_00++;
}

void ov01_021FEA38(UnkStruct_ov5_021F2874 *param0) {
    param0->unk_00--;

    ((param0->unk_00 >= 0) ? (void)0 : GF_AssertFail());
}

#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "camera.h"
#include "unk_02026E30.h"

void ov96_021F61C8(UnkStruct_ov96_021F5018 *param_1);

void ov96_021F637C(UnkStruct_ov96_021F5018 *param_1);

void ov96_021F637C(UnkStruct_ov96_021F5018 *param_1) {
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    ov96_021F61C8(param_1);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    return;
}

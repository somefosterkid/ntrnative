#include "global.h"
#include "camera.h"
#include "pokepic.h"
#include "unk_02014DA0.h"
#include "unk_02026E30.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "unk_struct_ov59_0223c5b0.h"

void ov59_0223C304(UnkStruct_ov59_0223C5B0 *arg0);

void ov59_0223C304(UnkStruct_ov59_0223C5B0 *arg0) {
    Thunk_G3X_Reset();
    sub_0201543C();
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    Camera_PushLookAtToNNSGlb();
    reg_G3_MTX_MODE = 0;
    reg_G3_MTX_IDENTITY = 0;
    reg_G3_MTX_MODE = 2;
    reg_G3_MTX_IDENTITY = 0;
    NNS_G3dGlbFlushP();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(arg0->unk_C);
    sub_02015460();
    reg_G3_SWAP_BUFFERS = 1;
}

s32 ov59_0223C5B0(UnkStruct_ov59_0223C5B0 *);

void ov59_0223C350(UnkStruct_ov59_0223C5B0 *arg0);

void ov59_0223C350(UnkStruct_ov59_0223C5B0 *arg0) {
    Camera_Delete(arg0->unk_8);
    PokepicManager_Delete(arg0->unk_C);
    ov59_0223C5B0(arg0);
    GF_3DVramMan_Delete(arg0->unk_4);
    Heap_Free(arg0);
}

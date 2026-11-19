#include "global.h"

#include "camera.h"
#include "unk_02026E30.h"
#include "overlay_89_unk_struct_ov113_0225_dbcc_t.h"

s32 ov89_0225A5A4(void *);

s32 sub_020181EC(void *);

void ov89_02259C0C(UnkStruct_ov113_0225DBCC_t *arg0);

void ov89_02259C0C(UnkStruct_ov113_0225DBCC_t *arg0) {
    s32 sp28[6];
    MtxFx33 sp4;
    u32 sp0;

    sp28[3] = 0x1000;
    sp28[4] = 0x1000;
    sp28[5] = 0x1000;
    sp28[0] = 0;
    sp28[1] = 0;
    sp28[2] = 0;
    MTX_Identity33_(&sp4);
    Thunk_G3X_Reset();
    Camera_SetStaticPtr(arg0->camera);
    Camera_ApplyPerspectiveType(0U, arg0->camera);
    Camera_PushLookAtToNNSGlb();
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, 0xFFFFF000, 0);
    NNS_G3dGlbLightColor(GX_LIGHTID_0, 0x739CU);
    NNS_G3dGlbMaterialColorDiffAmb(0x7FFFU, 0x7FFFU, 0);
    NNS_G3dGlbMaterialColorSpecEmi(0x7FFFU, 0x7FFFU, 0);
    NNS_G3dGlbSetBaseTrans((VecFx32 *)&sp28[0]);
    MI_Copy36B(&sp4, &NNS_G3dGlb.prmBaseRot);
    NNS_G3dGlb.flag &= ~0xA4;
    NNS_G3dGlbSetBaseScale((VecFx32 *)&sp28[3]);
    NNS_G3dGeBufferOP_N(0x11U, NULL, 0U);
    sub_020181EC(&arg0->unk_D8.unk_10);
    ov89_0225A5A4(&arg0->unk_194);
    sp0 = 1;
    NNS_G3dGeBufferOP_N(0x12U, &sp0, 1U);
}

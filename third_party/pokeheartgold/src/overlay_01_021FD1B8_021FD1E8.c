#include "global.h"

#include "overlay_01_021FD1B8.h"
#include "overlay_01_021F1348.h"

void sub_02069978(NNSG3dRenderObj *renderObj, Simple3DModel *model);
void ov01_021F19F4(FieldEffectManager *fieldEffMan, Simple3DModel *model, u32 modelIndex, u32 memberIdx, BOOL allocAtEnd);

void ov01_021FD1E8(UnkStruct_021F1118 *param0, u32 param1, u32 param2, FieldEffectManager *param3);

void sub_02069784(Simple3DModel *model);
void ov01_021FD20C(UnkStruct_021F1118 *param0);

void ov01_021FD1E8(UnkStruct_021F1118 *param0, u32 param1, u32 param2, FieldEffectManager *param3) {
    param0->unk_00 = param1;

    ov01_021F19F4(param3, &param0->unk_04, 0, param2, 0);
    sub_02069978(&param0->unk_18.g3DRenderObj, &param0->unk_04);
}

void ov01_021FD20C(UnkStruct_021F1118 *param0) {
    param0->unk_00 = 0xffff;
    sub_02069784(&param0->unk_04);
}

#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

struct FieldEffectManager;

struct Simple3DModel;

struct Simple3DRenderObj;

void sub_02069978(NNSG3dRenderObj *renderObj, Simple3DModel *model);

void ov01_021F19F4(FieldEffectManager *fieldEffMan, Simple3DModel *model, u32 modelIndex, u32 memberIdx, BOOL allocAtEnd);

extern const u32 ov01_0220901C[10];

extern const u32 ov01_02208FF4[10];

void ov01_021FE230(UnkStruct_021F1CF8 *param0);

void ov01_021FE230(UnkStruct_021F1CF8 *param0) {
    u32 v0, v1;

    for (v0 = 0; v0 < 10; v0++) {
        v1 = ov01_0220901C[v0];
        ov01_021F19F4(param0->unk_00, &param0->unk_04[v0], 0, v1, 0);
        sub_02069978(&param0->unk_194[v0].g3DRenderObj, &param0->unk_04[v0]);

        v1 = ov01_02208FF4[v0];
        ov01_021F19F4(param0->unk_00, &param0->unk_CC[v0], 0, v1, 0);
        sub_02069978(&param0->unk_4DC[v0].g3DRenderObj, &param0->unk_CC[v0]);
    }
}

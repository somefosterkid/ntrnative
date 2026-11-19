#include "global.h"

#include "overlay_01_021FD41C.h"
#include "unk_02069660.h"

void ov01_021FD60C(UnkStruct_ov5_021F1388 *ctx) {
    s32 i;
    Simple3DModel *model;

    i = 0;
    model = ctx->unk_28;
    do {
        sub_02069784(model);
        i = i + 1;
        model = model + 1;
    } while (i < OV01_021FD41C_MODEL_COUNT);
}

void ov01_021FD624(UnkStruct_ov5_021F1388 *ctx, int param1) {
    NNS_G3dMdlUseMdlAlpha(ctx->unk_28[0].g3DModel);
    NNS_G3dMdlSetMdlAlphaAll(ctx->unk_28[0].g3DModel, param1);
}

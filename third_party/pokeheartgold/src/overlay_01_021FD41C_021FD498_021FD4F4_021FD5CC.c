#include "global.h"

#include "overlay_01_021FD41C.h"
#include "unk_0206979C.h"

extern const int ov01_02208E90[OV01_021FD41C_MODEL_COUNT];

void ov01_021FD5CC(UnkStruct_ov5_021F1388 *ctx) {
    int v0;

    for (v0 = 0; v0 < OV01_021FD41C_MODEL_COUNT; v0++) {
        ov01_021F19F4(ctx->unk_20, &ctx->unk_28[v0], 0, ov01_02208E90[v0], 0);
        sub_02069978(&ctx->unk_78[v0].g3DRenderObj, &ctx->unk_28[v0]);
    }
}

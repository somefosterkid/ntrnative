#include "global.h"

#include "overlay_01_021FD41C.h"

void *ov01_021FD41C(FieldEffectManager *fieldEffMan) {
    UnkStruct_ov5_021F1388 *ctx;

    ctx = ov01_021F1430(fieldEffMan, sizeof(UnkStruct_ov5_021F1388), 0, 0);
    ctx->unk_20 = fieldEffMan;
    ov01_021FD5CC(ctx);
    ov01_021FD458(ctx);
    return ctx;
}

void ov01_021FD440(void *a0) {
    UnkStruct_ov5_021F1388 *ctx = a0;

    ov01_021FD47C(ctx);
    ov01_021FD60C(ctx);
    ov01_021F1448(ctx);
}

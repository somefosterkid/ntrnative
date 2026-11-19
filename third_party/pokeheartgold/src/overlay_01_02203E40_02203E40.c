#include "global.h"

#include "overlay_01_02203E40.h"

void *ov01_02203E40(FieldEffectManager *fieldEffMan) {
    UnkOv01_02203E40 *ctx;

    ctx = ov01_021F1430(fieldEffMan, sizeof(UnkOv01_02203E40), 0, 0);
    ctx->fieldEffMan = fieldEffMan;
    ctx->anim = NULL;
    ctx->unk_64 = 1.0f;
    ov01_02203E74(ctx);
    return ctx;
}

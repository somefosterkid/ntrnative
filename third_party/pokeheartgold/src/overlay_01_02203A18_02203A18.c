#include "global.h"

#include "overlay_01_02203A18.h"

void *ov01_02203A18(FieldEffectManager *fieldEffMan) {
    UnkOv01_02203A18 *ctx;

    ctx = ov01_021F1430(fieldEffMan, sizeof(UnkOv01_02203A18), 0, 0);
    ctx->fieldEffMan = fieldEffMan;
    ctx->busy = 0;
    ov01_02203B28(ctx);
    return ctx;
}

void ov01_02203A38(void *a0) {
    UnkOv01_02203A18 *ctx = a0;

    ov01_02203B70(ctx);
    ov01_021F1448(ctx);
}

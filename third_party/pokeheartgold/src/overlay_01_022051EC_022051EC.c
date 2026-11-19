#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_022051EC.h"

void *ov01_022051EC(FieldEffectManager *fieldEffMan) {
    UnkOv01_022051EC *ctx;

    ctx = ov01_021F1430(fieldEffMan, sizeof(UnkOv01_022051EC), FALSE, 0);
    ctx->fieldEffMan = fieldEffMan;
    ov01_02205218(ctx);
    return ctx;
}

void ov01_02205208(void *a0) {
    UnkOv01_022051EC *ctx = a0;

    ov01_022052A4(ctx);
    ov01_021F1448(ctx);
}

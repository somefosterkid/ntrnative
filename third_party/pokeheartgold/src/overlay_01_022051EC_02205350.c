#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "overlay_01_022051EC.h"

void ov01_02205350(void) {
}

void ov01_02205354(void *param0, UnkOv01_022051EC_Work *work) {
    Field3dObject_SetActiveFlag(&work->ctx->obj, 0);
    return;
}

void ov01_02205364(void *param0, UnkOv01_022051EC_Work *work) {
    UnkOv01_022051EC *ctx;
    BOOL done;

    ctx = work->ctx;
    done = ov01_02205388(ctx->anim, 2);
    if (done != 0) {
        work->state = 1;
    }
    Field3dObject_Draw(&ctx->obj);
    return;
}

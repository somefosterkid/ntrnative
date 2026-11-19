#include "global.h"

#include "overlay_07_bg_scroll_context.h"

s32 sub_02014A60(BufferManager *);

s32 sub_02014AA0();

s32 sub_02014AB0(s32, void *, s32, s32);

void ov07_02222BA4(CustomBgScrollContext *ctx);

void ov07_02222BA4(CustomBgScrollContext *ctx) {
    s32 v0;

    v0 = sub_02014A60(ctx->bufferManager);
    sub_02014AA0();
    sub_02014AB0(v0, ctx->offsetReg, 4, 1);
}

s32 sub_02014A8C(BufferManager *);

void ov07_02222BC8(CustomBgScrollContext *ctx);

void ov07_02222BC8(CustomBgScrollContext *ctx) {
    sub_02014A8C(ctx->bufferManager);
    ov07_02222BA4(ctx);
}

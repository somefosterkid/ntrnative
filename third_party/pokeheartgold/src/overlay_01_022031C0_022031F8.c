#include "global.h"

#include "overlay_01_022031C0.h"

s32 ov01_021FBD38(void *, void *);

void ov01_022031F8(UnkOv01_022031C0 *ctx) {
    ctx->mdlData0 = ov01_021F14B4(ctx->fxMgr, 0x81, 0);
    ov01_021FBD38(&ctx->model0, ctx->mdlData0);
    Field3dObject_InitFromModel(&ctx->obj0, &ctx->model0);
    ctx->mdlData1 = ov01_021F14B4(ctx->fxMgr, 0x68, 0);
    ov01_021FBD38(&ctx->model1, ctx->mdlData1);
    Field3dObject_InitFromModel(&ctx->obj1, &ctx->model1);
    Field3dModelAnimation_LoadFromFilesystem(&ctx->anim, &ctx->model1, NARC_a_1_0_3, 0xA4, HEAP_ID_FIELD1, &ctx->alloc);
    Field3dObject_AddAnimation(&ctx->obj1, &ctx->anim);
}

void ov01_021FBDFC(Field3dModel *model);

void ov01_02203270(UnkOv01_022031C0 *ctx) {
    ov01_021FBDFC(&ctx->model0);
    ov01_021F1448(ctx->mdlData0);
    ov01_021FBDFC(&ctx->model1);
    ov01_021F1448(ctx->mdlData1);
    Field3dModelAnimation_Unload(&ctx->anim, &ctx->alloc);
}

#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_01_022051EC.h"

void ov01_02205218(UnkOv01_022051EC *ctx) {
    HeapExp_FndInitAllocator(&ctx->allocator, HEAP_ID_FIELD1, 0x20);
    Field3dModel_LoadFromFilesystem(&ctx->model, NARC_a_1_0_3, 0x84, HEAP_ID_FIELD1);
    Field3dModelAnimation_LoadFromFilesystem(&ctx->anim[0], &ctx->model, NARC_a_1_0_3, 0xa8, HEAP_ID_FIELD1, &ctx->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&ctx->anim[1], &ctx->model, NARC_a_1_0_3, 0xa6, HEAP_ID_FIELD1, &ctx->allocator);
    Field3dObject_InitFromModel(&ctx->obj, &ctx->model);
    Field3dObject_AddAnimation(&ctx->obj, &ctx->anim[0]);
    Field3dObject_AddAnimation(&ctx->obj, &ctx->anim[1]);
    ov01_022053C4(ctx->anim, 2, 0);
    Field3dObject_SetActiveFlag(&ctx->obj, 0);
    return;
}

void ov01_022052A4(UnkOv01_022051EC *ctx) {
    Field3dModelAnimation_Unload(&ctx->anim[1], &ctx->allocator);
    Field3dModelAnimation_Unload(&ctx->anim[0], &ctx->allocator);
    Field3dModel_Unload(&ctx->model);
    return;
}

#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "overworld_anim_manager.h"
#include "overlay_02_hmcut_in.h"

void ov02_0224B87C(OverworldAnimManager *manager, FollowMonModelCtx *ctx);

void ov02_0224B87C(OverworldAnimManager *manager, FollowMonModelCtx *ctx) {
    Field3dObject_Draw(&ctx->cutIn->fieldModel.object);
    return;
}

#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "overworld_anim_manager.h"
#include "overlay_02_hmcut_in.h"

void ov02_0224B808(OverworldAnimManager *manager, FollowMonModelCtx *ctx);

void ov02_0224B808(OverworldAnimManager *manager, FollowMonModelCtx *ctx) {
    HMCutIn *temp_r4;
    u32 temp_r0;
    HMCutInFieldModel *new_var;
    temp_r0 = ctx->state;
    temp_r4 = ctx->cutIn;
    new_var = &temp_r4->fieldModel;
    switch (temp_r0) {
    case 0:
        ctx->state = 1U;
        return;

    case 1:
        Field3dObject_SetActiveFlag(&new_var->object, 1);
        Field3dModelAnimation_FrameSet(&new_var->anims[0], 0);
        Field3dModelAnimation_FrameSet(&new_var->anims[1], 0);
        ctx->state = 2U;

    case 2:
        temp_r0 = 0x1000;
        Field3dModelAnimation_FrameAdvanceAndCheck(&new_var->anims[0], temp_r0);
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&new_var->anims[1], temp_r0) != 0) {
            ctx->finished = 1;
            ctx->state = 3U;
        }

    default:

    case 3:
        return;
    }
}

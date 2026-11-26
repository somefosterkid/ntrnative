#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "sprite.h"
#include "sprite_system.h"

void ov86_021E705C(FrontierRecordsApp *param_1, s32 param_2, s32 param_3) {
    ManagedSprite_SetAnimationFrame(param_1->sprites[param_2], 0);
    ManagedSprite_SetAnim(param_1->sprites[param_2], param_3);
    return;
}

void ov86_021E707C(FrontierRecordsApp *arg0, s32 arg1, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(arg0->sprites[arg1], x, y);
}

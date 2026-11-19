#include "global.h"
#include "overlay_59_juice_feed_app.h"
#include "menu_input_state.h"
struct Ov59JuiceFeedApp;

s32 ov59_0223AE88(struct Ov59JuiceFeedApp *arg0);

void ov59_0223AA6C(Ov59JuiceFeedApp *arg0);

void ov59_0223AA6C(Ov59JuiceFeedApp *arg0) {
    s16 *temp_r1;
    s8 var_r1;

    ov59_0223AE88(arg0);
    MenuInputStateMgr_SetState(arg0->parent->args->menuInputStatePtr, arg0->menuInputState);
    arg0->parent->unk_1C = arg0->mons[arg0->selected].slot;
    if (arg0->mode != 0) {
        var_r1 = 1;
    } else {
        var_r1 = 3;
    }
    arg0->parent->unk_18 = var_r1;
    temp_r1 = arg0->parent->args->unk8;
    if (temp_r1 != NULL) {
        *temp_r1 = (s16) arg0->mode;
    }
}

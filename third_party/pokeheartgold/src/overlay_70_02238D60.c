#include "global.h"
struct GTSApplicationState;

#include "touchscreen_list_menu.h"
#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238FE0(void);

void ov70_02238D60(GTSApplicationState *arg0);

void ov70_02238D8C(Sprite *sprite, int x, int y);

void ov70_02238D60(GTSApplicationState *arg0) {
    TouchscreenListMenu_Destroy(arg0->unk_11D0);
    TouchscreenListMenuSpawner_Destroy(arg0->unk_11CC);
    ov70_02238FE0();
}

void ov70_02238D84(struct GTSApplicationState *arg0, s32 arg1, s32 arg2);

void ov70_02238D84(GTSApplicationState *arg0, s32 arg1, s32 arg2) {
    *((s32 *)&arg0->currentScreenInstruction) = arg1;
    *((s32 *)&arg0->nextScreenInstruction) = arg2;
}

void ov70_02238D8C(Sprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = FX32_CONST(x);
    pos.y = FX32_CONST(y);
    pos.z = 0;

    Sprite_SetMatrix(sprite, &pos);
}

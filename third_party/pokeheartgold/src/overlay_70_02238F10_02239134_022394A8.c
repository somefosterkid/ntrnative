#include "global.h"
struct GTSApplicationState;

#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_022394A8(struct GTSApplicationState *arg0);

void ov70_022394A8(GTSApplicationState *arg0) {
    Sprite_Delete(arg0->cursorSprite);
}

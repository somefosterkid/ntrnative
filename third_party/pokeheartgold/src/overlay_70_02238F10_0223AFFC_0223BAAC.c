#include "global.h"
struct GTSApplicationState;

#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223BAAC(struct GTSApplicationState *arg0);

void ov70_0223BAAC(GTSApplicationState *arg0) {
    Sprite_Delete(arg0->cursorSprite);
    Sprite_Delete(arg0->unused_DD0);
    Sprite_Delete(arg0->boxArrowSprites[0]);
    Sprite_Delete(arg0->boxArrowSprites[1]);
}

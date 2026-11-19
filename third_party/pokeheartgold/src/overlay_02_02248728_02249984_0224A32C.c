#include "global.h"

#include "overlay_02_hmcut_in.h"
#include "sprite.h"

void ov02_0224A32C(HMCutIn *cutIn);

void ov02_0224A32C(HMCutIn *cutIn) {
    SpriteList *spriteList;

    spriteList = cutIn->spriteList;
    if (spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(spriteList);
    }
}

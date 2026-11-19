#include "global.h"

#include "overlay_91.h"
#include "sprite.h"

void ov91_02260CB4(void *arg0) {
    Sprite_SetDrawFlag(*(Sprite **)((u8 *)arg0 + 4), 1);
    *(s32 *)arg0 = 1;
}

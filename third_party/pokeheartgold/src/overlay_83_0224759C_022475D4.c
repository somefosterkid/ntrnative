#include "global.h"

#include "sprite.h"

void ov83_022475D4(void *arg0, s32 arg1);

void ov83_022475D4(void *arg0, s32 seq) {
    Sprite_SetAnimationFrame(*(Sprite **)((u8 *)arg0 + 4), 0U);
    Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)arg0 + 4), seq);
}

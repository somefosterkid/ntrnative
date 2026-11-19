#include "global.h"

#include "overlay_34.h"
#include "sprite.h"

void ov34_0225E630(Ov34Work *work) {
    s32 i;

    i = 0;
    do {
        Sprite_GetAnimationFrame(work->sprites[i]);
        if (work->animFlags[i] == 1) {
            Sprite_SetAnimActiveFlag(work->sprites[i], 0);
            work->animFlags[i] = 0;
        } else if (Sprite_GetAnimActiveFlag(work->sprites[i]) == 0) {
            Sprite_SetAnimActiveFlag(work->sprites[i], 1);
            Sprite_SetAnimationFrame(work->sprites[i], 1U);
        }
        i += 1;
    } while (i < 2);
}

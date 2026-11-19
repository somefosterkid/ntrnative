#include "global.h"

#include "sprite.h"

s32 ov70_02238D8C(Sprite *, int, int);

extern s32 ov70_02245784;

extern s32 ov70_02245786;

void ov70_0223D8E8(Sprite *arg0, s32 arg1);

void ov70_0223D8E8(Sprite *arg0, s32 arg1) {
    s32 temp_r3;

    temp_r3 = arg1;
    ov70_02238D8C(arg0, *(u16 *)(&ov70_02245784 + temp_r3), *(u16 *)(&ov70_02245786 + temp_r3));
    if ((arg1 == 0x1F) || ((arg1 >= 0) && (arg1 <= 5))) {
        Sprite_SetPriority(arg0, 0U);
        return;
    }
    Sprite_SetPriority(arg0, 1U);
}

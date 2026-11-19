#include "global.h"

#include "sprite.h"
#include "overlay_97_course_sub_screen.h"

s32 ov97_0221F5F8(Ov97CourseSubScreenSprites *, u8 *);

s32 ov97_0221FBDC(Ov97CourseSubScreenSprites *, void *);

s32 ov97_0221FC54(Ov97CourseSubScreenSprites *);

void ov97_0221F56C(Ov97CourseSubScreen *arg0, u8 *arg1);

void ov97_0221F56C(Ov97CourseSubScreen *arg0, u8 *arg1) {
    u8 i;
    Ov97CourseSubScreenSprites *sub;
    u8 idx;

    sub = &arg0->sprites;
    ov97_0221FBDC(sub, arg1);
    ov97_0221FC54(&arg0->sprites);
    Sprite_SetDrawFlag(arg0->sprites.monSprite, 1);
    ov97_0221F5F8(&arg0->sprites, arg1);
    {
        u8 order[5] = { 1, 3, 2, 4, 0 };
        for (i = 0; i < 5; i++) {
            idx = order[i];
            if (*(u8 *)(arg1 + idx + 0x20) != 0) {
                Sprite_SetDrawFlag(sub->unk_1B4[idx], 1);
            } else {
                Sprite_SetDrawFlag(sub->unk_1B4[idx], 0);
            }
        }
    }
}

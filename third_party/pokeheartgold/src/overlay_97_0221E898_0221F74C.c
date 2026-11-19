#include "global.h"
#include "sprite.h"
#include "overlay_97_course_sub_screen.h"

void ov97_0221F74C(Ov97CourseSubScreen *arg0);

void ov97_0221F74C(Ov97CourseSubScreen *arg0) {
    Ov97CourseSubScreenSprites *sprites;
    s32 sp0;
    u8 temp_r0;
    u8 var_r4;
    u8 var_r4_2;

    sprites = &arg0->sprites;
    var_r4 = 0;
    do {
        Sprite_SetDrawFlag(sprites->unk_1B4[var_r4], 0);
        Sprite_SetDrawFlag(sprites->unk_1C8[var_r4], 0);
        var_r4 += 1;
    } while ((u32) var_r4 < 5U);
    sp0 = 0;
    do {
        Sprite_SetDrawFlag(sprites->statRows[sp0].arrow, 0);
        var_r4_2 = 0;
loop_4:
        Sprite_SetDrawFlag(sprites->statRows[sp0].marks[var_r4_2], 0);
        var_r4_2 += 1;
        if ((u32) var_r4_2 < 5U) {
            goto loop_4;
        }
        temp_r0 = sp0 + 1;
        sp0 = (s32) temp_r0;
    } while ((u32) temp_r0 < 5U);
    Sprite_SetDrawFlag(sprites->monSprite, 0);
}

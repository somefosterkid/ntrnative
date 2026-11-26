#include "global.h"

#include "sprite.h"
#include "unk_02013534.h"

void sub_02013728(u32);

extern VecFx32 ov96_0221C3E0;

void ov96_021F8C88(s32 param_1, s32 param_2, u32 *param_3);

void ov96_021F8C88(s32 param_1, s32 param_2, u32 *param_3) {
    VecFx32 vec = ov96_0221C3E0;

    vec.y = vec.y + param_2 * 0x1000;
    vec.x = (param_1 + 0x80) * 0x1000;
    Sprite_SetMatrix((Sprite *)param_3[0], &vec);
    vec.x = (param_1 + 0x10) * 0x1000;
    Sprite_SetMatrix((Sprite *)param_3[1], &vec);
    vec.x = (param_1 + 0x30) * 0x1000;
    Sprite_SetMatrix((Sprite *)param_3[2], &vec);
    vec.y = vec.y - 0x4000;
    Sprite_SetMatrix((Sprite *)param_3[3], &vec);
    sub_02013728(param_3[4]);
    sub_02013728(param_3[8]);
}

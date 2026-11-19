#include "global.h"

#include "sprite.h"

void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

struct Sprite *ov96_021EB5B8(void *param_1);

void ov96_021F4504(u8 *param_1, u32 param_2, u32 param_3);

void ov96_021F4E5C(u8 *param_1, u32 param_2);

void ov96_021F4E5C(u8 *param_1, u32 param_2) {
    Sprite *sprite;
    VecFx32 VStack_18;

    ov96_021F4504(param_1, 0, param_2);
    sprite = ov96_021EB5B8(*(void **)(param_1 + 0x90));
    VStack_18.x = 0x80000;
    VStack_18.y = 0x1d8000;
    VStack_18.z = 0;
    Sprite_SetMatrix(sprite, &VStack_18);
    ov96_021EB52C(*(s16 **)(param_1 + 0x90), 1, 1);
    return;
}

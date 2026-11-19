#include "sprite.h"
s32 ov96_021EB408(s32, s32, s32, u8, u8);
s32 ov96_021EB4F4(s32, u8, u8);
s32 ov96_021EB52C(s32, s32, s32);
Sprite *ov96_021EB5B8(s32);
s32 ov96_021F440C(s32, void *);
extern s32 ov96_0221BFF4;

typedef struct Words2 { u32 w[2]; } Words2;
void ov96_021F3EC0(s32 arg0, void *arg1) {
    VecFx32 pos;
    u32 a[2] = {0x66, 0x67};
    u32 b[2] = {5, 6};
    Sprite *sprite;
    int i;

    for (i = 0; i < 2; i++) {
        ov96_021EB408(arg0, 3, 2, a[i], b[i]);
    }
    for (i = 0; i < 2; i++) {
        ((s32 *)arg1)[i + 0x3E] = ov96_021EB4F4(arg0, a[i], b[i]);
        sprite = ov96_021EB5B8(((s32 *)arg1)[i + 0x3E]);
        Sprite_SetDrawPriority(sprite, 2);
        pos.z = 0;
        pos.x = (i * 0x80 + 0x40) << 0xC;
        pos.y = 0x260000;
        Sprite_SetMatrix(sprite, &pos);
        ov96_021EB52C(((s32 *)arg1)[i + 0x3E], 1, 0);
    }
    ov96_021F440C(arg0, arg1);
}

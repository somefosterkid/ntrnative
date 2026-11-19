#include "sprite_system.h"
ManagedSprite *ov96_022104D8(s32, s32, s16, s32, s32, s32);

void ov96_0221075C(void *arg0) {
    s32 ys;
    s32 i = 0;
    s32 y;
    s32 x;
    s32 j;
    s32 k = 7;
    s32 sign;
    ManagedSprite **slot;
    ManagedSprite *spr;

    y = 8;
    do {
        j = 0;
        x = (i + 4) * 8;
        ys = (s16)y;
        do {
            slot = (ManagedSprite **)((u8 *)arg0 + (j + k) * 4 + 0x68);
            if (j == 0) {
                sign = 1;
            } else {
                sign = -1;
            }
            spr = ov96_022104D8(*(s32 *)((u8 *)arg0 + 8), *(s32 *)((u8 *)arg0 + 0xC), (s16)(0x80 + sign * x), ys, 0x14, 0x40);
            *slot = spr;
            ManagedSprite_SetPriority(spr, 3);
            j++;
        } while (j < 2);
        k += 2;
        y += 0x10;
        i++;
    } while (i < 12);
}

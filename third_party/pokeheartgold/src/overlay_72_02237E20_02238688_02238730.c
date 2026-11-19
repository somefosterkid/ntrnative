#include "global.h"

#include "sprite.h"

void ov72_02238688(Sprite *, int, int);

extern u32 ov72_0223B364;

void ov72_02238730(Sprite *sprite, s32 param_2, u32 param_3);

void ov72_02238730(Sprite *sprite, s32 param_2, u32 param_3) {
    ov72_02238688(sprite, *(u16 *)((u8 *)&ov72_0223B364 + param_3 * 12 + param_2 * 4), 0x129);
}

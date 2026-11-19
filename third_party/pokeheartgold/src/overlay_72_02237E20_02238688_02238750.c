#include "global.h"

#include "sprite.h"

void ov72_02238688(Sprite *, int, int);

extern s32 ov72_0223B364;

extern s32 ov72_0223B366;

void ov72_02238750(Sprite *sprite, s32 arg1, u32 arg2);

void ov72_02238750(Sprite *sprite, s32 arg1, u32 arg2) {
    s32 temp_r3;
    s32 temp_r4;

    temp_r4 = arg1 * 4;
    temp_r3 = arg2 * 0xC;
    ov72_02238688(sprite, *(u16 *)(temp_r4 + (((u8 *)&ov72_0223B364) + temp_r3)), *(u16 *)(temp_r4 + (((u8 *)&ov72_0223B366) + temp_r3)) + 0x100);
}

#include "global.h"

#include "sprite.h"

struct Sprite *ov96_021EB408(enum HeapID *, s32, s32, u8, u8);

s32 ov96_021EB4F4(void *, s32, s32);

s32 ov96_021EB52C(void *, s32, s32);

Sprite *ov96_021EB5B8(void *);

extern u8 ov96_0221BFF4[];

void ov96_021F3F80(void *arg0, u8 *arg1);

void ov96_021F3F80(void *arg0, u8 *arg1) {
    u8 sp14[4];
    u8 sp10[4];
    VecFx32 sp18 = { 0, 0, 0 };
    Sprite *sprite;
    u8 *temp_r4;
    Sprite *temp_r0;
    u8 i;
    sp14[0] = ov96_0221BFF4[4];
    sp14[1] = ov96_0221BFF4[5];
    sp14[2] = ov96_0221BFF4[6];
    sp14[3] = ov96_0221BFF4[7];
    sp10[0] = ov96_0221BFF4[0];
    sp10[1] = ov96_0221BFF4[1];
    sp10[2] = ov96_0221BFF4[2];
    sp10[3] = ov96_0221BFF4[3];
    temp_r4 = (u8 *)(arg1 + 0x68);
    ov96_021EB408(arg0, 0, 2, 0x69, 8);
    ov96_021EB408(arg0, 0, 2, 0x69, 9);
    ov96_021EB408(arg0, 1, 2, 0x69, 0xA);
    i = 0;
    do {
        ov96_021EB408(arg0, 0, 2, 0x69, 0xB);
        i += 1;
    } while (((u32)i) < 2U);
    i = 0;
    do {
        ov96_021EB408(arg0, 1, 2, 0x69, 0xC);
        i += 1;
    } while (((u32)i) < 4U);
    i = 0;
    do {
        ov96_021EB408(arg0, 1, 2, 0x69, 0xD);
        i += 1;
    } while (((u32)i) < 4U);
    *((s32 *)(0xB4 + temp_r4)) = ov96_021EB4F4(arg0, 0x69, 8);
    temp_r0 = ov96_021EB5B8(*((void **)(temp_r4 + 0xB4)));
    Sprite_SetDrawPriority(temp_r0, 4U);
    Sprite_SetAnimCtrlSeq(temp_r0, 0);
    sp18.x = 0x80000;
    sp18.y = 0x220000;
    Sprite_SetMatrix(temp_r0, &sp18);
    ov96_021EB52C(*((void **)(temp_r4 + 0xB4)), 1, 1);
    *((s32 *)(temp_r4 + 0xB8)) = ov96_021EB4F4(arg0, 0x69, 9);
    temp_r0 = ov96_021EB5B8(*((void **)(temp_r4 + 0xB8)));
    Sprite_SetDrawPriority(temp_r0, 3U);
    Sprite_SetAnimCtrlSeq(temp_r0, 1);
    sp18.x = 0x98000;
    sp18.y = 0x220000;
    Sprite_SetMatrix(temp_r0, &sp18);
    ov96_021EB52C(*((void **)(temp_r4 + 0xB8)), 1, 1);
    *((s32 *)(temp_r4 + 0xC4)) = ov96_021EB4F4(arg0, 0x69, 0xA);
    temp_r0 = ov96_021EB5B8(*((void **)(temp_r4 + 0xC4)));
    Sprite_SetDrawPriority(temp_r0, 5U);
    Sprite_SetAnimCtrlSeq(temp_r0, 0xD);
    sp18.x = 0;
    sp18.y = 0x200000;
    Sprite_SetMatrix(temp_r0, &sp18);
    ov96_021EB52C(*((void **)(temp_r4 + 0xC4)), 1, 0);
    i = 0;
    do {
        *((s32 *)((temp_r4 + (i * 4)) + 0xBC)) = ov96_021EB4F4(arg0, 0x69, 0xB);
        sprite = ov96_021EB5B8(*((void **)((temp_r4 + (i * 4)) + 0xBC)));
        Sprite_SetDrawPriority(sprite, 6U);
        Sprite_SetAnimCtrlSeq(sprite, i + 0xB);
        sp18.x = ((0xA0 * i) + 0x30) << 0xC;
        sp18.y = 0x218000;
        Sprite_SetMatrix(sprite, &sp18);
        ov96_021EB52C(*((void **)((temp_r4 + (i * 4)) + 0xBC)), 1, 1);
        i += 1;
    } while (((u32)i) < 2U);
    i = 0;
    do {
        *((s32 *)((temp_r4 + (i * 4)) + 0xC8)) = ov96_021EB4F4(arg0, 0x69, 0xC);
        sprite = ov96_021EB5B8(*((void **)((temp_r4 + (i * 4)) + 0xC8)));
        Sprite_SetDrawPriority(sprite, 8U);
        Sprite_SetAnimCtrlSeq(sprite, 0xE);
        sp18.x = sp14[i] << 0xC;
        sp18.y = (sp10[i] << 0xC) + 0x200000;
        Sprite_SetMatrix(sprite, &sp18);
        ov96_021EB52C(*((void **)((temp_r4 + (i * 4)) + 0xC8)), 1, 1);
        i += 1;
    } while (((u32)i) < 4U);
    i = 0;
    do {
        *((s32 *)((temp_r4 + (i * 4)) + 0xD8)) = ov96_021EB4F4(arg0, 0x69, 0xD);
        sprite = ov96_021EB5B8(*((void **)((temp_r4 + (i * 4)) + 0xD8)));
        Sprite_SetDrawPriority(sprite, 7U);
        Sprite_SetAnimCtrlSeq(sprite, 0x12);
        sp18.x = sp14[i] << 0xC;
        sp18.y = (sp10[i] << 0xC) + 0x200000;
        Sprite_SetMatrix(sprite, &sp18);
        ov96_021EB52C(*((void **)((temp_r4 + (i * 4)) + 0xD8)), 1, 0);
        i += 1;
    } while (((u32)i) < 4U);
}

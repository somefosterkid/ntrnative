#include "global.h"

#include "overlay_96_ov96_arena_work.h"


s32 ov96_021EB06C(Ov96SpriteSetElem *, s32, s32, s32 *, s32 *);

s32 ov96_021F2FBC(void *elem1, void *elem2);

void ov96_021F2FEC(Ov96ArenaWork *arg0, u8 *arg1);

void ov96_021F2FEC(Ov96ArenaWork *arg0, u8 *arg1) {
    s32 sp14[12];
    s32 sp10;
    s32 spC;
    s32 sp8;
    u8 *sp4;
    s16 var_r4;
    u8 temp_r7;
    u8 var_r4_2;

    var_r4 = 0;
    do {
        *(s16 *)(&(&sp14[0])[var_r4]) = var_r4;
        sp4 = arg1 + var_r4;
        sp8 = var_r4 / 3;
        ov96_021EB06C(arg0->mon[sp8][var_r4 % 3].spr, arg1[var_r4], sp4[0xC], &sp10, &spC);
        *(s16 *)((u8 *)(&(&sp14[0])[var_r4]) + 2) = (s16)spC;
        var_r4 = (s16)(u8)(var_r4 + 1);
    } while ((u32)var_r4 < 0xCU);
    MATH_QSort(&sp14[0], 0xCU, 4U, ov96_021F2FBC, arg0->unk_7F0);
    var_r4_2 = 0;
    do {
        temp_r7 = (u8)((u16 *)&sp14[0])[var_r4_2 * 2];
        ov96_021EABA8(arg0->mon[(s32)temp_r7 / 3][(s32)temp_r7 % 3].spr, var_r4_2 + 6);
        var_r4_2 += 1;
    } while ((u32)var_r4_2 < 0xCU);
}

#include "global.h"

#include "overlay_96_ov96_arena_work.h"

s32 ov96_021EB0A4(void *, s32, s32, s32 *, s32 *);

extern s32 ov96_0221BDD4;

void ov96_021F2A00(Ov96ArenaWork *arg0);

void ov96_021F2A00(Ov96ArenaWork *arg0) {
    s32 sp14;
    s32 sp10;
    s32 spC;
    Ov96ArenaMon *sp8;
    s32 *sp4;
    s32 var_r7;
    s32 var_r6;
    Ov96ArenaMon *var_r4;
    s32 temp_r0;
    s32 *var_r5;

    spC = 0;
    sp8 = arg0->mon[0];
    sp4 = &ov96_0221BDD4;
    var_r7 = 2;
    do {
        var_r4 = sp8;
        var_r5 = sp4;
        var_r6 = 0;
loop_2:
        if (var_r4->state == 0) {
            var_r4->state = 2;
            ov96_021EB0A4(var_r4->spr, *((u16 *)var_r5), *((u16 *)(((u8 *)var_r5) + 2)), &sp14, &sp10);
            var_r4->pos.x = sp14 << 0xC;
            var_r4->pos.y = sp10 << 0xC;
            var_r4->unk_1C = var_r4->pos;
            var_r4->unk_40 = 2;
        }
        var_r6 += 1;
        var_r4++;
        var_r5 += 1;
        if (var_r6 < 3) {
            goto loop_2;
        }
        sp8 += 3;
        sp4 += 3;
        temp_r0 = spC + 1;
        spC = temp_r0;
    } while (temp_r0 < 4);
}

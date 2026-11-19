#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "overlay_96_app_0221040c.h"

ManagedSprite *ov96_0220ECA4(SpriteSystem *, SpriteManager *, s16, s16, u16, u16, s32);

ManagedSprite *ov96_0220ED34(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);

void ov96_0221007C(UnkStruct_ov96_0220E3F0_Hud *arg0, SpriteSystem *arg1, SpriteManager *arg2, Ov96App_0221040C *arg3);

void ov96_0221007C(UnkStruct_ov96_0220E3F0_Hud *arg0, SpriteSystem *arg1, SpriteManager *arg2, Ov96App_0221040C *arg3) {
    UnkStruct_ov96_0220E3F0_HudGroup *sp18;
    s32 sp14;
    SpriteManager *sp10;
    UnkStruct_ov96_0220E3F0_Hud *spC;
    ManagedSprite **var_r4_2;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r6;

    spC = arg0;
    sp10 = arg2;
    arg0->unk_0 = arg3;
    var_r5 = 0;
    do {
        arg0->slots[var_r5].sprite = ov96_0220ED34(arg1, sp10, 0, 0, 5, 5);
        var_r5 += 1;
    } while (var_r5 < 0xC);
    sp14 = 0;
    sp18 = spC->groups;
    for (; sp14 < 2; sp14++) {
        var_r4_2 = sp18->unk_0;
        for (var_r6 = 0; var_r6 < 2; var_r6++) {
            var_r5_2 = (var_r6 == 1);
            var_r4_2[0] = ov96_0220ECA4(arg1, sp10, 0, 0, 0, 3, var_r5_2);
            var_r4_2[2] = ov96_0220ECA4(arg1, sp10, 0, 0, 2, 0x14, var_r5_2);
            var_r4_2++;
        }
        sp18->unk_10 = ov96_0220ED34(arg1, sp10, 0, 0, 0, 1);
        sp18->unk_14 = ov96_0220ED34(arg1, sp10, 0, 0, 4, 2);
        sp18++;
    }
}

#include "global.h"

#include "sprite.h"
#include "overlay_96_sprite_res_man.h"

void ov96_021E9290(Ov96SpriteResMan *man);

void ov96_021EB138(Ov96SpriteSet *set);

void ov96_021EB138(Ov96SpriteSet *set) {
    ov96_021E9290(set->man);
}

void ov96_021EB144(Ov96SpriteSet *set, s32 arg1);

void ov96_021EB144(Ov96SpriteSet *set, s32 arg1) {
    s32 flag;
    u8 v0;

    v0 = 0;
    if (set->count > 0) {
        if (arg1 == 0) {
            flag = 1;
        } else {
            flag = 0;
        }
        do {
            Sprite_SetAnimActiveFlag(ov96_021E8BAC(ov96_021EAA20(ov96_021EAA04(set, v0))), flag);
            v0 += 1;
        } while ((s32)v0 < set->count);
    }
}

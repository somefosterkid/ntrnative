#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "sprite.h"

Sprite *ov96_021EB5B8(void *param_1);

void ov96_021EB630(void *param_1, s32 arg1);

void ov96_02208784(Ov96HurdleHud *hud, s32 param_2);

void ov96_02208784(Ov96HurdleHud *hud, s32 param_2) {
    s32 i;
    u8 k;
    u8 ord[4];

    for (i = 0; i < 2; i++) {
        Sprite_SetDrawPriority(hud->digits[i], 2);
    }
    for (i = 0; i < 2; i++) {
        Sprite_SetDrawPriority(ov96_021EB5B8(hud->unk_130[i]), 2);
    }
    k = 1;
    for (i = 0; i < 4; i++) {
        if (i == param_2) {
            ord[0] = i;
        }
    }
    for (i = 0; i < 4; i++) {
        if (ord[0] != i) {
            ord[k++] = i;
        }
    }
    for (i = 0; i < 4; i++) {
        ov96_021EB630(hud->players[ord[i]].unk_10, 3 + i * 2);
        ov96_021EB630(hud->players[ord[i]].unk_0C, 4 + i * 2);
        ov96_021EB630(hud->players[ord[i]].unk_14, i + 11);
    }
}

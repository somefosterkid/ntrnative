#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"

void ov96_021EB564(void *, s32);

void ov96_021EB52C(void *, s32, s32);

void ov96_02208864(Ov96HurdleHud *hud, s32 param_2, s32 param_3, s32 param_4);

void ov96_02208864(Ov96HurdleHud *hud, s32 param_2, s32 param_3, s32 param_4) {
    ov96_021EB564(hud->unk_17C[param_2][hud->unk_1A2], 2);
    ov96_021EB52C(hud->unk_17C[param_2][hud->unk_1A2], 1, 1);
    hud->unk_1A2 = (hud->unk_1A2 + 1) % 2;
    return;
}

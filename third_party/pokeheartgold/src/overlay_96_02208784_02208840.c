#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96hurdle_hud.h"

void ov96_021EB564(void *param_1, s32 param_2);

void ov96_02208840(Ov96HurdleHud *hud, s32 param_2, u32 param_3);

void ov96_02208840(Ov96HurdleHud *hud, s32 param_2, u32 param_3) {
    if (9 < param_3) {
        GF_AssertFail();
    }
    ov96_021EB564(hud->badges[param_2].unk_4, param_3 + 9);
    return;
}

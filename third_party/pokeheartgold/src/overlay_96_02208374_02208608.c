#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96hurdle_hud.h"

s32 ov96_02208608(Ov96HurdleHud *hud, u32 arg1);

s32 ov96_02208608(Ov96HurdleHud *hud, u32 arg1) {
    s32 v0;

    if ((arg1 == 0) || (arg1 >= 4U)) {
        GF_AssertFail();
    }
    v0 = arg1 - 1;
    hud->unk_1BC[v0].unk_0 = hud->unk_1BC[v0].unk_0 + 0x20;
    if (hud->unk_1BC[v0].unk_0 >= 0x58U) {
        hud->unk_1BC[v0].unk_0 = 0;
        hud->unk_1BC[v0].unk_2 = hud->unk_1BC[v0].unk_2 + 1;
    }
    if ((u32)hud->unk_1BC[v0].unk_2 >= 3U) {
        hud->unk_1BC[v0].unk_2 = 0;
        return 1;
    }
    return 0;
}

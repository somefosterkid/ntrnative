#include "global.h"

#include "bg_window.h"
#include "font_types_def.h"
#include "render_window.h"
#include "unk_02005D10.h"
#include "overlay_68_move_relearner.h"

void ov68_021E6C74(MoveRelearner *param_1);

void ov68_021E6C74(MoveRelearner *param_1) {
    ClearFrameAndWindow2(&param_1->windows[6], 1);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[6]);
    return;
}

BOOL ov68_021E6C8C(TextPrinterTemplate *template, u16 glyphId);

BOOL ov68_021E6C8C(TextPrinterTemplate *template, u16 glyphId) {
    BOOL BVar1;

    switch (glyphId) {
    case 1:
        BVar1 = GF_IsAnySEPlaying();
        return BVar1;
    case 2:
        BVar1 = IsFanfarePlaying();
        return BVar1;
    case 3:
        PlaySE(0x5e6);
        break;
    case 4:
        PlayFanfare(0x4a0);
        break;
    case 5:
        BVar1 = IsSEPlaying(0x5e6);
        return BVar1;
    }
    return 0;
}

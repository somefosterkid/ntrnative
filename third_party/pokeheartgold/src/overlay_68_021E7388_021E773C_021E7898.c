#include "global.h"

#include "bg_window.h"
#include "unk_02019BA4.h"
#include "overlay_68_move_relearner.h"

void ov68_021E66A0(MoveRelearner *relearner, u8 x, u8 y, u8 width, u8 height, u8 srcX, u8 srcY);

void ov68_021E7898(MoveRelearner *relearner, s32 active);

void ov68_021E7898(MoveRelearner *relearner, s32 active) {
    if (active == 1) {
        GridInputHandler_SetEnabledFlag(relearner->gridInput, 7U);
        ScheduleWindowCopyToVram(&relearner->windows[8]);
        ov68_021E66A0(relearner, 0xF, 0x14, 8, 4, 0x8, 12);
        relearner->learnButtonActive = 1;
        return;
    }
    GridInputHandler_ClearEnabledFlag(relearner->gridInput, 7U);
    ClearWindowTilemapAndScheduleTransfer(&relearner->windows[8]);
    ov68_021E66A0(relearner, 0xF, 0x14, 8, 4, 0, 0xC);
    relearner->learnButtonActive = 0;
}

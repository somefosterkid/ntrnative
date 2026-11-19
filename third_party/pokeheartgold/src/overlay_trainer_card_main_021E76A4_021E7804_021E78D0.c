#include "global.h"

#include "bg_window.h"
#include "trainer_card_main.h"

void ov51_021E78D0(TrainerCardMain *cardMain, int hide) {
    if (hide == 1) {
        ClearWindowTilemapAndScheduleTransfer(&cardMain->windows[13]);
        ClearWindowTilemapAndScheduleTransfer(&cardMain->windows[12]);
        return;
    }
    ScheduleWindowCopyToVram(&cardMain->windows[13]);
    ScheduleWindowCopyToVram(&cardMain->windows[12]);
    return;
}

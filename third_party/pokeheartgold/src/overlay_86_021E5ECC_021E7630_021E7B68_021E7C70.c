#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"

void ov86_021E7C70(FrontierRecordsApp *param_1);

void ov86_021E7C70(FrontierRecordsApp *param_1) {
    u32 uVar1;
    Window *window;
    uVar1 = 5;
    window = &param_1->windows[5];
    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0xe - 1));
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[21]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[22]);
    return;
}

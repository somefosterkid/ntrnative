#include "global.h"

#include "bg_window.h"
#include "overlay_31.h"
#include "render_window.h"

void ov31_0225EDA0(MartSubScreen *param_1) {
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[MART_SUBSCREEN_WIN_SELECTED_ITEM]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[MART_SUBSCREEN_WIN_UNK_7]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[MART_SUBSCREEN_WIN_UNK_8]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[MART_SUBSCREEN_WIN_UNK_9]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[MART_SUBSCREEN_WIN_OWNED]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[MART_SUBSCREEN_WIN_TOTAL]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX]);
    ClearFrameAndWindow2(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], 1);
    ClearFrameAndWindow2(&param_1->msgWindow, 0);
    RemoveWindow(&param_1->windows[MART_SUBSCREEN_WIN_SELECTED_ITEM]);
    RemoveWindow(&param_1->windows[MART_SUBSCREEN_WIN_UNK_7]);
    RemoveWindow(&param_1->windows[MART_SUBSCREEN_WIN_UNK_8]);
    RemoveWindow(&param_1->windows[MART_SUBSCREEN_WIN_UNK_9]);
    RemoveWindow(&param_1->windows[MART_SUBSCREEN_WIN_OWNED]);
    RemoveWindow(&param_1->windows[MART_SUBSCREEN_WIN_TOTAL]);
    RemoveWindow(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX]);
    return;
}

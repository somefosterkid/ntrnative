#include "global.h"

#include "bg_window.h"
#include "overlay_31.h"
#include "render_window.h"

void ov31_0225D83C(MartSubScreen *arg0, u32 arg1) {
    switch (arg1) {
    case 1:
        ov31_0225DD14(arg0);
        ov31_0225DF98(arg0);
        ov31_0225D684(arg0, 0);
        return;
    case 2:
        ov31_0225D9D4(arg0, 1);
        ov31_0225DCF4(arg0);
        ClearWindowTilemapAndScheduleTransfer(&arg0->pageWindow);
        ov31_0225EC58(arg0);
        ov31_0225E184(arg0);
        ov31_0225E20C(arg0, arg0->martData->item);
        ov31_0225E2D4(arg0, arg0->martData->quantity);
        ov31_0225E474(arg0);
        ov31_0225D684(arg0, 1);
        ov31_0225E54C(arg0);
        return;
    case 3:
        ov31_0225D9D4(arg0, 2);
        ov31_0225DCF4(arg0);
        ov31_0225EC58(arg0);
        ov31_0225E184(arg0);
        ov31_0225E20C(arg0, arg0->martData->item);
        ov31_0225E5FC(arg0);
        ClearWindowTilemapAndScheduleTransfer(&arg0->pageWindow);
        ClearWindowTilemapAndScheduleTransfer(&arg0->unk_64);
        return;
    case 4:
        ov31_0225D9D4(arg0, 0);
        ov31_0225EDA0(arg0);
        ov31_0225DF98(arg0);
        ov31_0225D684(arg0, 0);
        ov31_0225DCA8(arg0);
        ClearFrameAndWindow2(&arg0->msgWindow, 1);
        ov31_0225DD14(arg0);
        ov31_0225DE84(arg0);
        ScheduleWindowCopyToVram(&arg0->unk_64);
        return;
    case 5:
        ov31_0225D9D4(arg0, 0);
        ov31_0225DF98(arg0);
        ClearFrameAndWindow2(&arg0->msgWindow, 0);
        ov31_0225DD14(arg0);
        ov31_0225DE84(arg0);
        return;
    case 6:
        ov31_0225E2D4(arg0, arg0->martData->quantity);
        return;
    case 7:
        ClearWindowTilemapAndScheduleTransfer(&arg0->unk_64);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[MART_SUBSCREEN_WIN_UNK_9]);
        ov31_0225E5FC(arg0);
        return;
    case 8:
        ov31_0225E700(arg0);
        return;
    case 9:
        ov31_0225E7D4(arg0);
        return;
    case 10:
        ov31_0225EA08(arg0);
        return;
    case 11:
        ClearWindowTilemapAndScheduleTransfer(&arg0->unk_64);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[MART_SUBSCREEN_WIN_UNK_9]);
        ov31_0225EA9C(arg0);
        return;
    case 12:
        ov31_0225EB30(arg0);
        return;
    case 13:
        ov31_0225EBC4(arg0);
        return;
    }
}

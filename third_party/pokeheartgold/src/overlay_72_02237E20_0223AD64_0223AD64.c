#include "global.h"

#include "bg_window.h"
#include "overlay_72_wfc_connect.h"

void ov72_0223AD64(NintendoWfcConnect *param_1);

void ov72_0223AD64(NintendoWfcConnect *param_1) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    window = param_1->buttonWindows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 3);
    RemoveWindow(&param_1->numberInputWindow);
    return;
}

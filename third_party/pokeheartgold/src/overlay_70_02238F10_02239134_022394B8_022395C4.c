#include "global.h"

#include "bg_window.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_022395C4(GTSApplicationState *param_1);

void ov70_022395C4(GTSApplicationState *param_1) {
    s32 iVar1;
    Window *window;

    RemoveWindow(&param_1->unk_1198);
    RemoveWindow(&param_1->confirmationWindow);
    RemoveWindow(&param_1->bottomInstructionWindow);
    iVar1 = 0;
    window = param_1->menuButtonWindows;
    do {
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 3);
    RemoveWindow(&param_1->titleWindow);
    return;
}

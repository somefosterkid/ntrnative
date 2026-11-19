#include "global.h"

#include "menu_input_state.h"
#include "overlay_113.h"

void ov113_021E5B60(UnownReportData *arg0);

void ov113_021E5B60(UnownReportData *arg0) {
    MenuInputStateMgr_SetState(arg0->args->menuInputStatePtr, arg0->menuInputState);
}

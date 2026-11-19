#include "global.h"

#include "system.h"
#include "overlay_113.h"

s32 ov113_021E5ED0(UnownReportData *arg0, s32 arg1);

s32 ov113_021E5F48(UnownReportData *arg0);

s32 ov113_021E5F48(UnownReportData *arg0) {
    int new_var;
    if (0xCF3 & gSystem.newKeys) {
        arg0->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (gSystem.newKeys & 2) {
        return ov113_021E5ED0(arg0, 0);
    }
    new_var = 0x40;
    new_var = new_var & gSystem.newKeys;
    if (new_var) {
        return ov113_021E5ED0(arg0, 1);
    }
    if (0x80 & gSystem.newKeys) {
        return ov113_021E5ED0(arg0, 2);
    }
    return 2;
}

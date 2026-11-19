#include "global.h"

#include "overlay_72_wfc_connect.h"

void ov72_0223AED0(NintendoWfcConnect *arg0, s8 arg1, s8 arg2, s8 arg3);

void ov72_0223AED0(NintendoWfcConnect *arg0, s8 arg1, s8 arg2, s8 arg3) {
    arg0->buttonFlash.isBgRect = 1;
    arg0->buttonFlash.timer = 0;
    arg0->buttonFlash.state = 0;
    arg0->buttonFlash.target = 5;
    arg0->buttonFlash.pressedPltt = 1;
    arg0->buttonFlash.normalPltt = 0;
    arg0->buttonFlash.x = arg1;
    arg0->buttonFlash.y = arg2;
    arg0->buttonFlash.width = 6;
    arg0->buttonFlash.height = 6;
    arg0->numberInput.nextState = arg3;
    arg0->numberInput.state = 0xA;
}

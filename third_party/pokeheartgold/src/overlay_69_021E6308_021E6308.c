#include "global.h"

#include "geonet_globe.h"
#include "system.h"

void ov69_021E645C(s32 x, s32 y, s32 *horizontalDir, s32 *horizontalDelta, s32 *verticalDir, s32 *verticalDelta);

void ov69_021E6308(GeonetGlobe *globe);

void ov69_021E6308(GeonetGlobe *globe) {
    s32 iVar1;
    s32 auStack_14;
    s32 uStack_18;
    s32 uStack_1c;
    s32 uStack_20;
    iVar1 = 0;
    globe->touchKeys = 0;
    if (!((((gSystem.touchX < 0xc0) || (0x100 < gSystem.touchX)) || (gSystem.touchY < 0xA0)) || (0xB8 < gSystem.touchY))) {
        iVar1 = 2;
    } else if (((gSystem.touchX <= (0x41 - 1)) && ((0x9F + 1) <= gSystem.touchY)) && (gSystem.touchY <= (0xB9 - 1))) {
        iVar1 = 0x400;
    }
    if (gSystem.touchNew != 0) {
        if (iVar1 != 0) {
            globe->touchKeys = iVar1;
            return;
        }
        globe->touchDragPhase = 0;
        globe->touchDragX = 0;
        globe->touchDragY = 0;
        globe->touchHoldTimer = 0;
        globe->touchKeys = 0;
        globe->lastTouchX = gSystem.touchX;
        globe->lastTouchY = gSystem.touchY;
        globe->touchHoldTimer = 4;
    }
    if (gSystem.touchHeld != 0) {
        if (globe->touchDragPhase != 0) {
            if (globe->touchDragPhase != 1) {
                return;
            }
        } else if (globe->touchHoldTimer == 0) {
            globe->touchDragPhase = globe->touchDragPhase + 1;
        } else {
            globe->touchHoldTimer = globe->touchHoldTimer + (-1);
        }
        if (iVar1 == 0) {
            ov69_021E645C(globe->lastTouchX, globe->lastTouchY, &auStack_14, &uStack_18, &uStack_1c, &uStack_20);
            globe->touchKeys = auStack_14 | uStack_1c;
            globe->touchDragX = uStack_18;
            globe->touchDragY = uStack_20;
            globe->lastTouchX = gSystem.touchX;
            globe->lastTouchY = gSystem.touchY;
            return;
        }
    } else {
        if (((iVar1 == 0) && (globe->touchHoldTimer != 0)) && ((globe->touchDragX < 4) && (globe->touchDragY < 4))) {
            globe->touchKeys = 1;
        }
        globe->touchDragPhase = 0;
        globe->touchDragX = 0;
        globe->touchDragY = 0;
        globe->touchHoldTimer = 0;
    }
    return;
}

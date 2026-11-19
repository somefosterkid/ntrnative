#include "global.h"

#include "geonet_globe.h"

BOOL ov69_021E7198(GeonetGlobe *globe, int newKeys, int heldKeys);

BOOL ov69_021E7198(GeonetGlobe *globe, int newKeys, int heldKeys) {
    u16 v0;
    u16 v1;
    s16 v2;
    s16 v3;
    BOOL v4 = 0;

    v2 = globe->rotationAngle.x;
    v3 = globe->rotationAngle.y;

    if ((newKeys & PAD_BUTTON_A) || (globe->touchKeys & PAD_BUTTON_A)) {
        if (globe->canRotateFreely == 1) {
            if (globe->zoomMode == 0) {
                globe->zoomMode = 1;
            } else {
                globe->zoomMode = 0;
            }
        }

        v4 = 1;
        return v4;
    }

    if (globe->zoomMode == 0) {
        if ((globe->touchDragX) || (globe->touchDragY)) {
            v0 = 0x200 / 6 * globe->touchDragX;
            v1 = 0x200 / 6 * globe->touchDragY;
        } else {
            v0 = 0x200;
            v1 = 0x200;
        }
    } else {
        if ((globe->touchDragX) || (globe->touchDragY)) {
            v0 = 0x20 / 3 * globe->touchDragX;
            v1 = 0x20 / 3 * globe->touchDragY;
        } else {
            v0 = 0x20;
            v1 = 0x20;
        }
    }

    if ((heldKeys & PAD_KEY_LEFT) || (globe->touchKeys & PAD_KEY_LEFT)) {
        if (globe->canRotateFreely == 1) {
            globe->rotationAngle.y += v0;
        } else {
            if (v3 < (s16)0xd820) {
                globe->rotationAngle.y += v0;
            }
        }

        v4 = 1;
    }

    if ((heldKeys & PAD_KEY_RIGHT) || (globe->touchKeys & PAD_KEY_RIGHT)) {
        if (globe->canRotateFreely == 1) {
            globe->rotationAngle.y -= v0;
        } else {
            if (v3 > (s16)0xcc80) {
                globe->rotationAngle.y -= v0;
            }
        }

        v4 = 1;
    }

    if ((heldKeys & PAD_KEY_UP) || (globe->touchKeys & PAD_KEY_UP)) {
        if (globe->canRotateFreely == 1) {
            if ((v2 + v1) < (0x4000 - 0x200)) {
                globe->rotationAngle.x += v1;
            } else {
                globe->rotationAngle.x = (0x4000 - 0x200);
            }
        } else {
            if (v2 < (s16)0x2020) {
                globe->rotationAngle.x += v1;
            }
        }

        v4 = 1;
    }

    if ((heldKeys & PAD_KEY_DOWN) || (globe->touchKeys & PAD_KEY_DOWN)) {
        if (globe->canRotateFreely == 1) {
            if ((v2 - v1) > (-0x4000 + 0x200)) {
                globe->rotationAngle.x -= v1;
            } else {
                globe->rotationAngle.x = (-0x4000 + 0x200);
            }
        } else {
            if (v2 > (s16)0x1300) {
                globe->rotationAngle.x -= v1;
            }
        }

        v4 = 1;
    }

    return v4;
}

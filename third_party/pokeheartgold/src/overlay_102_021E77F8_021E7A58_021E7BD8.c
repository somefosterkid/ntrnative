#include "global.h"

#include "touchscreen.h"

extern TouchscreenHitbox ov102_021EC610;

void ov102_021E7BD8(void);

void ov102_021E7BD8(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&ov102_021EC610);
}

extern TouchscreenHitbox ov102_021EC620;

void ov102_021E7BE8(void);

void ov102_021E7BE8(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&ov102_021EC620);
}

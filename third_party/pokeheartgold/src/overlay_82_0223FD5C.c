#include "global.h"

#include "bg_window.h"

void ov82_0223FD5C(Window *windows);

void ov82_0223FD5C(Window *windows) {
    for (u16 i = 0; i < 4; i++) {
        RemoveWindow(&windows[i]);
    }
}

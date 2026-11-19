#include "global.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6070(void *arg0);
void ov14_021F6094(void *arg0);
void ov14_021F60A8(void *arg0);

void ov14_021F6070(void *arg0) {
    ov14_021F605C(arg0);
    ov14_021F5950(arg0, 0x20, 0x1A, 0);
    ov14_021F5950(arg0, 0x21, 0x1B, 0);
}

void ov14_021F6094(void *arg0) {
    ov14_021F5BD8(arg0, 0x29, 0x4E, 0, 3);
}

void ov14_021F60A8(void *arg0) {
    ov14_021F5BD8(arg0, 0x29, 0x4F, 0, 3);
}

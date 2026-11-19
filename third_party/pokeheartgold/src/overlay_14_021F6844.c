#include "global.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6698(void *, s32);
void ov14_021F6844(void *arg0, s32 arg1);

void ov14_021F6844(void *arg0, s32 arg1) {
    s32 v0;

    v0 = arg1;
    switch (v0) {
    case 0:
        v0 = 7;
        break;
    case 1:
        v0 = 0xA;
        break;
    }
    ov14_021F6698(arg0, v0);
}

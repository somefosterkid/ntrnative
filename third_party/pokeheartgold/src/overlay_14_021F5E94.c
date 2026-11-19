#include "global.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F5C94(void **, s32, s32);
void ov14_021F5E94(void **arg0);
void ov14_021F5EB4(void *arg0, s32 arg1);
void ov14_021F5EC4(void *arg0, s32 arg1);

void ov14_021F5E94(void **arg0) {
    if (*(s32 *)((u8 *)(*arg0) + 8) == 3) {
        ov14_021F5C94(arg0, 0x19, 0x40);
        return;
    }
    ov14_021F5C94(arg0, 0x19, 0x3D);
}

void ov14_021F5EB4(void *arg0, s32 arg1) {
    ov14_021F5950(arg0, 0x1A, 0x3E, arg1);
}

void ov14_021F5EC4(void *arg0, s32 arg1) {
    ov14_021F5950(arg0, 0x1B, 0x4C, arg1);
}

void ov14_021F5ED4(void *arg0, s32 arg1) {
    ov14_021F5950(arg0, 0x1C, 0x47, arg1);
}

#include "global.h"

#include "camera.h"

s32 ov01_021EAEE0(s32, s32, s32);

void ov01_021EAEA4(Camera *arg0, void *arg1, s32 arg2, s32 arg3);

void ov01_021EAEA4(Camera *arg0, void *arg1, s32 arg2, s32 arg3) {
    VecFx32 sp0 = { 0, 0, 0 };
    sp0.x = ov01_021EAEE0(*((s32 *)arg1), arg2, arg3);
    sp0.z = ov01_021EAEE0(*((s32 *)((u8 *)arg1 + 8)), arg2, arg3);
    Camera_OffsetLookAtPosAndTarget(&sp0, arg0);
}

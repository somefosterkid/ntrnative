#include "global.h"

#include "camera.h"

void ov01_021EAE50(Camera *arg0, u16 *arg1, u16 *arg2, s32 arg3, u8 arg4);

void ov01_021EAE50(Camera *arg0, u16 *arg1, u16 *arg2, s32 arg3, u8 arg4) {
    s32 var_r0;
    u16 temp_r0;
    u16 temp_r4;
    u16 temp_r1;
    CameraAngle sp0 = { 0, 0, 0, 0 };

    temp_r4 = *arg1;
    temp_r0 = *arg2;
    if ((u32)temp_r0 >= (u32)temp_r4) {
        temp_r1 = (u16)(temp_r0 - temp_r4);
        var_r0 = (s32)(temp_r1 * arg3) / (s32)arg4;
    } else {
        temp_r1 = (u16)(temp_r4 - temp_r0);
        var_r0 = 0 - ((s32)(temp_r1 * arg3) / (s32)arg4);
    }
    sp0.x = temp_r4 + var_r0;
    Camera_SetAnglePos(&sp0, arg0);
}

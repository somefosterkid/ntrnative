#include "global.h"

#include "camera.h"

void ov01_021E95CC(Camera *arg0, u8 *arg1);

void ov01_021E9610(Camera *camera, u8 *param1);

void ov01_021E95CC(Camera *camera, u8 *arg1) {
    s32 v1;
    u16 v0;
    u8 v2;

    v2 = 0xC;
    v1 = *arg1 * 0xC;
    if (v1 <= 0x60) {
        v0 = (u16)v1;
        if ((u32)v0 > 0x60U) {
            v2 = v0 - 0x60;
        }
        if ((u16)(Camera_GetPerspectiveAngle(camera) - v2) != 0) {
            Camera_AdjustPerspectiveAngle((u16)(0 - v2), camera);
        }
        *arg1 += 1;
    }
}

void ov01_021E9610(Camera *camera, u8 *param1) {
    u8 v0;
    u16 v1;

    if ((*param1) * 16 >= 96) {
        return;
    }

    v1 = (*param1) * 16;

    if (v1 > 96) {
        v0 = v1 - 96;
    } else {
        v0 = 16;
    }

    {
        u16 v2;

        v2 = Camera_GetPerspectiveAngle(camera);

        if ((u16)(v2 + v0) > 0x0) {
            Camera_AdjustPerspectiveAngle(v0, camera);
        }

        (*param1)++;
    }
}

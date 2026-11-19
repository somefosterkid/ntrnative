#include "global.h"
struct ChooseStarterApp;

#include "error_handling.h"
#include "overlay_01_choose_starter_app.h"

void ov96_021EB06C(struct ChooseStarterApp *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);

void ov96_021EB06C(ChooseStarterApp *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 temp_r3;
    s32 temp_r4;
    s32 temp_r5;
    u32 temp_r2;

    if (*((s32 *)&arg0->chooseStarterStep) == 0) {
        GF_AssertFail();
        return;
    }
    temp_r5 = arg0->cameraMovement.xRotation.frameCountRange * 2;
    temp_r3 = arg0->cameraMovement.distance.frameCountRange;
    temp_r2 = temp_r5 - temp_r3;
    temp_r4 = arg2 - (temp_r3 + ((s32)(temp_r2 + (temp_r2 >> 0x1F)) >> 1));
    temp_r3 = (temp_r4 + temp_r5) - arg0->cameraMovement.xRotation.frameCount;
    *arg3 = arg1;
    *arg4 = temp_r3;
}

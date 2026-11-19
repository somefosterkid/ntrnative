#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "camera.h"

extern s32 ov96_0221C0F4;

void ov96_021F6398(UnkStruct_ov96_021F5018 *arg0);

void ov96_021F6398(UnkStruct_ov96_021F5018 *arg0) {
    VecFx32 sp20;
    VecFx32 sp14;
    CameraAngle spC;

    sp14 = *(VecFx32 *)((u8 *)&ov96_0221C0F4);
    arg0->unk_194.x = 0;
    arg0->unk_194.y = 0;
    arg0->unk_194.z = 0;
    spC.x = 0;
    spC.y = 0;
    spC.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&arg0->unk_194, FX32_CONST(100), &spC, 0xA4U, 1U, 1, arg0->unk_190);
    Camera_OffsetLookAtPosAndTarget(&sp14, arg0->unk_190);
    Camera_SetPerspectiveClippingPlane(FX32_CONST(4), FX32_CONST(256), arg0->unk_190);
    sp20.x = 0;
    sp20.y = 0x1000;
    sp20.z = 0;
    Camera_SetLookAtCamUp((VecFx32 *)&sp20, arg0->unk_190);
    Camera_SetStaticPtr(arg0->unk_190);
}

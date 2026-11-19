#include "global.h"

#include "camera.h"

extern u8 ov105_021E5DCC;

extern u8 ov105_021E5DCE;

extern u8 ov105_021E5DF0;

extern u8 ov105_021E5E2C;

extern u8 ov105_021E5E38;

extern u8 ov105_021E5E3A;

void ov105_021E5BCC(void **param_1);

void ov105_021E5BCC(void **param_1) {
    s32 iVar1;
    s32 iVar2;
    u32 uVar3;

    VecFx32 zero = { 0, 0, 0 };
    VecFx32 *target;

    target = (VecFx32 *)(param_1 + 0x24);
    *target = zero;
    uVar3 = (u32) * (u8 *)((u8 *)param_1 + 0x9f);
    iVar2 = uVar3 * 0x14;
    iVar1 = (u32) * (u8 *)(param_1 + 0x27) * 0x14;
    Camera_Init_FromTargetDistanceAndAngle((VecFx32 *)(param_1 + 0x24), *(fx32 *)(&ov105_021E5E2C + iVar1 + iVar2), (CameraAngle *)(&ov105_021E5E2C + iVar1 + iVar2 + 4), *(u16 *)(&ov105_021E5E3A + iVar1 + iVar2), (&ov105_021E5E38)[iVar1 + iVar2], 1, (Camera *)*param_1);
    Camera_OffsetLookAtPosAndTarget((VecFx32 *)(&ov105_021E5DF0 + (u32) * (u8 *)(param_1 + 0x27) * 0xc + uVar3 * 0xc),
        (Camera *)*param_1);
    Camera_SetPerspectiveClippingPlane((u32) * (u16 *)(&ov105_021E5DCC + (u32) * (u8 *)(param_1 + 0x27) * 4 + uVar3 * 4) << 0xc, (u32) * (u16 *)(&ov105_021E5DCE + (u32) * (u8 *)(param_1 + 0x27) * 4 + uVar3 * 4) << 0xc, (Camera *)*param_1);
    Camera_SetStaticPtr((Camera *)*param_1);
    return;
}

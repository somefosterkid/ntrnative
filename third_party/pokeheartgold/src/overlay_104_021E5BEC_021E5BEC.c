#include "global.h"
#include "camera.h"

struct Camera;

struct CameraAngle;

void Camera_SetStaticPtr(Camera *camera);

void Camera_SetPerspectiveClippingPlane(fx32 near, fx32 far, Camera *camera);

void Camera_Init_FromTargetDistanceAndAngle(VecFx32 *target, fx32 distance, const CameraAngle *angle, u16 perspectiveAngle, u8 perspectiveType, BOOL setReference, Camera *camera);

void Camera_OffsetLookAtPosAndTarget(const VecFx32 *delta, Camera *camera);

extern const u8 ov104_021E5FD0[];

extern const u8 ov104_021E5FDC[];

extern const u8 ov104_021E5FDE[];

extern const u8 ov104_021E5F88[];

extern const u8 ov104_021E5F24[];

extern const u8 ov104_021E5F26[];

void ov104_021E5BEC(u8 *data);

void ov104_021E5BEC(u8 *data) {
    VecFx32 zero = { 0, 0, 0 };
    VecFx32 *target;
    u32 idx;
    u32 off14, off3c;

    target = (VecFx32 *)(data + 0x158);
    *target = zero;
    idx = data[0x168];
    off14 = idx * 0x14;
    off3c = data[0x164] * 0x3C;
    Camera_Init_FromTargetDistanceAndAngle((VecFx32 *)(data + 0x158),
        *(const fx32 *)(ov104_021E5FD0 + off3c + off14),
        (const CameraAngle *)(ov104_021E5FD0 + off3c + off14 + 4),
        *(const u16 *)(ov104_021E5FDE + off3c + off14),
        ov104_021E5FDC[off3c + off14],
        1,
        *(Camera **)data);
    Camera_OffsetLookAtPosAndTarget((const VecFx32 *)(ov104_021E5F88 + data[0x164] * 0x24 + idx * 0xC), *(Camera **)data);
    Camera_SetPerspectiveClippingPlane(
        (fx32)(*(const u16 *)(ov104_021E5F24 + data[0x164] * 0xC + idx * 4) << 12),
        (fx32)(*(const u16 *)(ov104_021E5F26 + data[0x164] * 0xC + idx * 4) << 12),
        *(Camera **)data);
    Camera_SetStaticPtr(*(Camera **)data);
}

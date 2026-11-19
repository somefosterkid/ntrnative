#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "error_handling.h"
#include "field_camera_template.h"
#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021EABA8.h"

s32 ov01_021EAC6C(FieldSystem *, u32);

void FieldCamera_Create(VecFx32 *_target, FieldSystem *fieldSystem, u32 cameraType, s32 withHistory);

void FieldCamera_Create(VecFx32 *_target, FieldSystem *fieldSystem, u32 cameraType, s32 withHistory) {
    VecFx32 *spC;
    FieldCameraTemplate *temp_r4;

    spC = _target;
    temp_r4 = &ov01_02206478[cameraType];
    if (cameraType >= 0x11U) {
        GF_AssertFail();
    }
    fieldSystem->camera = Camera_New(HEAP_ID_FIELD1);
    Camera_Init_FromTargetDistanceAndAngle(spC, temp_r4->distance, &temp_r4->angle, temp_r4->perspectiveAngle, (u8)temp_r4->perspectiveType, 1, fieldSystem->camera);
    Camera_SetStaticPtr(fieldSystem->camera);
    Camera_SetPerspectiveClippingPlane(temp_r4->nearClip, temp_r4->farClip, fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&temp_r4->lookAtOffset, fieldSystem->camera);
    if (withHistory != 0) {
        Camera_History_New(7, 6, 2, HEAP_ID_FIELD1, fieldSystem->camera);
    }
    fieldSystem->unk28 = ov01_021EAC4C(HEAP_ID_FIELD1);
    ov01_021EAC6C(fieldSystem, cameraType);
}

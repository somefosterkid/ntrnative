#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"

typedef struct {
    u32 unk_00_0 : 1;
    u32 unk_00_1 : 1;
    u32 : 30;
    u32 unk_04_0 : 31;
    u32 : 1;
} UnkStruct_ov69_0225DA74;

typedef struct {
    Camera *camera;
    CameraAngle unk_04;
    fx32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkStruct_ov69_0225DAEC;

extern const VecFx32 ov48_0225B184;

extern const VecFx32 ov48_0225B16C;

void ov48_02259BC0(UnkStruct_ov69_0225DAEC *param0, UnkStruct_ov69_0225DA74 param1, enum HeapID heapID);

void ov48_02259BC0(UnkStruct_ov69_0225DAEC *param0, UnkStruct_ov69_0225DA74 param1, enum HeapID heapID) {
    param0->camera = Camera_New(heapID);

    Camera_Init_FromTargetAndPos(&ov48_0225B184, &ov48_0225B16C, 0x5c1, 0, 0, param0->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_ONE * 100, param0->camera);
    Camera_ApplyPerspectiveType(0, param0->camera);
    Camera_SetStaticPtr(param0->camera);

    if (param1.unk_00_0 == 0) {
        param0->unk_10 = 1;
        param0->unk_0C = 0x50000;
    } else {
        param0->unk_10 = 0;
        param0->unk_0C = 0x128000;
    }

    Camera_SetDistance(param0->unk_0C, param0->camera);
}

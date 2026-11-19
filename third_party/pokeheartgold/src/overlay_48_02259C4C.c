#include "global.h"

#include "unk_02005D10.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"

void ov48_02259C4C(void *arg0);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct {
    s16 unk_00;
    s16 unk_02;
    MtxFx33 unk_04;
    u16 unk_28;
    u16 unk_2A;
    u16 unk_2C;
} UnkStruct_ov69_0225CA7C_sub1_sub1;
typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225CA7C_sub1_sub1 unk_04[1024];
} UnkStruct_ov69_0225CA7C_sub1;
typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Field3dObject unk_B810[3];
    Field3dModel unk_B978[3];
} UnkStruct_ov69_0225CA7C;
typedef struct {
    Camera *camera;
    CameraAngle unk_04;
    fx32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkStruct_ov69_0225DAEC;
typedef struct FogManager {
    BOOL enabled;
    GXFogBlend mode;
    GXFogSlope slope;
    int offset;
    GXRgb color;
    int alpha;
    char densityTable[32];
} FogManager;
BOOL ov48_02259C78(UnkStruct_ov69_0225DAEC *param0, UnkStruct_ov69_0225CA7C *param1);
GXRgb ov48_02259CFC(FogManager *fogMan);

void ov48_02259C4C(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 0x10) == 0) {
        *(u16 *)((u8 *)arg0 + 0x10) = 1U;
        PlaySE(0x5D5U);
    } else {
        *(u16 *)((u8 *)arg0 + 0x10) = 0U;
        PlaySE(0x5D5U);
    }
    *(s16 *)((u8 *)arg0 + 0x12) = 1;
}

BOOL ov48_02259C78(UnkStruct_ov69_0225DAEC *param0, UnkStruct_ov69_0225CA7C *param1) {
    if (param0->unk_12 == 0) {
        return 1;
    }

    switch (param0->unk_10) {
    case 1:
        if (param0->unk_0C > (0x50000 + 0x8000)) {
            param0->unk_0C -= 0x8000;
            param1->unk_B804.x -= 0x80;
            param1->unk_B804.y = param1->unk_B804.x;
        } else {
            param0->unk_0C = 0x50000;
            param0->unk_12 = 0;
        }
        break;
    case 0:
        if (param0->unk_0C < (0x128000 - 0x8000)) {
            param0->unk_0C += 0x8000;
            param1->unk_B804.x += 0x80;
            param1->unk_B804.y = param1->unk_B804.x;
        } else {
            param0->unk_0C = 0x128000;
            param0->unk_12 = 0;
        }
        break;
    }

    Camera_SetDistance(param0->unk_0C, param0->camera);

    return 0;
}

GXRgb ov48_02259CFC(FogManager *fogMan) {
    return fogMan->color;
}

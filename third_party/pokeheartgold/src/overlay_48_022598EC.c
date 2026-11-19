#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResourceCollection;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_02018030(Field3dModel *model, NARC *narc, u32 memberIndex, u32 heapID);

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

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
    u32 unk_00_0 : 1;
    u32 unk_00_1 : 1;
    u32 : 30;
    u32 unk_04_0 : 31;
    u32 : 1;
} UnkStruct_ov69_0225DA74;

typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Field3dObject unk_B810[3];
    Field3dModel unk_B978[3];
} UnkStruct_ov69_0225CA7C;

extern const u32 ov48_0225B178[3];

void ov48_02258A80(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov66_02231560 *param1, u32 heapID);

void ov48_022598EC(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov66_02231560 *param1, UnkStruct_ov69_0225D35C *param2, UnkStruct_ov69_0225DA74 param3, u32 heapID);

void sub_02018068(Field3dModel *model);
void sub_02018288(Field3dObject *obj, const MtxFx33 *rotation);
void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Field3dObject unk_24;
    Field3dModel unk_9C;
} UnkStruct_ov69_0225D71C;
void ov48_022598AC(const UnkStruct_ov69_0225D71C *param0, VecFx32 *param1);
void ov48_022598BC(const UnkStruct_ov69_0225D71C *param0, MtxFx33 *param1);
u16 ov45_0222D6B0(const UnkStruct_ov66_02231560 *param0, u8 param1);
u8 ov45_0222D6D4(const UnkStruct_ov66_02231560 *param0, u8 param1);
BOOL ov45_0222D6FC(const UnkStruct_ov66_02231560 *param0, u8 param1);
BOOL ov45_0222D724(const UnkStruct_ov66_02231560 *param0, u32 param1);
void ov48_02259984(UnkStruct_ov69_0225CA7C *param0);
void ov48_022599A0(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov69_0225D71C *param1);
u32 ov48_02259A68(const UnkStruct_ov66_02231560 *param0, u16 param1, u16 param2);

void ov48_022598EC(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov66_02231560 *param1, UnkStruct_ov69_0225D35C *param2, UnkStruct_ov69_0225DA74 param3, u32 heapID) {
    int v0;

    ov48_02258A80(param0, param1, heapID);

    for (v0 = 0; v0 < 3; v0++) {
        sub_02018030(&param0->unk_B978[v0], param2->unk_1A8, ov48_0225B178[v0], heapID);
        sub_020181B0(&param0->unk_B810[v0], &param0->unk_B978[v0]);
    }

    if (param3.unk_00_0 == 0) {
        VEC_Set(&param0->unk_B804, 0x300, 0x300, FX32_ONE);
    } else {
        VEC_Set(&param0->unk_B804, FX32_ONE, FX32_ONE, FX32_ONE);
    }
}

void ov48_02259984(UnkStruct_ov69_0225CA7C *param0) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        sub_02018068(&param0->unk_B978[v0]);
    }
}

void ov48_022599A0(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov69_0225D71C *param1) {
    MtxFx33 v0;
    int v1;
    MtxFx33 v2;
    VecFx32 v3;

    ov48_022598AC(param1, &v3);
    ov48_022598BC(param1, &v2);

    for (v1 = 0; v1 < 3; v1++) {
        sub_020182A8(&param0->unk_B810[v1], v3.x, v3.y, v3.z);

        if (v1 == 2) {
            sub_020182C4(&param0->unk_B810[v1], param0->unk_B804.x, param0->unk_B804.y, param0->unk_B804.z + ((fx32)(((0.1) > 0) ? ((0.1) * (1 << 12) + 0.5f) : ((0.1) * (1 << 12) - 0.5f))));
        } else {
            sub_020182C4(&param0->unk_B810[v1], param0->unk_B804.x, param0->unk_B804.y, param0->unk_B804.z);
        }
    }

    {
        MTX_Identity33(&v0);
        sub_02018288(&param0->unk_B810[2], &v0);
    }

    {
        for (v1 = 0; v1 < param0->unk_00.unk_00; v1++) {
            MTX_Concat33(&param0->unk_00.unk_04[v1].unk_04, &v2, &v0);

            if (param0->unk_00.unk_04[v1].unk_28 != 3) {
                sub_02018288(&param0->unk_B810[param0->unk_00.unk_04[v1].unk_28], &v0);
            }
        }
    }
}

u32 ov48_02259A68(const UnkStruct_ov66_02231560 *param0, u16 param1, u16 param2) {
    int v0;
    BOOL v1;
    BOOL v2;
    u8 v3, v4;

    for (v0 = 0; v0 < 50; v0++) {
        v2 = ov45_0222D724(param0, (u8)v0);

        if (v2) {
            v1 = ov45_0222D6FC(param0, v0);
            v3 = ov45_0222D6B0(param0, v0);
            v4 = ov45_0222D6D4(param0, v0);

            if ((v3 == param1) && (v4 == param2)) {
                if (v1 == 1) {
                    return 1;
                }

                return 0;
            }
        }
    }

    return 3;
}

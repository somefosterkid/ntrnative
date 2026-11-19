#include "global.h"

#include "field/overlay_01_021FB878.h"

void sub_020180F8(Field3DModelAnimation *anim, NNSFndAllocator *allocator);

typedef struct {
    Field3dModel unk_00[2];
    Field3DModelAnimation unk_20[5];
    BOOL unk_84[5];
} UnkStruct_ov70_022615A4;

void ov49_0225D574(Field3dModel *param0);

void ov49_0225D6AC(UnkStruct_ov70_022615A4 *param0, NNSFndAllocator *param1);

void ov49_0225D6AC(UnkStruct_ov70_022615A4 *param0, NNSFndAllocator *param1) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        if (param0->unk_84[v0]) {
            sub_020180F8(&param0->unk_20[v0], param1);
            param0->unk_84[v0] = 0;
        }
    }

    for (v0 = 0; v0 < 2; v0++) {
        ov49_0225D574(&param0->unk_00[v0]);
    }
}

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

void sub_020181B0(Easy3DObject *obj, Field3dModel *model);

void sub_020181D4(Easy3DObject *obj, Field3DModelAnimation *anim);

void sub_020182A0(Easy3DObject *obj, BOOL visible);

typedef struct {
    BOOL unk_00;
    Easy3DObject unk_04[2];
    u32 unk_F4[5];
    fx32 unk_108[5];
    u8 pad_11C[10];
    u8 unk_126;
    u8 unk_127;
    u32 unk_128;
} UnkStruct_ov70_022615E8;

void ov49_0225D6F0(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1);

void ov49_0225D6F0(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1) {
    int v0;

    param0->unk_00 = 1;

    for (v0 = 0; v0 < 2; v0++) {
        sub_020181B0(&param0->unk_04[v0], &param1->unk_00[v0]);
        sub_020182A0(&param0->unk_04[v0], 1);
    }

    for (v0 = 0; v0 < 5 - 1; v0++) {
        if (param1->unk_84[v0] == 1) {
            param0->unk_F4[v0] = 1;

            if (v0 != 3) {
                sub_020181D4(&param0->unk_04[0], &param1->unk_20[v0]);
            } else {
                sub_020181D4(&param0->unk_04[1], &param1->unk_20[v0]);
            }
        }
    }
}

void sub_020181E0(Easy3DObject *obj, Field3DModelAnimation *anim);

void ov49_0225D76C(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1);

void ov49_0225D76C(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1) {
    int v0;

    for (v0 = 0; v0 < 5 - 1; v0++) {
        if (param0->unk_F4[v0] == 1) {
            param0->unk_F4[v0] = 0;

            if (v0 != 3) {
                sub_020181E0(&param0->unk_04[0], &param1->unk_20[v0]);
            } else {
                sub_020181E0(&param0->unk_04[1], &param1->unk_20[v0]);
            }
        }
    }

    param0->unk_00 = 0;
}

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void ov49_0225D57C(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);

void ov49_0225D7B8(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1);

void ov49_0225D7B8(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        if (param0->unk_F4[v0]) {
            ov49_0225D57C(&param0->unk_108[v0], &param1->unk_20[v0], FX32_ONE);
            sub_02018198(&param1->unk_20[v0], param0->unk_108[v0]);
        }
    }
}

#include "global.h"
#include "field/overlay_01_021FB878.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct UnkStruct_ov70_02260E8C_t;

typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

struct UnkStruct_ov70_02260E8C_t {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    Field3dObject unk_04;
    u32 unk_7C[4];
    fx32 unk_8C[4];
    VecFx32 unk_9C;
    VecFx32 unk_A8;
};

typedef struct {
    Field3dModel unk_00[2];
    void *unk_20[2][3];
    Field3DModelAnimation unk_38[2][4];
} UnkStruct_ov70_022618C8;

extern void ov49_0225D57C(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);

extern BOOL ov49_0225D5A0(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);

void ov49_0225DA70(UnkStruct_ov70_02260E8C *param0, UnkStruct_ov70_022618C8 *param1);

void ov49_0225DA70(UnkStruct_ov70_02260E8C *param0, UnkStruct_ov70_022618C8 *param1) {
    int v0;

    {

        for (v0 = 0; v0 < 4; v0++) {
            if (param0->unk_7C[v0]) {
                switch (v0) {
                case 0:
                case 2:
                    ov49_0225D57C(&param0->unk_8C[v0], &param1->unk_38[param0->unk_02][v0], ((fx32)0x0000000000001000L));
                    break;
                case 1:
                case 3: {
                    BOOL v2;

                    v2 = ov49_0225D5A0(&param0->unk_8C[v0], &param1->unk_38[param0->unk_02][v0], ((fx32)0x0000000000001000L));

                    if (v2 == 1) {
                        param0->unk_7C[v0] = 0;
                        param0->unk_8C[v0] = 0;
                    }
                } break;
                default:
                    ((0) ? (void)0 : GF_AssertFail());
                    break;
                }
            }
        }
    }
}

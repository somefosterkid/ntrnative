#include "global.h"

#include "field/overlay_01_021FB878.h"

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void sub_02018068(Field3dModel *model);
void sub_020180F8(Field3DModelAnimation *anim, NNSFndAllocator *allocator);
typedef struct {
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[6];
    u16 unk_210;
    u16 unk_212;
    Field3DModelAnimation unk_214[1];
    fx32 unk_228;
    u16 unk_22C;
    u16 unk_22E;
} UnkStruct_ov115_02263DF8;

void ov91_022600FC(UnkStruct_ov115_02263DF8 *param0, NNSFndAllocator *param1);

void ov91_022600FC(UnkStruct_ov115_02263DF8 *param0, NNSFndAllocator *param1) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        sub_02018068(&param0->unk_168[v0]);
    }

    for (v0 = 0; v0 < 6; v0++) {
        sub_020180F8(&param0->unk_198[v0], param1);
    }

    for (v0 = 0; v0 < 1; v0++) {
        sub_020180F8(&param0->unk_214[v0], param1);
    }

    memset(param0, 0, sizeof(UnkStruct_ov115_02263DF8));
}

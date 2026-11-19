#include "global.h"

#include "field/overlay_01_021FB878.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void sub_02018124(Field3DModelAnimation *anim, fx32 frameDelta);
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
void ov91_02260378(UnkStruct_ov115_02263DF8 *param0);
void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void ov91_022601AC(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1, int param2);
void ov91_022601C8(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1);

void ov91_022601AC(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1, int param2) {
    sub_02018124(&param0->unk_198[0], ((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f))));
    ov91_02260378(param0);
}

void ov91_022601C8(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        sub_020182A8(&param0->unk_00[v0], param1->unk_D8.x, param1->unk_D8.y, param1->unk_D8.z);
    }
}

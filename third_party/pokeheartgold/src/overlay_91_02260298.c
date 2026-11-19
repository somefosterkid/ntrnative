#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "error_handling.h"

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void sub_020181D4(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020181E0(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020182A0(Field3dObject *obj, BOOL visible);

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

extern const u8 ov91_0226274C[6];

void ov91_02260298(UnkStruct_ov115_02263DF8 *param0, u32 param1);

BOOL sub_0201815C(Field3DModelAnimation *anim, fx32 frameDelta);
BOOL ov91_0226031C(UnkStruct_ov115_02263DF8 *param0, fx32 param1);

void ov91_02260298(UnkStruct_ov115_02263DF8 *param0, u32 param1) {
    GF_ASSERT(param1 < 6);

    if (param0->unk_210 != param1) {
        sub_020181E0(&param0->unk_00[ov91_0226274C[param0->unk_210]], &param0->unk_198[param0->unk_210]);
        sub_020181D4(&param0->unk_00[ov91_0226274C[param1]], &param0->unk_198[param1]);
        sub_02018198(&param0->unk_198[param1], 0);
        sub_020182A0(&param0->unk_00[ov91_0226274C[param0->unk_210]], 0);
        sub_020182A0(&param0->unk_00[ov91_0226274C[param1]], 1);
        param0->unk_210 = param1;
    }
}

BOOL ov91_0226031C(UnkStruct_ov115_02263DF8 *param0, fx32 param1) {
    return sub_0201815C(&param0->unk_198[param0->unk_210], param1);
}

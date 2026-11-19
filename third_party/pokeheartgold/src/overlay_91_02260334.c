#include "global.h"
#include "field/overlay_01_021FB878.h"

s32 sub_020181D4(void *, void *);

s32 sub_020181E0(void *, void *);

void ov91_02260334(void *arg0, u16 arg1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
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
void ov91_02260364(UnkStruct_ov115_02263DF8 *param0, BOOL param1);
void ov91_02260370(UnkStruct_ov115_02263DF8 *param0, u32 param1);

void ov91_02260334(void *arg0, u16 arg1) {
    if (((UnkStruct_ov115_02263DF8 *)arg0)->unk_212 != arg1) {
        if (arg1 == 1) {
            sub_020181D4(arg0, arg0 + 0x198);
        } else {
            sub_020181E0(arg0, arg0 + 0x198);
        }
        ((UnkStruct_ov115_02263DF8 *)arg0)->unk_212 = arg1;
    }
}

void ov91_02260364(UnkStruct_ov115_02263DF8 *param0, BOOL param1) {
    param0->unk_22E = param1;
}

void ov91_02260370(UnkStruct_ov115_02263DF8 *param0, u32 param1) {
    param0->unk_22C = param1;
}

#include "global.h"
#include "field/overlay_01_021FB878.h"

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

s32 sub_02018198(Field3DModelAnimation *, s32);

extern s32 ov91_02261C12;

extern s32 ov91_02261C13;

void ov91_02260378(UnkStruct_ov115_02263DF8 *param0);

void ov91_02260378(UnkStruct_ov115_02263DF8 *param0) {
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r5;
    s32 var_r0;
    u16 temp_r0;

    if (param0->unk_22E == 1) {
        if ((s32)(param0->unk_228 + 0x2000) < 0x28000) {
            param0->unk_228 += 0x2000;
        } else {
            param0->unk_228 = (s32)(0x2000U >> 1);
        }
    } else {
        temp_r0 = param0->unk_22C;
        if (temp_r0 == 4) {
            var_r0 = 0;
        } else {
            var_r0 = temp_r0 + 1;
        }
        temp_r2 = var_r0 * 2;
        temp_r1 = *(u8 *)(((u8 *)&ov91_02261C12) + temp_r2) << 0xC;
        temp_r5 = param0->unk_228;
        if (temp_r5 < temp_r1) {
            param0->unk_228 = temp_r1;
        } else {
            temp_r0_2 = temp_r5 + 0x2000;
            if (temp_r0_2 < (s32)(*(u8 *)(((u8 *)&ov91_02261C13) + temp_r2) << 0xC)) {
                param0->unk_228 = temp_r0_2;
            } else {
                param0->unk_228 = temp_r1;
            }
        }
    }
    sub_02018198(param0->unk_214, param0->unk_228);
}

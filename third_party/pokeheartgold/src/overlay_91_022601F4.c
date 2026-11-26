#include "global.h"
#include "field/overlay_01_021FB878.h"
#include "math_util.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_020182E0(u8 *, s16, s32);
#else
s32 sub_020182E0(u8 *, u16, s32);
#endif

void ov91_022601F4(void *arg0, void *arg1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
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
void sub_020181EC(Field3dObject *obj);
void sub_02018198(Field3DModelAnimation *anim, fx32 frame);
void ov91_02260218(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1);
void ov91_0226023C(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1);
void ov91_02260254(UnkStruct_ov115_02263DF8 *param0, u32 param1);

void ov91_022601F4(void *arg0, void *arg1) {
    s32 v1;
    u8 *v2;
    u32 v0;

    v2 = arg0;
    v1 = 0;
    v0 = *(u16 *)((u8 *)arg1 + 2);
    do {
        sub_020182E0(v2, v0, 1);
        v1 += 1;
        v2 += 0x78;
    } while (v1 < 3);
}

void ov91_02260218(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        sub_020182C4(&param0->unk_00[v0], param1->unk_D4, param1->unk_D4, param1->unk_D4);
    }
}

void ov91_0226023C(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        sub_020181EC(&param0->unk_00[v0]);
    }
}

void ov91_02260254(UnkStruct_ov115_02263DF8 *param0, u32 param1) {
    fx32 v0;

    param1 -= ((20 * 0xffff) / 360);
    v0 = (param1 * 90) / GF_DegreeToSinCosIdx(90 - ((20 * 0xffff) / 360));
    v0 = (v0 * ((fx32)(((180) > 0) ? ((180) * (1 << 12) + 0.5f) : ((180) * (1 << 12) - 0.5f)))) / 90;
    v0 = ((fx32)(((180) > 0) ? ((180) * (1 << 12) + 0.5f) : ((180) * (1 << 12) - 0.5f))) - v0;

    sub_02018198(&param0->unk_198[1], v0);
}

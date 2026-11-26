#include "global.h"
#include "sprite.h"
#include "field/overlay_01_021FB878.h"

struct Field3dObject;

struct Sprite;

struct SpriteList;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_020182E0(Field3dObject *obj, s16 angle, enum RotationAxis axis);
#else
void sub_020182E0(Field3dObject *obj, u16 angle, enum RotationAxis axis);
#endif

typedef struct {
    u16 unk_00;
    u16 unk_02;
    VecFx32 unk_04;
    fx32 unk_10;
    VecFx32 unk_14;
    s32 unk_20;
    BOOL unk_24;
} UnkStruct_ov115_02262134;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    UnkStruct_ov115_02262134 unk_04;
    VecFx32 unk_2C;
    s16 unk_38;
    u16 unk_3A;
    BOOL unk_3C;
    fx32 unk_40;
} UnkStruct_ov115_022622F8;

typedef struct {
    Sprite *unk_00;
    Field3dObject unk_04;
    Field3dObject unk_7C;
    BOOL unk_F4;
    const UnkStruct_ov115_022622F8 *unk_F8;
    u16 unk_FC;
    u16 unk_FE;
    u16 unk_100;
    u16 unk_102;
    s16 unk_104;
} UnkStruct_ov115_022647A0;

fx32 ov91_0225E6F8(const UnkStruct_ov115_022622F8 *param0);

void ov91_02260B5C(UnkStruct_ov115_022647A0 *param0);

void ov91_02260B5C(UnkStruct_ov115_022647A0 *param0) {
    fx32 v0;
    u16 v1;
    u16 v2;

    param0->unk_104--;

    if (param0->unk_104 < 0) {
        v0 = ov91_0225E6F8(param0->unk_F8);
        v2 = ((v0 * (1024 - 512)) / (0x8000 + 0x16200));
        v1 = FX_Atan2Idx(param0->unk_F8->unk_04.unk_04.z, param0->unk_F8->unk_04.unk_04.x);

        param0->unk_100 = 512 + (FX_MulInline(FX_SinIdx(v1), v2 << 12) >> 12);
        param0->unk_102 = 512 + (FX_MulInline(FX_CosIdx(v1), v2 << 12) >> 12);
        param0->unk_104 = 8;
    }

    param0->unk_FC += param0->unk_100;
    param0->unk_FE += param0->unk_102;

    sub_020182E0(&param0->unk_04, param0->unk_FC, (enum RotationAxis)(0));
    sub_020182E0(&param0->unk_04, param0->unk_FE, (enum RotationAxis)(2));
}

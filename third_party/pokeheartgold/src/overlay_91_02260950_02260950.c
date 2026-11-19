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

extern void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

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

extern BOOL ov91_0225E990(const UnkStruct_ov115_022622F8 *param0);

extern BOOL ov91_0225E9AC(const UnkStruct_ov115_022622F8 *param0);

void ov91_02260950(UnkStruct_ov115_022647A0 *param0);

void ov91_02260950(UnkStruct_ov115_022647A0 *param0) {
    BOOL v0;
    fx32 v1;
    fx32 v2;
    fx32 v3;
    int v4;

    v0 = ov91_0225E9AC(param0->unk_F8);

    if (v0 == 1) {
        v2 = ((fx32)(((-300) > 0) ? ((-300) * (1 << 12) + 0.5f) : ((-300) * (1 << 12) - 0.5f)));
    } else {
        v0 = ov91_0225E990(param0->unk_F8);

        if (v0 == 0) {
            v2 = (((fx32)(((-50) > 0) ? ((-50) * (1 << 12) + 0.5f) : ((-50) * (1 << 12) - 0.5f))) - ((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f))));
        } else {
            v2 = ((fx32)(((-300) > 0) ? ((-300) * (1 << 12) + 0.5f) : ((-300) * (1 << 12) - 0.5f)));
        }
    }

    sub_020182A8(&param0->unk_7C, param0->unk_F8->unk_2C.x, v2, param0->unk_F8->unk_2C.z);

    v1 = param0->unk_F8->unk_2C.y - v2;
    v3 = ((fx32)0x0000000000001000L) + (FX_Div(FX_MulInline(v1, ((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f)))), ((fx32)(((300) > 0) ? ((300) * (1 << 12) + 0.5f) : ((300) * (1 << 12) - 0.5f)))));

    sub_020182C4(&param0->unk_7C, v3, v3, v3);

    v4 = (FX_Div(FX_MulInline(v1, ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)))), ((fx32)(((300) > 0) ? ((300) * (1 << 12) + 0.5f) : ((300) * (1 << 12) - 0.5f))))) >> 12;
    v4 = 8 + 16 - v4;

    NNS_G3dGlbPolygonAttr((int)(0), (GXPolygonMode)(0), (GXCull)(0), 0, v4, 0);
}

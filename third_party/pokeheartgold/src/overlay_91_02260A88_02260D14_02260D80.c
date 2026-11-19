#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "math_util.h"
#include "obj_char_transfer.h"
#include "overlay_91.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"

static void ov91_02260EE0(int param0, int param1, int param2, int param3, int param4, s32 *param5, s32 *param6);

struct TextOBJ;

struct G3DPipelineBuffers;

struct MsgData;

struct SpriteResourceCollection;

struct MessageFormat;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_ov114_02025CAD4_t;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov115_02261304;

typedef struct {
    u32 unk_00;
    s32 unk_04;
    s32 unk_08;
    Window unk_0C;
} UnkStruct_ov115_022617E8;

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
    Field3dModel unk_00[5];
    Field3dModel unk_50;
    SpriteResource *unk_60[4];
    SpriteResourcesHeader unk_70;
} UnkStruct_ov115_02264A40;

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

typedef struct {
    BOOL unk_00;
    Sprite *unk_04;
} UnkStruct_ov115_02264BA0;

typedef struct {
    BOOL unk_00;
    Sprite *unk_04;
    VecFx32 unk_08;
    s32 unk_14;
    BOOL unk_18;
} UnkStruct_ov115_02264D80;

typedef struct {
    UnkStruct_ov115_02264D80 unk_00[8];
} UnkStruct_ov115_02264C90;

typedef struct {
    Sprite *unk_00[3];
    u32 unk_0C[3];
    VecFx32 unk_18[3];
    BOOL unk_3C;
} UnkStruct_ov115_022615B0;

typedef struct {
    UnkStruct_ov115_022615B0 unk_00[4];
} UnkStruct_ov115_02261574;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    UnkStruct_ov115_02261304 unk_08;
} UnkStruct_ov115_02261ADC_sub1;

typedef struct {
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[3];
    fx32 unk_1D4;
    u8 unk_1D8;
    u8 unk_1D9;
    s16 unk_1DA;
    fx32 unk_1DC;
    fx32 unk_1E0;
} UnkStruct_ov115_02264FA0;

typedef struct {
    u16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov115_022653CC;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    Window unk_38;
    FontOAM *unk_48;
    UnkStruct_02012B20 *unk_4C;
    UnkStruct_02021AC8 unk_50;
    String *unk_5C;
    SpriteResource *unk_60;
    UnkStruct_ov115_02261304 unk_64;
    UnkStruct_ov115_02261304 unk_7C;
    s16 unk_94;
    s16 unk_96;
    u16 unk_98;
    s16 unk_9A;
    enum HeapID heapID;
} UnkStruct_ov115_02265788;

void ov91_02260D80(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_02264C90 *param1);

void ov91_02260D80(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_02264C90 *param1) {
    int v0;

    for (v0 = 0; v0 < 8; v0++) {
        Sprite_Delete(param1->unk_00[v0].unk_04);
    }
}

void ov91_02260D98(UnkStruct_ov115_02264C90 *param0);

void ov91_02260D98(UnkStruct_ov115_02264C90 *param0) {
    int v0;
    s32 v1;
    s32 v2, v3;
    fx32 v4;
    VecFx32 v5;

    for (v0 = 0; v0 < 8; v0++) {
        if (param0->unk_00[v0].unk_00 == 1) {
            if (param0->unk_00[v0].unk_14 < 16) {
                v1 = param0->unk_00[v0].unk_14;
                ov91_02260EE0(v1, 16, 8, 190, 125, &v2, &v3);
            } else if (param0->unk_00[v0].unk_14 < (16 + 10)) {
                v1 = param0->unk_00[v0].unk_14 - 16;
                ov91_02260EE0(v1, 10, 4, 182, 140, &v2, &v3);
            } else {
                v1 = param0->unk_00[v0].unk_14 - (16 + 10);
                ov91_02260EE0(v1, 6, 3, 180, 160, &v2, &v3);
            }

            param0->unk_00[v0].unk_14++;

            v4 = 9 - ((param0->unk_00[v0].unk_14 * 3) / (16 + 10 + 6));
            v4 = FX_Div(v4 << FX32_SHIFT, FX32_CONST(10));
            v5.x = v4;
            v5.y = v4;

            Sprite_SetAffineScale(param0->unk_00[v0].unk_04, &v5);

            if (param0->unk_00[v0].unk_18) {
                param0->unk_00[v0].unk_08.x += v2 << FX32_SHIFT;
            } else {
                param0->unk_00[v0].unk_08.x -= v2 << FX32_SHIFT;
            }

            param0->unk_00[v0].unk_08.y += v3 << FX32_SHIFT;
            Sprite_SetMatrix(param0->unk_00[v0].unk_04, &param0->unk_00[v0].unk_08);

            if ((param0->unk_00[v0].unk_08.x < (-FX32_CONST(32))) || (param0->unk_00[v0].unk_08.x > (FX32_CONST(288))) || (param0->unk_00[v0].unk_08.y < (-FX32_CONST(32)) + (512 << FX32_SHIFT)) || (param0->unk_00[v0].unk_08.y > (FX32_CONST(224)) + (512 << FX32_SHIFT))) {
                param0->unk_00[v0].unk_00 = 0;
                Sprite_SetDrawFlag(param0->unk_00[v0].unk_04, FALSE);
            }
        }
    }
}

void ov91_02260E88(UnkStruct_ov115_02264C90 *param0, s32 param1, s32 param2);

void ov91_02260E88(UnkStruct_ov115_02264C90 *param0, s32 param1, s32 param2) {
    int v0;
    UnkStruct_ov115_02264D80 *v1 = NULL;

    for (v0 = 0; v0 < 8; v0++) {
        if (param0->unk_00[v0].unk_00 == 0) {
            v1 = &param0->unk_00[v0];
            break;
        }
    }

    if (v1 == NULL) {
        return;
    }

    if (param1 < 128) {
        v1->unk_18 = 1;
    } else {
        v1->unk_18 = 0;
    }

    v1->unk_08.x = param1 << FX32_SHIFT;
    v1->unk_08.y = (param2 << FX32_SHIFT) + (512 << FX32_SHIFT);

    Sprite_SetMatrix(v1->unk_04, &v1->unk_08);

    v1->unk_00 = 1;
    Sprite_SetDrawFlag(v1->unk_04, TRUE);
    v1->unk_14 = 0;
}

static void ov91_02260EE0(int param0, int param1, int param2, int param3, int param4, s32 *param5, s32 *param6) {
    s16 v0;
    s32 v1 = param4 - param3;
    v0 = param3 + ((param0 * v1) / param1);

    *param5 = FX_Mul(GF_CosDegNoWrap(v0), param2 << FX32_SHIFT) >> FX32_SHIFT;
    *param6 = FX_Mul(GF_SinDegNoWrap(v0), param2 << FX32_SHIFT) >> FX32_SHIFT;
}

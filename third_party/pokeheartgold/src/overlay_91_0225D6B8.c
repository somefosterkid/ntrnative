#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"

struct TextOBJ;
struct G3DPipelineBuffers;
struct MsgData;
struct Sprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct UnkStruct_ov114_02025CAD4_t;
typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;
Sprite *Sprite_CreateAffine(const SpriteTemplate *template);
void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);
void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);
void Sprite_SetAnimSpeed(Sprite *sprite, fx32 speed);
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
void ov91_0225D4D4(u32 param0, u32 param1, u32 param2, s32 *param3, s32 *param4);
void Sprite_Delete(Sprite *gfxElem);
void Sprite_SetDrawPriority(Sprite *sprite, u32 priority);
static void ov91_0225D884(UnkStruct_ov115_022615B0 *param0, u32 param1);
static void ov91_0225D8E0(UnkStruct_ov115_022615B0 *param0, u32 param1);

void ov91_0225D6B8(UnkStruct_ov115_022615B0 *param0, SpriteTemplate *param1, UnkStruct_ov115_02261ADC *param2, u32 param3, u32 param4, u32 param5, u32 param6);
void ov91_0225D750(UnkStruct_ov115_022615B0 *param0);
void ov91_0225D768(UnkStruct_ov115_022615B0 *param0, u32 param1, u32 param2, int param3);
void ov91_0225D84C(UnkStruct_ov115_022615B0 *param0);
static void ov91_0225D884(UnkStruct_ov115_022615B0 *param0, u32 param1);
static void ov91_0225D8E0(UnkStruct_ov115_022615B0 *param0, u32 param1);

void ov91_0225D6B8(UnkStruct_ov115_022615B0 *param0, SpriteTemplate *param1, UnkStruct_ov115_02261ADC *param2, u32 param3, u32 param4, u32 param5, u32 param6) {
    int v0;
    s32 v1, v2;

    for (v0 = 0; v0 < 3; v0++) {
        ov91_0225D4D4(param3, param5, param4, &v1, &v2);
        param1->position.x = v1 << 12;
        param1->position.y = v2 << 12;
        param0->unk_18[v0] = param1->position;
        param0->unk_00[v0] = Sprite_CreateAffine(param1);

        Sprite_SetAnimCtrlSeq(param0->unk_00[v0], 2);
        Sprite_SetDrawFlag(param0->unk_00[v0], 0);
        Sprite_SetAnimActiveFlag(param0->unk_00[v0], 1);
        Sprite_SetAnimSpeed(param0->unk_00[v0], ((fx32)(((1.5) > 0) ? ((1.5) * (1 << 12) + 0.5f) : ((1.5) * (1 << 12) - 0.5f))));

        param0->unk_0C[v0] = 0;
    }

    if (param5 == param4) {
        param0->unk_3C = 1;
    } else {
        param0->unk_3C = 0;
    }
}

void ov91_0225D750(UnkStruct_ov115_022615B0 *param0) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        Sprite_Delete(param0->unk_00[v0]);
    }
}

void ov91_0225D768(UnkStruct_ov115_022615B0 *param0, u32 param1, u32 param2, int param3) {
    int v0;
    int v1;
    BOOL v2;
    u32 v3;
    u32 v4;
    u32 v5;
    u32 v6;

    v2 = 0;
    v4 = 0;
    v1 = -1;

    for (v0 = 0; v0 < 3; v0++) {
        if (Sprite_GetDrawFlag(param0->unk_00[v0]) == 0) {
            v1 = v0;
            v2 = 1;
        } else {
            v3 = Sprite_GetDrawPriority(param0->unk_00[v0]);
            v3++;
            Sprite_SetDrawPriority(param0->unk_00[v0], v3);

            if ((v2 == 0) && (v4 <= v3)) {
                v4 = v3;
                v1 = v0;
            }
        }
    }

    ((v1 >= 0) ? (void)0 : GF_AssertFail());

    if (param2 >= 3) {
        param2 = 3 - 1;
    }

    if (param3 != 2) {
        if (param0->unk_3C == 1) {
            v5 = param2 + 2;
        } else {
            v5 = param2 + 8;
        }

        v6 = 3 + param1;
    } else {
        if (param0->unk_3C == 1) {
            v5 = param2 + 5;
        } else {
            v5 = param2 + 11;
        }

        v6 = 7;
    }

    Sprite_SetAnimCtrlSeq(param0->unk_00[v1], v5);
    param0->unk_0C[v1] = 0;
    Sprite_SetDrawFlag(param0->unk_00[v1], 1);
    ov91_0225D884(param0, v1);
    Sprite_SetDrawPriority(param0->unk_00[v1], 0);
    Sprite_SetPaletteOverride(param0->unk_00[v1], v6);
}

void ov91_0225D84C(UnkStruct_ov115_022615B0 *param0) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        if (Sprite_GetDrawFlag(param0->unk_00[v0]) == 1) {
            param0->unk_0C[v0]++;

            if (param0->unk_0C[v0] > 16) {
                ov91_0225D8E0(param0, v0);
            } else {
                ov91_0225D884(param0, v0);
            }
        }
    }
}

static void ov91_0225D884(UnkStruct_ov115_022615B0 *param0, u32 param1) {
    fx32 v0;
    VecFx32 v1;

    v0 = FX_Div(FX_MulInline(((fx32)0x0000000000001000L) * param0->unk_0C[param1], -((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)))), ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))));
    v1 = param0->unk_18[param1];
    v1.y += v0;

    Sprite_SetMatrix(param0->unk_00[param1], &v1);
}

static void ov91_0225D8E0(UnkStruct_ov115_022615B0 *param0, u32 param1) {
    Sprite_SetDrawFlag(param0->unk_00[param1], 0);
}

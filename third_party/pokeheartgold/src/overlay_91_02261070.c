#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_91.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"

struct TextOBJ;

struct G3DPipelineBuffers;

struct MsgData;

struct SpriteList;

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

void sub_02018068(Field3dModel *model);

void sub_020180F8(Field3DModelAnimation *anim, NNSFndAllocator *allocator);

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

void ov91_02261070(UnkStruct_ov115_02264FA0 *param0, UnkStruct_ov115_02261ADC *param1, NNSFndAllocator *param2);

s32 sub_020182A0(void *, s32);

void ov91_02261070(UnkStruct_ov115_02264FA0 *param0, UnkStruct_ov115_02261ADC *param1, NNSFndAllocator *param2) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        sub_020180F8(&param0->unk_198[v0], param2);
    }

    for (v0 = 0; v0 < 3; v0++) {
        sub_02018068(&param0->unk_168[v0]);
    }
}

void sub_02018124(Field3DModelAnimation *anim, fx32 frameDelta);

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void sub_020181EC(Field3dObject *obj);

void ov91_022610A8(UnkStruct_ov115_02264FA0 *param0);

void ov91_022610A8(UnkStruct_ov115_02264FA0 *param0) {
    int v0;

    sub_02018124(&param0->unk_198[0], param0->unk_1D4);

    if (param0->unk_1D8) {
        switch (param0->unk_1D9) {
        case 0:

            if ((param0->unk_1DC + param0->unk_1E0) < (FX32_CONST(51))) {
                param0->unk_1DC += param0->unk_1E0;
            } else {
                param0->unk_1DC = 0;
            }

            if ((param0->unk_1E0 + (FX32_CONST(0.10))) <= (FX32_CONST(2.30))) {
                param0->unk_1E0 += (FX32_CONST(0.10));
            } else {
                param0->unk_1E0 = (FX32_CONST(2.30));
                param0->unk_1DA = 24;
                param0->unk_1D9++;
            }
            break;
        case 1:
            if ((param0->unk_1DC + param0->unk_1E0) < (FX32_CONST(51))) {
                param0->unk_1DC += param0->unk_1E0;
            } else {
                param0->unk_1DC = 0;
            }

            param0->unk_1DA--;

            if (param0->unk_1DA <= 0) {
                param0->unk_1D9++;
                param0->unk_1DC = (FX32_CONST(51));
            }
            break;
        case 2:
            if ((param0->unk_1DC + (FX32_CONST(1.0))) < (FX32_CONST(100))) {
                param0->unk_1DC += (FX32_CONST(1.0));
            } else {
                param0->unk_1DC = (FX32_CONST(51));
            }
            break;
        default:
            GF_ASSERT(FALSE);
            break;
        }

        sub_02018198(&param0->unk_198[1], param0->unk_1DC);
        sub_02018198(&param0->unk_198[2], param0->unk_1DC);
    }

    for (v0 = 0; v0 < 3; v0++) {
        sub_020181EC(&param0->unk_00[v0]);
    }
}

void ov91_022611D0(void *arg0) {
    sub_020182A0(arg0, 0);
    sub_020182A0(arg0 + 0x78, 1);
    *((s32 *)&((UnkStruct_ov115_02264FA0 *)arg0)->unk_1DC) = 0;
    *((s32 *)&((UnkStruct_ov115_02264FA0 *)arg0)->unk_1E0) = 0x1000;
    *((s8 *)&((UnkStruct_ov115_02264FA0 *)arg0)->unk_1D8) = 1;
    *((s8 *)&((UnkStruct_ov115_02264FA0 *)arg0)->unk_1D9) = 0;
}

void ov91_02261200(void *arg0, s32 arg1) {
    *((s32 *)&((UnkStruct_ov115_02264FA0 *)arg0)->unk_1D4) = arg1;
}

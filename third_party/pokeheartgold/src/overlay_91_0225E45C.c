#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "overlay_91.h"
#include "player_data.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "touch_pad_data_buffer.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"

struct TextOBJ;

struct G3DPipelineBuffers;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov115_02260D78_t;

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 padding_02[2];
} UnkStruct_ov115_022612BC;

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;

typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;

typedef struct {
    u32 unk_00[4];
} UnkStruct_ov115_02265AD4;

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
    u32 unk_00;
    NNSG2dSVec2 unk_04[8];
    u16 unk_24;
    u16 unk_26;
    u32 unk_28;
} UnkStruct_ov115_02262CA4;

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
    UnkStruct_ov115_02262DC4 unk_00;
    UnkStruct_ov115_02262DC4 unk_18;
    UnkStruct_ov115_02262DC4 unk_30;
    s32 unk_48;
} UnkStruct_ov115_02262E64;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    u8 unk_38;
    u8 unk_39;
    u8 unk_3A;
    u8 unk_3B;
    s32 unk_3C;
} UnkStruct_ov115_02263130;

typedef struct {
    u32 unk_00[4];
    u32 unk_10;
    u32 unk_14;
} UnkStruct_ov115_022632C8;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    Window unk_38;
    TextOBJ *unk_48;
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

typedef struct {
    u16 unk_00;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov115_022658E0;

typedef struct {
    BgConfig *unk_00;
    MessageFormat *unk_04;
    MsgData *unk_08;
    String *unk_0C;
    String *unk_10;
    u16 unk_14;
    u16 unk_16;
    G3DPipelineBuffers *unk_18;
    SpriteList *unk_1C;
    G2dRenderer unk_20;
    SpriteResourceCollection *unk_1AC[4];
    SpriteResourcesHeader unk_1BC;
    SpriteResource *unk_1E0[4];
    UnkStruct_02012744 *unk_1F0;
    Camera *camera;
    VecFx32 unk_1F8;
    UnkStruct_ov115_02264FA0 unk_204;
    UnkStruct_ov115_02261574 unk_3E8;
    UnkStruct_ov115_022617E8 unk_4E8;
    UnkStruct_ov115_02261ADC_sub1 unk_504;
    UnkStruct_ov115_02263DF8 unk_524;
    UnkStruct_ov114_0225CAD4 *unk_754;
    UnkStruct_ov115_022653CC unk_758;
    UnkStruct_ov115_02264A40 unk_760;
    UnkStruct_ov115_022647A0 unk_7F4[96];
    UnkStruct_ov115_02264BA0 unk_6A34;
    UnkStruct_ov115_02264C90 unk_6A3C;
    UnkStruct_ov115_02265788 unk_6B1C;
    NNSFndAllocator unk_6BBC;
} UnkStruct_ov115_02261ADC;

struct UnkStruct_ov115_02260D78_t {
    u32 unk_00;
    BOOL unk_04;
    s32 unk_08;
    s32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    UnkStruct_ov115_02262CA4 unk_20;
    UnkStruct_ov115_022622F8 unk_4C[96];
    UnkStruct_ov115_02261C18 unk_19CC;
    UnkStruct_ov115_02261ADC unk_1AB4;
    UnkStruct_ov115_02265AD4 unk_8680;
    TouchPadDataBuffer unk_8690;
    UnkStruct_ov115_02262E64 unk_86D4;
    const UnkStruct_ov115_02262F50 *unk_8720;
    u16 unk_8724;
    u16 unk_8726;
    UnkStruct_ov115_022612BC unk_8728;
    BOOL unk_872C;
    UnkStruct_ov115_02263130 unk_8730;
    UnkStruct_ov115_022632C8 unk_8770;
    UnkStruct_ov115_022658E0 unk_8788;
};

u32 ov91_0225D600(UnkStruct_ov115_02260D78 *param0);

void ov91_0225D694(UnkStruct_ov115_02261574 *param0, u32 param1, u32 param2, int param3);

u32 ov91_0225DE8C(const UnkStruct_ov115_02261C18 *param0, const VecFx32 *param1, const VecFx32 *param2, VecFx32 *param3);

BOOL ov91_0225DFB0(const UnkStruct_ov115_02261C18 *param0, const VecFx32 *param1);

void ov91_0225E3F4(UnkStruct_ov115_02260D78 *param0, UnkStruct_ov115_022622F8 *param1, u16 param2, u32 param3);

void ov91_0225E728(const UnkStruct_ov115_022622F8 *param0, VecFx32 *param1);

void ov91_0225E7C4(UnkStruct_ov115_022622F8 *param0, UnkStruct_ov115_02260D78 *param1, BOOL param2);

void ov91_0225E7E8(UnkStruct_ov115_022622F8 *param0, UnkStruct_ov115_02260D78 *param1, BOOL param2, fx32 param3, BOOL param4);

BOOL ov91_0225E990(const UnkStruct_ov115_022622F8 *param0);

BOOL ov91_0225E9C0(UnkStruct_ov115_022622F8 *param0);

void ov91_0225E9E4(UnkStruct_ov115_022622F8 *param0, UnkStruct_ov115_02260D78 *param1);

void ov91_0225EA54(UnkStruct_ov115_022622F8 *param0, const VecFx32 *param1);

void ov91_02261808(UnkStruct_ov115_02265788 *param0, UnkStruct_ov115_02261ADC *param1, u32 param2);

void ov91_02261890(UnkStruct_ov115_02265788 *param0);

void ov91_022618B0(UnkStruct_ov115_02265788 *param0, UnkStruct_ov115_02261ADC *param1, u32 param2);

void ov91_0225E45C(UnkStruct_ov115_02260D78 *param0, UnkStruct_ov115_022622F8 *param1);

s32 ov91_0225E67C(s32, s32);

void ov91_0225E45C(UnkStruct_ov115_02260D78 *param0, UnkStruct_ov115_022622F8 *param1) {
    VecFx32 v0;
    u32 v1;
    VecFx32 v2;
    BOOL v3;
    fx32 v4;
    u32 v5;

    if ((param1->unk_00 != 1) && (param1->unk_00 != 2)) {
        return;
    }

    v4 = param1->unk_2C.y;
    v5 = param1->unk_00;

    ov91_0225E728(param1, &v0);

    if (param1->unk_00 == 1) {
        v1 = ov91_0225DE8C(&param0->unk_19CC, &param1->unk_2C, &v0, &v2);
    } else {
        v1 = 0;
    }

    ov91_0225EA54(param1, &v0);

    if ((v1 == 3) || (v1 == 1)) {
        ov91_0225EA54(param1, &v2);
    }

    param1->unk_02++;

    if (v1 == 1) {
        param0->unk_18 += ov91_0225D600(param0);

        if (param0->unk_18 > 99999) {
            param0->unk_18 = 99999;
        }

        switch (param0->unk_1C) {
        case 0:
            PlaySE(1410);
            PlaySE(1411);
            break;
        case 1:
            PlaySE(1410);
            PlaySE(1412);
            break;
        default:
            PlaySE(1410);
            PlaySE(1413);
            break;
        }

        ov91_0225E9E4(param1, param0);
        ov91_0225D694(&param0->unk_1AB4.unk_3E8, param0->unk_14, param0->unk_1C, param0->unk_00);

        if (param0->unk_1C < 99) {
            param0->unk_1C++;
        }

        if (param0->unk_1C == 3) {
            ov91_02261808(&param0->unk_1AB4.unk_6B1C, &param0->unk_1AB4, param0->unk_1C);
        } else {
            ov91_022618B0(&param0->unk_1AB4.unk_6B1C, &param0->unk_1AB4, param0->unk_1C);
        }
    } else {
        v3 = 0;
        v1 = ov91_0225DFB0(&param0->unk_19CC, &param1->unk_2C);

        if (v1 == 1) {
            v3 = 1;

            if (v5 == 1) {
                PlaySE(1826);
            }

            ov91_0225E7C4(param1, param0, 1);
        } else if (param1->unk_2C.y <= ((fx32)(((-50) > 0) ? ((-50) * (1 << 12) + 0.5f) : ((-50) * (1 << 12) - 0.5f)))) {
            v1 = ov91_0225E990(param1);

            if (v1 == 0) {
                v3 = 1;

                if (v4 <= ((fx32)(((-50) > 0) ? ((-50) * (1 << 12) + 0.5f) : ((-50) * (1 << 12) - 0.5f)))) {
                    ov91_0225E3F4(param0, param1, 0, param0->unk_1C);
                } else {
                    ov91_0225E7C4(param1, param0, 0);
                }
            } else if (param1->unk_2C.y <= ((fx32)(((0.250f) > 0) ? ((0.250f) * (1 << 12) + 0.5f) : ((0.250f) * (1 << 12) - 0.5f)))) {
                v3 = 1;
                ov91_0225E3F4(param0, param1, 0, param0->unk_1C);
            }
        } else {
            v1 = ov91_0225E9C0(param1);

            if (v1 == 1) {
                v3 = 1;

                VEC_Subtract(&param1->unk_2C, &v0, &param1->unk_2C);
                ov91_0225E7E8(param1, param0, 1, ((fx32)(((0.250f) > 0) ? ((0.250f) * (1 << 12) + 0.5f) : ((0.250f) * (1 << 12) - 0.5f))), 0);
            }
        }

        if ((v3 == 1) && (v5 == 1)) {
            param0->unk_1C = 0;
            ov91_02261890(&param0->unk_1AB4.unk_6B1C);
        }
    }
}

void ov91_0225E62C(s32 arg0) {
    s32 v0;
    s32 v1;

    v0 = 0;
    v1 = arg0 + 0x4C;
    do {
        ov91_0225E67C(v1, arg0);
        v0 += 1;
        v1 += 0x44;
    } while (v0 < 0x60);
}

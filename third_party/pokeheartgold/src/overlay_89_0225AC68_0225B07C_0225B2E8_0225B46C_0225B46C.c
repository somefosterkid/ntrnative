#include "global.h"
#include "camera.h"
#include "field/overlay_01_021FB878.h"

struct CameraAngle;

struct Field3dModel;

struct Field3dObject;

struct UnkStruct_ov113_0225E6B8_t;

typedef struct UnkStruct_ov113_0225E6B8_t UnkStruct_ov113_0225E6B8;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_0225FE30;

typedef struct {
    VecFx32 unk_00;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FEE0;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FF8C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260064;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0226013C;

typedef struct {
    CameraAngle cameraAngle;
    fx32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E[2];
} UnkStruct_ov113_02260218;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_022602E4;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_02260394;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_0226046C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260544;

typedef struct {
    UnkStruct_ov113_0225FE30 unk_00;
    UnkStruct_ov113_0225FEE0 unk_08;
    UnkStruct_ov113_0225FF8C unk_18;
    UnkStruct_ov113_02260064 unk_28;
    UnkStruct_ov113_0226013C unk_38;
    UnkStruct_ov113_02260218 unk_48;
    UnkStruct_ov113_022602E4 unk_58;
    UnkStruct_ov113_02260394 unk_60;
    UnkStruct_ov113_0226046C unk_80;
    UnkStruct_ov113_02260544 unk_A0;
} UnkStruct_ov113_0225EB20_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_ov113_0225EDCC;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
} UnkStruct_ov113_0225FD80;

typedef struct {
    UnkStruct_ov113_0225EB20_sub1 unk_00;
    UnkStruct_ov113_0225FD80 unk_B0;
    UnkStruct_ov113_0225E6B8 *unk_B4[128];
    UnkStruct_ov113_0225E6B8 *unk_2B4[128];
    NNSG3dResTex *unk_4B4[32];
    u32 unk_534[2];
    UnkStruct_ov113_0225EDCC unk_53C[128];
    u8 unk_73C;
    u8 unk_73D;
    u8 unk_73E;
    u8 unk_73F;
    u8 unk_740;
    s16 unk_742;
} UnkStruct_ov113_0225EB20;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov113_02260818;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void Field3dObject_SetPosEx(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182B0(const Field3dObject *obj, fx32 *outX, fx32 *outY, fx32 *outZ);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_020182E0(Field3dObject *obj, s16 angle, enum RotationAxis axis);
#else
void sub_020182E0(Field3dObject *obj, u16 angle, enum RotationAxis axis);
#endif

u16 sub_020182EC(const Field3dObject *obj, enum RotationAxis axis);

fx32 GF_SinDegFX32(fx32 degrees);

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
} UnkStruct_ov113_0225EDA4;

typedef struct {
    s32 unk_00;
    UnkStruct_ov113_0225EDA4 unk_04;
} UnkStruct_ov113_0225EF0C;

typedef struct {
    int unk_00;
    fx32 unk_04;
    u8 unk_08;
    u8 unk_09;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225EF78;

typedef struct {
    Field3dObject unk_00[3];
    u16 unk_168;
    u8 unk_16A;
    UnkStruct_ov113_0225EDA4 unk_16C[4];
} UnkStruct_ov113_0225F060;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225F1E4;

typedef struct {
    int unk_00;
    fx32 unk_04;
    int unk_08;
    fx32 unk_0C;
    UnkStruct_ov113_0225EDA4 unk_10;
} UnkStruct_ov113_0225F368;

typedef struct {
    Field3dObject unk_00[3];
    u16 unk_168;
    u8 unk_16A;
    u8 unk_16B;
    u8 unk_16C;
    u8 unk_16D;
    UnkStruct_ov113_0225EDA4 unk_170;
} UnkStruct_ov113_0225F51C;

typedef struct {
    int unk_00;
    fx32 unk_04;
    fx32 unk_08;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225F728;

typedef struct {
    fx32 unk_00;
    u16 unk_04;
    u8 unk_06;
    UnkStruct_ov113_0225EDA4 unk_08;
} UnkStruct_ov113_0225F818;

typedef struct {
    Field3dObject unk_00[2];
    fx32 unk_F0;
    u16 unk_F4;
    u8 unk_F6;
    u8 unk_F7;
    UnkStruct_ov113_0225EDA4 unk_F8[3];
} UnkStruct_ov113_0225F890;

typedef struct {
    Field3dObject unk_00[2];
    fx32 unk_F0;
    u16 unk_F4;
    u8 unk_F6;
    u8 unk_F7;
    UnkStruct_ov113_0225EDA4 unk_F8[3];
} UnkStruct_ov113_0225FAB0;

typedef struct {
    fx32 unk_00;
    u16 unk_04;
    u8 unk_06;
    UnkStruct_ov113_0225EDA4 unk_08;
} UnkStruct_ov113_0225FCD0;

typedef union {
    UnkStruct_ov113_0225EF0C val1;
    UnkStruct_ov113_0225EF78 val2;
    UnkStruct_ov113_0225F060 val3;
    UnkStruct_ov113_0225F1E4 val4;
    UnkStruct_ov113_0225F368 val5;
    UnkStruct_ov113_0225F51C val6;
    UnkStruct_ov113_0225F728 val7;
    UnkStruct_ov113_0225F818 val8;
    UnkStruct_ov113_0225F890 val9;
    UnkStruct_ov113_0225FAB0 val10;
    UnkStruct_ov113_0225FCD0 val11;
} UnkStruct_ov113_0225E6B8_sub1;

struct UnkStruct_ov113_0225E6B8_t {
    UnkStruct_ov113_02260818 unk_00;
    Field3dModel unk_0C;
    Field3dObject unk_1C;
    UnkStruct_ov113_0225E6B8_sub1 unk_94;
    u32 unk_240_0 : 4;
    u32 unk_240_4 : 4;
    u32 unk_240_8 : 8;
    u32 unk_240_16 : 8;
    u32 unk_240_24 : 8;
};

void ov113_0225EA60(NNSG3dResTex *param0, int param1);

BOOL ov89_0225A958(UnkStruct_ov113_0225EB20 *param0, NNSG3dResTex *param1);

void ov113_0225ECA0(Field3dObject *param0, const UnkStruct_ov113_02260818 *param1, UnkStruct_ov113_0225EDA4 *param2, int param3);

extern const struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
} Unk_ov113_02260BBC;

int ov89_0225B46C(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225E6B8 *param1);

void ov89_0225AB64(NNSG3dResTex *param0, int param1);

void sub_020182A8(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov89_0225ADA4(Field3dObject *param0, const UnkStruct_ov113_02260818 *param1, UnkStruct_ov113_0225EDA4 *param2, int param3);

int ov89_0225B46C(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225E6B8 *param1) {
    UnkStruct_ov113_0225F368 *v0 = &param1->unk_94.val5;
    u16 v1;
    fx32 v2, v3, v4;
    fx32 v5;

    switch (v0->unk_00) {
    case 0:
        v1 = sub_020182EC(&param1->unk_1C, (enum RotationAxis)(2));
        v1 += 65536 / 4 / 6;

        if (v1 >= 65536 / 4 / 2) {
            ov89_0225AB64(param1->unk_0C.texture, param1->unk_240_8);
            v1 -= 65536 / 4;
            ov89_0225A958(param0, param1->unk_0C.texture);
            param1->unk_240_4 = 1;
            v0->unk_00++;
        }

        sub_020182E0(&param1->unk_1C, v1, (enum RotationAxis)(2));
        break;
    case 1:
        param1->unk_240_4 = 0;

        v1 = sub_020182EC(&param1->unk_1C, (enum RotationAxis)(2));
        v1 += 65536 / 4 / 6;

        if ((v1 >= 0) && (v1 < 65536 / 4 * 1)) {
            v1 = 0;
            v0->unk_00++;
        }

        sub_020182E0(&param1->unk_1C, v1, (enum RotationAxis)(2));
        break;
    case 2:
        v0->unk_08++;

        if (v0->unk_08 < 4) {
            break;
        }

        sub_020182B0(&param1->unk_1C, &v2, &v3, &v4);

        v0->unk_0C = v3;
        v0->unk_08 = 0;
        v0->unk_00++;
    case 3:
        sub_020182B0(&param1->unk_1C, &v2, &v3, &v4);

        if ((v2 < -0x50000) || (v2 > 0x50000) || (v3 > 0x30000) || (v3 < -0x30000)) {
            return 1;
        }

        v0->unk_04 += 0x8000;

        if (v0->unk_04 >= (360 << 12)) {
            v0->unk_04 -= 360 << 12;
        }

        v5 = FX_MulInline(GF_SinDegFX32(v0->unk_04), 0x8000);
        sub_020182A8(&param1->unk_1C, v2 - 0x1000, v0->unk_0C + v5, v4);
        break;
    }

    ov89_0225ADA4(&param1->unk_1C, &param1->unk_00, &v0->unk_10, 0);
    return 0;
}

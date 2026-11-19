#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "unk_02005D10.h"

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

BOOL ov89_0225BE84(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225FD80 *param1, int param2, int param3);

BOOL ov89_0225BF34(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);
BOOL ov89_0225BFE4(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);
BOOL ov89_0225C090(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);
BOOL ov89_0225C168(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);
BOOL ov89_0225C240(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);

BOOL ov89_0225BE84(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225FD80 *param1, int param2, int param3) {
    int v0;

    if (param2 == 3) {
        return 0;
    }

    if (param3 == 0) {
        v0 = -1;
    } else {
        v0 = 1;
    }

    switch (param1->unk_00) {
    case 0:
        param1->unk_02 += 0x200;

        if (param1->unk_02 >= 0x800) {
            param1->unk_02 = 0x800;
            param1->unk_00++;
        }

        param0->unk_740 = 1;
        param0->unk_742 = (param1->unk_02 >> 8) * v0;
        break;
    case 1:
        param1->unk_02 -= 0x200;

        if (param1->unk_02 <= 0) {
            param1->unk_02 = 0;
            param1->unk_01++;

            if (param1->unk_01 < 2) {
                param1->unk_00 = 0;
            } else {
                param1->unk_00++;
            }
        }

        param0->unk_740 = 1;
        param0->unk_742 = (param1->unk_02 >> 8) * v0;
        break;
    default:
        param0->unk_73E = 1;
        return 1;
    }

    return 0;
}

BOOL ov89_0225BF34(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_0225FE30 *v0 = &param1->unk_00;

    switch (v0->unk_06) {
    case 0:
        v0->unk_00 = Camera_GetDistance(camera);
        v0->unk_06++;
        PlaySE(1493);
    case 1:
        Camera_AdjustDistance(((-((fx32)0x0000000000001000L) * 50) / 30), camera);
        v0->unk_04++;

        if (v0->unk_04 >= 30) {
            v0->unk_04 = 0;
            v0->unk_06++;
        }
        break;
    case 2:
        v0->unk_04++;

        if (v0->unk_04 > 4) {
            v0->unk_04 = 0;
            v0->unk_06++;
        }
        break;
    case 3:
        Camera_AdjustDistance((-(-((fx32)0x0000000000001000L) * 50) / 6), camera);
        v0->unk_04++;

        if ((v0->unk_04 >= 6) || (Camera_GetDistance(camera) >= v0->unk_00)) {
            Camera_SetDistance(v0->unk_00, camera);
            return 1;
        }

        break;
    }

    return 0;
}

BOOL ov89_0225BFE4(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_0225FEE0 *v0 = &param1->unk_08;
    fx32 v1;

    switch (v0->unk_0C) {
    case 0:
        PlaySE(1493);
        v0->unk_0C++;
    case 1:
        v0->unk_0D++;
        v1 = ((fx32)0x0000000000001000L) * 4 / 16 * v0->unk_0D;

        if (v1 < ((fx32)0x0000000000001000L) * 2) {
            v0->unk_00.y = ((fx32)0x0000000000001000L) - v1;
        } else {
            v0->unk_00.y = -((fx32)0x0000000000001000L) + (v1 - ((fx32)0x0000000000001000L) * 2);
        }

        if (v1 < ((fx32)0x0000000000001000L) * 1) {
            v0->unk_00.x = v1;
        } else if (v1 < ((fx32)0x0000000000001000L) * 2) {
            v0->unk_00.x = ((fx32)0x0000000000001000L) - (v1 - ((fx32)0x0000000000001000L));
        } else if (v1 < ((fx32)0x0000000000001000L) * 3) {
            v0->unk_00.x = -(v1 - ((fx32)0x0000000000001000L) * 2);
        } else {
            v0->unk_00.x = -((fx32)0x0000000000001000L) + (v1 - ((fx32)0x0000000000001000L) * 3);
        }

        if (v0->unk_0D >= 16) {
            v0->unk_00.y = ((fx32)0x0000000000001000L);
            v0->unk_00.x = 0;
            v0->unk_0D = 0;
            v0->unk_0E++;

            if (v0->unk_0E >= 2) {
                v0->unk_0C++;
            }
        }

        Camera_SetLookAtCamUp(&v0->unk_00, camera);
        break;
    default:
        return 1;
    }

    return 0;
}

BOOL ov89_0225C090(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_0225FF8C *v0 = &param1->unk_18;
    CameraAngle v1 = { 0, 0, 0, 0 };

    switch (v0->unk_0E) {
    case 0:
        v0->cameraAngle = Camera_GetAngle(camera);
        v0->unk_08 = v0->cameraAngle.y;
        v0->unk_0E++;
        PlaySE(1493);
    case 1:
        v1.y += (-0x2000 / 2);
        v0->unk_08 += (-0x2000 / 2);
        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 <= v0->cameraAngle.y - 0x2000) {
            v0->unk_0E++;
        }
        break;
    case 2:
        v0->unk_0C++;

        if (v0->unk_0C >= 16) {
            v0->unk_0E++;
        }
        break;
    case 3:
        v1.y += (0x2000 / 16);
        v0->unk_08 += (0x2000 / 16);

        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 >= v0->cameraAngle.y) {
            Camera_SetAnglePos(&v0->cameraAngle, camera);
            return 1;
        }
        break;
    }

    return 0;
}

BOOL ov89_0225C168(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_02260064 *v0 = &param1->unk_28;
    CameraAngle v1 = { 0, 0, 0, 0 };

    switch (v0->unk_0E) {
    case 0:
        v0->cameraAngle = Camera_GetAngle(camera);
        v0->unk_08 = v0->cameraAngle.y;
        v0->unk_0E++;
        PlaySE(1493);
    case 1:
        v1.y += (0x2000 / 2);
        v0->unk_08 += (0x2000 / 2);
        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 >= v0->cameraAngle.y + 0x2000) {
            v0->unk_0E++;
        }
        break;
    case 2:
        v0->unk_0C++;

        if (v0->unk_0C >= 16) {
            v0->unk_0E++;
        }
        break;
    case 3:
        v1.y += (-0x2000 / 16);
        v0->unk_08 += (-0x2000 / 16);

        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 <= v0->cameraAngle.y) {
            Camera_SetAnglePos(&v0->cameraAngle, camera);
            return 1;
        }
        break;
    }

    return 0;
}

BOOL ov89_0225C240(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_0226013C *v0 = &param1->unk_38;
    CameraAngle v1 = { 0, 0, 0, 0 };

    switch (v0->unk_0E) {
    case 0:
        v0->cameraAngle = Camera_GetAngle(camera);
        v0->unk_08 = v0->cameraAngle.x;
        PlaySE(1493);
        v0->unk_0E++;
    case 1:
        v1.x += (0x2000 / 3);
        v0->unk_08 += (0x2000 / 3);

        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 >= v0->cameraAngle.x + 0x2000) {
            v0->unk_0E++;
        }

        break;
    case 2:
        v0->unk_0C++;

        if (v0->unk_0C >= 16) {
            v0->unk_0E++;
        }
        break;
    case 3:
        v1.x += (-0x2000 / 16);
        v0->unk_08 += (-0x2000 / 16);

        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 <= v0->cameraAngle.x) {
            Camera_SetAnglePos(&v0->cameraAngle, camera);
            return 1;
        }

        break;
    }

    return 0;
}

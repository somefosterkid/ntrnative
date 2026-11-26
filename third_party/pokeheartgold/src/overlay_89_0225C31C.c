#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "math_util.h"
#include "unk_02005D10.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_89.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

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

BOOL ov89_0225C31C(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);

s32 ov45_0222A578(u32, s32);
void ov45_0222A844(s32, PlayerProfile *, u32);
s32 ov45_0222AB28(u32, s32);
s32 ov45_0222EC68(u32);
u32 ov45_0222EC7C(u32);
void ov89_0225C724(MsgData *param_1, MessageFormat *param_2, u8 *param_3, u32 param_4, u32 param_5);

extern void ov89_0225C818(Window param0[], u32 param1);

extern __attribute__((aligned(4))) const u16 ov89_0225CE70[9][2];

BOOL ov89_0225C31C(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_02260218 *v0 = &param1->unk_48;
    fx32 v1;
    CameraAngle v2 = { 0, 0, 0, 0 };

    switch (v0->unk_0D) {
    case 0:
        v0->cameraAngle = Camera_GetAngle(camera);
        v0->unk_0D++;
        PlaySE(1493);
    case 1:
        v0->unk_08 += 0x18000;

        if (v0->unk_08 >= (360 << 12)) {
            v0->unk_08 -= 360 << 12;
            v0->unk_0C++;

            if (v0->unk_0C >= 2) {
                v0->unk_0D++;
                v0->unk_08 = 0;
            }
        }

        v1 = FX_MulInline(GF_SinDegFX32(v0->unk_08), 0x1000);
        v2 = v0->cameraAngle;
        v2.y = v1;
        Camera_SetAngleTarget(&v2, camera);
        break;
    default:
        return 1;
    }

    return 0;
}

BOOL ov89_0225C3E8(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);

BOOL ov89_0225C3E8(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_022602E4 *v0 = &param1->unk_58;

    switch (v0->unk_06) {
    case 0:
        v0->unk_00 = Camera_GetDistance(camera);
        v0->unk_06++;
        PlaySE(1493);
    case 1:
        Camera_AdjustDistance(((((fx32)0x0000000000001000L) * 50) / 30), camera);
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
        Camera_AdjustDistance((-(((fx32)0x0000000000001000L) * 50) / 6), camera);
        v0->unk_04++;

        if ((v0->unk_04 >= 6) || (Camera_GetDistance(camera) <= v0->unk_00)) {
            Camera_SetDistance(v0->unk_00, camera);
            return 1;
        }

        break;
    }

    return 0;
}

BOOL ov89_0225C498(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);

BOOL ov89_0225C498(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_02260394 *v0 = &param1->unk_60;
    VecFx32 v1 = { 0, 0, 0 };
    fx32 v2;

    switch (v0->unk_1D) {
    case 0:
        v0->unk_0C = Camera_GetLookAtCamPos(camera);
        v0->unk_00 = Camera_GetLookAtCamTarget(camera);
        v0->unk_1D++;
        PlaySE(1493);
    case 1:
        v0->unk_18 += 0x20000;

        if (v0->unk_18 >= (360 << 12)) {
            v0->unk_18 -= 360 << 12;
            v0->unk_1C++;

            if (v0->unk_1C >= 4) {
                v0->unk_1D++;
                v0->unk_18 = 0;
            }
        }

        v2 = FX_MulInline(GF_SinDegFX32(v0->unk_18), 0x10000);
        v1.x = v2;

        Camera_SetLookAtCamTarget(&v0->unk_00, camera);
        Camera_SetLookAtCamPos(&v0->unk_0C, camera);
        Camera_OffsetLookAtPosAndTarget(&v1, camera);
        break;
    default:
        return 1;
    }

    return 0;
}

BOOL ov89_0225C570(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);

BOOL ov89_0225C570(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_0226046C *v0 = &param1->unk_80;
    VecFx32 v1 = { 0, 0, 0 };
    fx32 v2;

    switch (v0->unk_1D) {
    case 0:
        v0->unk_0C = Camera_GetLookAtCamPos(camera);
        v0->unk_00 = Camera_GetLookAtCamTarget(camera);
        v0->unk_1D++;
        PlaySE(1493);
    case 1:
        v0->unk_18 += 0x20000;

        if (v0->unk_18 >= (360 << 12)) {
            v0->unk_18 -= 360 << 12;
            v0->unk_1C++;

            if (v0->unk_1C >= 4) {
                v0->unk_1D++;
                v0->unk_18 = 0;
            }
        }

        v2 = FX_MulInline(GF_SinDegFX32(v0->unk_18), 0x8000);
        v1.y = v2;

        Camera_SetLookAtCamTarget(&v0->unk_00, camera);
        Camera_SetLookAtCamPos(&v0->unk_0C, camera);
        Camera_OffsetLookAtPosAndTarget(&v1, camera);
        break;
    default:
        return 1;
    }

    return 0;
}

BOOL ov89_0225C648(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera);

BOOL ov89_0225C648(UnkStruct_ov113_0225EB20 *param0, UnkStruct_ov113_0225EB20_sub1 *param1, Camera *camera) {
    UnkStruct_ov113_02260544 *v0 = &param1->unk_A0;
    CameraAngle v1 = { 0, 0, 0, 0 };

    switch (v0->unk_0E) {
    case 0:
        v0->cameraAngle = Camera_GetAngle(camera);
        v0->unk_08 = v0->cameraAngle.x;
        v0->unk_0E++;

        PlaySE(1493);
    case 1:
        v1.x += (-0x2000 / 3);
        v0->unk_08 += (-0x2000 / 3);

        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 <= v0->cameraAngle.x - 0x2000) {
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
        v1.x += (0x2000 / 16);
        v0->unk_08 += (0x2000 / 16);

        Camera_AdjustAnglePos(&v1, camera);

        if (v0->unk_08 >= v0->cameraAngle.x) {
            Camera_SetAnglePos(&v0->cameraAngle, camera);
            return 1;
        }

        break;
    }

    return 0;
}

void ov89_0225C724(MsgData *param_1, MessageFormat *param_2, u8 *param_3, u32 param_4, u32 param_5) {
    u32 uVar1;
    s32 iVar2;
    s32 iVar3;
    PlayerProfile *profile;
    u32 uVar4;
    String *src;
    String *dest;
    uVar1 = ov45_0222EC7C(param_5);
    if (uVar1 != 0xffffffff) {
        iVar2 = ov45_0222EC68(param_5);
        if ((iVar2 != (-1)) && (iVar3 = ov45_0222A578(param_4, iVar2), iVar3 != 0)) {
            String *string;
            if ((7 + 1) <= uVar1) {
                GF_AssertFail();
                return;
            }
            profile = PlayerProfile_New(HEAP_ID_125);
            ov45_0222A844(iVar3, profile, 0x7d);
            string = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_125);
            uVar4 = PlayerProfile_GetTrainerGender(profile);
            BufferString(param_2, 0, string, uVar4, 1, 2);
            src = NewString_ReadMsgData(param_1, 0);
            dest = String_New(0x40, HEAP_ID_125);
            StringExpandPlaceholders(param_2, dest, src);
            iVar3 = uVar1 * 0x10;
            FillWindowPixelBuffer((Window *)(param_3 + iVar3), 0);
            iVar2 = ov45_0222AB28(param_4, iVar2);
            if (iVar2 == 1) {
                uVar4 = 0x70800;
            } else {
                uVar4 = 0x10200;
            }
            AddTextPrinterParameterizedWithColor((Window *)(param_3 + iVar3), FONT_SYSTEM, dest, 0, 0, 0, uVar4, (PrinterCallback_t)0x0);
            CopyWindowToVram((Window *)(param_3 + iVar3));
            String_Delete(string);
            String_Delete(src);
            String_Delete(dest);
            Heap_Free(profile);
        }
    }
    return;
}

void ov89_0225C818(Window param0[], u32 param1) {
    if (param1 >= 8) {
        ((0) ? (void)0 : GF_AssertFail());
        return;
    }

    ClearWindowTilemapAndCopyToVram(&param0[param1]);
}

u16 ov89_0225C830(int param0, u32 param1) {
    return ov89_0225CE70[param1 % NELEMS(ov89_0225CE70)][param0];
}

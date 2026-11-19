#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "font.h"
#include "math_util.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct TextOBJ;

struct GF3DVramMan;

struct MsgData;

struct ParticleSystem;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov117_02261280_t;

struct UnkStruct_ov117_022626B0_t;

struct UnkStruct_ov66_0222DFF8_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

extern int (*const ov93_02262C94[4])(UnkStruct_ov117_02261280_t *, UnkStruct_ov117_022653F4 *, UnkStruct_ov117_02265428 *);

void ov93_02261538(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2);

int ov93_0226154C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2);
void ov93_02261528(UnkStruct_ov117_02265428 *param0, s8 param1);
void ov93_0226249C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 *param1, UnkStruct_ov117_02266344 *param2);
int ov93_02261554(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov117_02266BC4;
extern const UnkStruct_ov117_02266BC4 ov93_02262CC4[5];
int ov93_02261744(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2);

void ov93_02261538(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2) {
    ov93_02262C94[param2->unk_10](param0, param1, param2);
}

int ov93_0226154C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2) {
    param2->unk_15 = 0;
    return 1;
}

int ov93_02261554(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2) {
    s16 v0, v1, v2, v3, v4, v5;
    fx32 v6;
    int v7 = 0;
    fx32 v8;
    f32 v9;

    v2 = 0;
    v3 = 0;
    v0 = param2->unk_1C;
    v1 = param2->unk_1E;

    switch (param2->unk_11) {
    case 0:
        ManagedSprite_SetDrawFlag(param2->unk_00, 1);
        ManagedSprite_SetDrawFlag(param2->unk_08, 1);
        ManagedSprite_GetPositionXYWithSubscreenOffset(param2->unk_00, &v0, &v1, ((192 + 160) << 12));
        param2->unk_1C = v0;
        param2->unk_1E = v1;
        param2->unk_11++;

    case 1:
        switch (param2->unk_0C) {
        case 0:
            v4 = param2->unk_1E - -32;
            v3 = param2->unk_12 * v4 / 15;
            v1 = -32 + v3;
            break;
        case 90 << 12:
            v5 = param2->unk_1C - (256 + 32);
            v2 = param2->unk_12 * v5 / 15;
            v0 = (256 + 32) + v2;
            break;
        case 180 << 12:
            v4 = param2->unk_1E - (196 + 32);
            v3 = param2->unk_12 * v4 / 15;
            v1 = (196 + 32) + v3;
            break;
        case 270 << 12:
            v5 = param2->unk_1C - -32;
            v2 = param2->unk_12 * v5 / 15;
            v0 = -32 + v2;
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }
        if (param2->unk_12 >= 15) {
            ManagedSprite_SetPositionXYWithSubscreenOffset(param2->unk_00, param2->unk_1C, param2->unk_1E, ((192 + 160) << 12));
            ManagedSprite_SetPositionXYWithSubscreenOffset(param2->unk_08, param2->unk_1C, param2->unk_1E + 24, ((192 + 160) << 12));
            ov93_02261528(param2, 2);
            ov93_0226249C(param0, param2, &param2->unk_30);

            return 1;
        }

        v6 = ((180 * param2->unk_12) << 12) / 15;
        v7 = -(FX_MulInline(GF_SinDegFX32(v6), (24 << 12))) / ((fx32)0x0000000000001000L);

        ManagedSprite_SetPositionXYWithSubscreenOffset(param2->unk_00, v0, v1 + v7, ((192 + 160) << 12));
        ManagedSprite_SetPositionXYWithSubscreenOffset(param2->unk_08, v0, v1 + 24, ((192 + 160) << 12));

        v8 = ((fx32)0x0000000000001000L) - (((fx32)0x0000000000001000L) * (-v7 / 3) / ((24 << 12) >> 12));
        v9 = ((f32)((v8) / (f32)(1 << 12)));

        ManagedSprite_SetAffineScale(param2->unk_08, v9, v9);
        break;
    }

    param2->unk_12++;
    return 1;
}

int ov93_02261744(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1, UnkStruct_ov117_02265428 *param2) {
    fx32 v0, v1;
    fx32 v2;
    int v3 = 0;
    fx32 v4;
    f32 v5;

    if (param1->unk_F3 > 0) {
        ManagedSprite_TickNFrames(param2->unk_00, ((8 + 8 + 12 + 8 + 8) << FX32_SHIFT) / ov93_02262CC4[param1->unk_F4].unk_00);
    } else {
        ManagedSprite_SetAnimationFrame(param2->unk_00, 0);
    }

    v2 = ((180 * param1->unk_F3) << FX32_SHIFT) / ov93_02262CC4[param1->unk_F4].unk_00;
    v3 = -(FX_Mul(GF_SinDegFX32(v2), (12 << FX32_SHIFT))) / FX32_ONE;
    v0 = 256 / 2 + FX_Mul(GF_SinDegFX32(param1->unk_E4 + param2->unk_0C), 76);
    v1 = 196 / 2 + (-FX_Mul(GF_CosDegFX32(param1->unk_E4 + param2->unk_0C), (64 + 4)));

    ManagedSprite_SetPositionXYWithSubscreenOffset(param2->unk_00, v0, v1 + -24 + v3, ((192 + 160) << FX32_SHIFT));
    ManagedSprite_SetPositionXYWithSubscreenOffset(param2->unk_08, v0, v1 + -24 + 24, ((192 + 160) << FX32_SHIFT));

    v4 = FX32_ONE - (FX32_ONE * (-v3 / 3) / ((12 << FX32_SHIFT) >> FX32_SHIFT));
    v5 = FX_FX32_TO_F32(v4);

    ManagedSprite_SetAffineScale(param2->unk_08, v5, v5);

    if (param1->unk_F3 == ov93_02262CC4[param1->unk_F4].unk_00 - 1) {
        ov93_0226249C(param0, param2, &param2->unk_30);
    }

    if (((param1->unk_E4 + param2->unk_0C) >> FX32_SHIFT) % 360 == 180) {
        param2->unk_15 = 1;
    } else {
        param2->unk_15 = 0;
    }

    return 1;
}

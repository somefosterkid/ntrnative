#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "math_util.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
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

void ov93_02260608(UnkStruct_ov117_02261280_t *param0);

void ov93_0225FC8C(BgConfig *param0, UnkStruct_ov117_02263DAC *param1);
void ov93_02260314(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264808 *param1, UnkStruct_ov117_02263DAC *param2);
void ov93_02260B84(UnkStruct_ov117_02261280_t *param0);
BOOL ov93_02260908(UnkStruct_ov117_02261280_t *param0);

void ov93_02260608(UnkStruct_ov117_02261280_t *param0) {
    int v0;
    UnkStruct_ov117_02264508 *v1, *v2, *v3;

    for (v0 = 0; v0 < 36; v0++) {
        v1 = &param0->unk_339C.unk_04[v0];

        if (v1->unk_00 != NULL) {
            Sprite_DeleteAndFreeResources(v1->unk_00);
        }
    }

    for (v0 = 0; v0 < 8; v0++) {
        v2 = &param0->unk_339C.unk_364[v0];

        if (v2->unk_00 != NULL) {
            Sprite_DeleteAndFreeResources(v2->unk_00);
        }
    }

    for (v0 = 0; v0 < 3; v0++) {
        v3 = &param0->unk_339C.unk_424[v0];

        if (v3->unk_00 != NULL) {
            Sprite_DeleteAndFreeResources(v3->unk_00);
        }
    }
}

BOOL ov93_02260660(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_02260660(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_02264808 *v0 = &param0->unk_339C;
    UnkStruct_ov117_02264508 *v1, *v2, *v3;
    s16 v4, v5;
    int v6 = 0;
    int v7;

    if (v0->unk_00 == 0) {
        return 0;
    }

    for (v7 = 0; v7 < 36; v7++) {
        v1 = &v0->unk_04[v7];

        if (v1->unk_00 != NULL) {
            if (v1->unk_14 == 0) {
                Sprite_DeleteAndFreeResources(v1->unk_00);
                v1->unk_00 = NULL;
                continue;
            }

            v1->unk_0C += v1->unk_10;
            v1->unk_04 += v1->unk_08;

            if (v1->unk_04 >= (128 + 32) << FX32_SHIFT) {
                v1->unk_04 = (128 + 32) << FX32_SHIFT;
            }

            v4 = 128 + (FX_Mul(GF_SinDegFX32(v1->unk_0C), v1->unk_04)) / FX32_ONE;
            v5 = 96 + (-FX_Mul(GF_CosDegFX32(v1->unk_0C), v1->unk_04)) / FX32_ONE;

            ManagedSprite_SetPositionXYWithSubscreenOffset(v1->unk_00, v4, v5, ((192 + 160) << FX32_SHIFT));

            v1->unk_14--;
            v6++;
        }
    }

    for (v7 = 0; v7 < 8; v7++) {
        v2 = &v0->unk_364[v7];

        if (v2->unk_00 != NULL) {
            if (v2->unk_14 == 0) {
                Sprite_DeleteAndFreeResources(v2->unk_00);
                v2->unk_00 = NULL;
                continue;
            }

            v2->unk_0C += v2->unk_10;
            v2->unk_04 += v2->unk_08;

            if (v2->unk_04 >= (128 + 32) << FX32_SHIFT) {
                v2->unk_04 = (128 + 32) << FX32_SHIFT;
            }

            v4 = 128 + (FX_Mul(GF_SinDegFX32(v2->unk_0C), v2->unk_04)) / FX32_ONE;
            v5 = 96 + (-FX_Mul(GF_CosDegFX32(v2->unk_0C), v2->unk_04)) / FX32_ONE;

            ManagedSprite_SetPositionXYWithSubscreenOffset(v2->unk_00, v4, v5, ((192 + 160) << FX32_SHIFT));

            v2->unk_14--;
            v6++;
        }
    }

    for (v7 = 0; v7 < 3; v7++) {
        v3 = &v0->unk_424[v7];

        if (v3->unk_00 != NULL) {
            if (v3->unk_14 == 0) {
                Sprite_DeleteAndFreeResources(v3->unk_00);
                v3->unk_00 = NULL;
                continue;
            }

            v3->unk_0C += v3->unk_10;
            v3->unk_04 += v3->unk_08;

            if (v3->unk_04 >= (128 + 32) << FX32_SHIFT) {
                v3->unk_04 = (128 + 32) << FX32_SHIFT;
            }

            v4 = 128 + (FX_Mul(GF_SinDegFX32(v3->unk_0C), v3->unk_04)) / FX32_ONE;
            v5 = 96 + (-FX_Mul(GF_CosDegFX32(v3->unk_0C), v3->unk_04)) / FX32_ONE;

            ManagedSprite_SetPositionXYWithSubscreenOffset(v3->unk_00, v4, v5, ((192 + 160) << FX32_SHIFT));

            v3->unk_14--;
            v6++;
        }
    }

    if (v6 == 0) {
        v0->unk_00 = 0;
    }

    return 1;
}

BOOL ov93_02260908(UnkStruct_ov117_02261280_t *param0) {
    s32 i;
    UnkStruct_ov117_02264808 *dest;

    dest = &param0->unk_339C;
    if (dest->unk_00 == 1) {
        return 0;
    }
    MI_CpuFill8(dest, 0, sizeof(UnkStruct_ov117_02264808));
    ov93_02260314(param0, &param0->unk_339C, &param0->unk_1428);
    ov93_0225FC8C(param0->unk_2C, &param0->unk_1428);
    ov93_02260B84(param0);
    param0->unk_2FCC = 0;
    param0->unk_1428.unk_08 = 0;
    param0->unk_1428.unk_10 = 0;
    for (i = 0; i < 4; i++) {
        param0->unk_1428.unk_18[i] = 0;
    }
    PlaySE(0x594);
    dest->unk_00 = 1;
    return 1;
}

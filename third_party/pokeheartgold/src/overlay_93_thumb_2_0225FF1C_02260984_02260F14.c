#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "error_handling.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "sprite_system.h"
#include "unk_02035900.h"
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

void ov93_02260F14(UnkStruct_ov117_02261280_t *param0, int param1, s32 param2, UnkStruct_ov117_02264E84 *param3);

void ov93_02260F14(UnkStruct_ov117_02261280_t *param0, int param1, s32 param2, UnkStruct_ov117_02264E84 *param3) {
    MI_CpuClear8(param3, sizeof(UnkStruct_ov117_02264E84));

    param3->unk_00 = sub_0203769C();
    param3->unk_02 = param1;
    param3->unk_04 = param2;
    param3->unk_08 = param2;
    param3->unk_01 = 0;
    param3->unk_0C = 0;
}

BOOL ov93_02260F3C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264E84 *param1);

BOOL ov93_02260F3C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264E84 *param1) {
    if (param0->unk_2FB0 - param0->unk_2FAC >= 8) {
        return 0;
    }

    param0->unk_2F2C[param0->unk_2FB0 % 8] = *param1;
    param0->unk_2FB0++;
    return 1;
}

UnkStruct_ov117_02264E84 *ov93_02260F84(UnkStruct_ov117_02261280_t *param0);

UnkStruct_ov117_02264E84 *ov93_02260F84(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_02264E84 *v0;

    if (param0->unk_2FAC >= param0->unk_2FB0) {
        return NULL;
    }

    v0 = &param0->unk_2F2C[param0->unk_2FAC % 8];
    param0->unk_2FAC++;
    return v0;
}

BOOL ov93_02260FB8(UnkStruct_ov117_02261280_t *param0, int param1);

BOOL ov93_02260FB8(UnkStruct_ov117_02261280_t *param0, int param1) {
    UnkStruct_ov117_02264E84 *v0 = NULL;
    int v1, v2, v3;

    if ((param0->unk_2FAC == param0->unk_2FB0) || (param1 == 0)) {
        return 0;
    }

    v2 = param0->unk_2FAC % 8;
    v3 = param0->unk_2FB0 % 8;

    if (v2 < v3) {
        for (v1 = v2; v1 < v3; v1++) {
            if (param0->unk_2F2C[v1].unk_0C == 0) {
                v0 = &param0->unk_2F2C[v1];
                break;
            }
        }
    } else {
        for (v1 = v2; v1 < 8; v1++) {
            if (param0->unk_2F2C[v1].unk_0C == 0) {
                v0 = &param0->unk_2F2C[v1];
                break;
            }
        }

        if (v0 == NULL) {
            for (v1 = 0; v1 < v3; v1++) {
                if (param0->unk_2F2C[v1].unk_0C == 0) {
                    v0 = &param0->unk_2F2C[v1];
                    break;
                }
            }
        }
    }

    if (v0 == NULL) {
        GF_ASSERT(FALSE);
        return 0;
    }

    GF_ASSERT(v0->unk_01 == 0);

    v0->unk_01 = param1;
    v0->unk_0C = 1;

    switch (param1) {
    case 1:
        v0->unk_08 = v0->unk_04 * 2;
        break;
    case 2:
        v0->unk_08 = v0->unk_04 / 2;
        break;
    case 3:
        v0->unk_08 = v0->unk_04 * 3;
        break;
    }

    return 1;
}

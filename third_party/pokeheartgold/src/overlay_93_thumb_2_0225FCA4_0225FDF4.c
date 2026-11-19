#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "font.h"
#include "obj_char_transfer.h"
#include "overlay_93_thumb_2.h"
#include "overlay_manager.h"
#include "sprite_system.h"
#include "unk_02035900.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

int ov93_0225FDF4(int param0) {
    if (param0 >= 10) {
        return 2;
    }

    if (param0 >= 5) {
        return 1;
    }

    return 0;
}

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

void ov93_02262098(UnkStruct_ov117_02265EC8 *param0, int param1);

int ov93_0225E3C4(UnkStruct_ov117_02261280_t *param0, int param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

int ov93_0225FE08(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02263DAC *param1, int param2, int param3, int param4);

int ov93_0225FE08(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02263DAC *param1, int param2, int param3, int param4) {
    param1->unk_10 += param2;
    param1->unk_14 = param1->unk_10 / 6;
    param1->unk_18[ov93_0225E3C4(param0, param3)] += param2;

    if (param3 == sub_0203769C()) {
        param0->unk_3848 += param2;
        ov93_02262098(&param0->unk_15A8, param0->unk_3848);
    }

    return param1->unk_10;
}

static int ov93_0225FE5C(UnkStruct_ov117_02263DAC *param0);

static int ov93_0225FE5C(UnkStruct_ov117_02263DAC *param0) {
    int v0;

    if (param0->unk_10 == 0) {
        return 0;
    }

    if (param0->unk_10 >= param0->unk_14) {
        v0 = param0->unk_14;
    } else {
        v0 = param0->unk_10;
    }

    param0->unk_10 -= v0;
    param0->unk_08 += v0;

    return 1;
}

void ov93_0225FCA4(BgConfig *param0, UnkStruct_ov117_02263DAC *param1, int param2);

void ov93_0225FE80(UnkStruct_ov117_02261280_t *param0, BgConfig *param1, UnkStruct_ov117_02263DAC *param2);

void ov93_0225FE80(UnkStruct_ov117_02261280_t *param0, BgConfig *param1, UnkStruct_ov117_02263DAC *param2) {
    if (param0->unk_2FCC == 0) {
        return;
    }

    ov93_0225FE5C(param2);
    ov93_0225FCA4(param1, param2, param0->unk_00->unk_30);
}

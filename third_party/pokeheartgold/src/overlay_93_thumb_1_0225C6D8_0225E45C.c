#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_020379A0.h"
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

int ov93_0225E45C(UnkStruct_ov117_02261280_t *param0, int param1);

int ov93_0225E45C(UnkStruct_ov117_02261280_t *param0, int param1) {
    int v0;

    for (v0 = 0; v0 < param0->unk_00->unk_30; v0++) {
        if (param0->unk_00->unk_2C[v0] == param1) {
            return v0;
        }
    }

    GF_ASSERT(FALSE);
    return 0;
}

void ov93_0225E48C(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2);

void ov93_0225E48C(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2) {
    GF_ASSERT(param0->unk_2FD4 == 0);

    param0->unk_2FD4 = param1;
    param0->unk_2FD8 = param2;
}

BOOL ov93_0225E0F4(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_0225E4B0(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_0225E4B0(UnkStruct_ov117_02261280_t *param0) {
    int v0 = 0;

    switch (param0->unk_2FD4) {
    case 1:
        v0 = 1;
        break;
    case 2:
        v0 = 1;
        break;
    case 3:
        v0 = 1;
        break;
    case 4:
        if (param0->unk_2FCC == 1) {
            v0 = 1;
        }
        break;
    case 5:
        if (param0->unk_339C.unk_00 == 0) {
            v0 = 1;
        }
        break;
    case 6:
        if (param0->unk_174C.unk_1C == 1) {
            v0 = 1;
        }
        break;
    default:
        return 0;
    }

    if (v0 == 1) {
        sub_02037AC0(param0->unk_2FD8);

        if (ov93_0225E0F4(param0) == 1) {
            param0->unk_2FD4 = 7;
        } else {
            param0->unk_2FD4 = 0;
        }

        return 1;
    }

    return 0;
}

static int ov93_0225E548(UnkStruct_ov117_02261280_t *param0, int param1);

static int ov93_0225E548(UnkStruct_ov117_02261280_t *param0, int param1) {
    if (param0->unk_2FD4 != 7) {
        return 0;
    }

    if (param0->unk_2FD8 != param1) {
        return 0;
    }

    if (sub_02037B38(param0->unk_2FD8) == 1) {
        param0->unk_2FD4 = 0;
        return 1;
    }

    return 0;
}

void ov93_022628F4(UnkStruct_ov117_02261280_t *param0, u8 param1);

void ov93_0226297C(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2);

void ov93_022629B8(UnkStruct_ov117_02261280_t *param0, u8 param1);

int ov93_0225E584(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02262664 *param1);

int ov93_0225E584(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02262664 *param1) {
    switch (param1->unk_0C) {
    case 0:
        ov93_0226297C(param0, 1, param1->unk_18);
        param1->unk_0C++;
        break;
    case 1:
        if (ov93_0225E548(param0, param1->unk_18) == 1) {
            param1->unk_18++;
            param1->unk_0C++;
        }
        break;
    case 2:
        ov93_022629B8(param0, param1->unk_18);
        param1->unk_0C++;
        break;
    case 3:
        if (ov93_0225E548(param0, param1->unk_18) == 1) {
            param1->unk_18++;
            param1->unk_0C++;
        }
        break;
    case 4:
        ov93_022628F4(param0, param1->unk_18);
        param1->unk_0C++;
        break;
    case 5:
        if (ov93_0225E548(param0, param1->unk_18) == 1) {
            param1->unk_18++;
            return 1;
        }
        break;
    }
    return 0;
}

int ov93_0225E3B8(UnkStruct_ov117_02261280_t *param0);

int ov93_0225FDF4(int param0);

void ov93_02262830(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2);

void ov93_02262884(UnkStruct_ov117_02261280_t *param0, int param1, int param2, u8 param3);

int ov93_0225E61C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02262664 *param1);

int ov93_0225E61C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02262664 *param1) {
    if (param0->unk_2FB4 > (43 * 30 + 12)) {
        if ((param0->unk_2FD4 == 0) && (param0->unk_2EDC.unk_00 == 0) && (param0->unk_2EF0.unk_00 == 0)) {
            return 1;
        } else {
            (void)0;
        }
    }

    switch (param1->unk_0C) {
    case 0:
        ov93_02262884(param0, param0->unk_2FC8, ov93_0225FDF4(param0->unk_2FC8), param1->unk_18);
        param1->unk_0C++;
        break;
    case 1:
        if (ov93_0225E548(param0, param1->unk_18) == 1) {
            param1->unk_18++;
            param1->unk_0C++;
        }
        break;
    case 2:
        if ((param0->unk_1428.unk_0C != 0) && (param0->unk_1428.unk_08 >= param0->unk_1428.unk_0C)) {
            param0->unk_2FC8++;
            ov93_02262830(param0, ov93_0225E3B8(param0), param1->unk_18);
            param1->unk_0C++;
        }
        break;
    case 3:
        if (ov93_0225E548(param0, param1->unk_18) == 1) {
            param1->unk_18++;
            param1->unk_0C = 0;
        }
        break;
    }

    return 0;
}

BOOL ov93_02262748(UnkStruct_ov117_02261280_t *param0);

void ov93_02262934(UnkStruct_ov117_02261280_t *param0, u8 param1);

int ov93_0225E6F8(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02262664 *param1);

int ov93_0225E6F8(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02262664 *param1) {
    switch (param1->unk_0C) {
    case 0:
        ov93_02262934(param0, param1->unk_18);
        param1->unk_0C++;
        break;
    case 1:
        if (ov93_0225E548(param0, param1->unk_18) == 1) {
            param1->unk_18++;
            param1->unk_0C++;
        }
        break;
    case 2:
        if (param0->unk_20 == 8) {
            param1->unk_0C++;
        }
        break;
    case 3:
        param1->unk_14++;

        if (param1->unk_14 < 90) {
            break;
        }

        if (ov93_02262748(param0) == 1) {
            return 1;
        }
        break;
    }

    return 0;
}

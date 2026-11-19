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
#include "unk_02005D10.h"
#include "error_handling.h"
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

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

void sub_020181D4(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020182A0(Field3dObject *obj, BOOL visible);

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

BOOL ov93_0225DF38(UnkStruct_ov117_02261280_t *param0, int param1);

void sub_020181E0(Field3dObject *obj, Field3DModelAnimation *anim);
void ov93_0225E008(UnkStruct_ov117_02261280_t *param0, int param1, int param2);
BOOL sub_0201815C(Field3DModelAnimation *anim, fx32 frameDelta);
void ov93_0225E03C(UnkStruct_ov117_02261280_t *param0);
void sub_020181EC(Field3dObject *obj);
void ov93_0225E0A4(UnkStruct_ov117_02261280_t *param0);
BOOL ov93_0225E0F4(UnkStruct_ov117_02261280_t *param0);
int ov93_0225E10C(UnkStruct_ov117_02261280_t *param0);
BOOL ov93_0225E1E0(UnkStruct_ov117_02261280_t *param0, int param1, const UnkStruct_ov117_022621D4 *param2);
void ov93_0225E144(UnkStruct_ov117_02261280_t *param0, int param1, const UnkStruct_ov117_022621D4 *param2);
UnkStruct_ov117_022621D4 *ov93_0225E1A0(UnkStruct_ov117_02261280_t *param0, int param1);
BOOL ov93_0225E27C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_0225E230(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
UnkStruct_ov117_022621D4 *ov93_0225E2D4(UnkStruct_ov117_02261280_t *param0);
BOOL ov93_02262724(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022621D4 *param1);
int ov93_0225E300(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_0225DF38(UnkStruct_ov117_02261280_t *param0, int param1) {
    UnkStruct_ov117_02261F08 *v0 = &param0->unk_D8;
    Field3dModel *v1;
    int v2, v3;

    if (param1 > 1200) {
        v3 = 3;
    } else if (param1 > 3 / 3 * 2) {
        v3 = 2;
    } else if (param1 > 3 / 3 * 1) {
        v3 = 1;
    } else {
        v3 = 0;
    }

    v1 = &v0->unk_88[v3];

    for (v2 = 0; v2 < 8; v2++) {
        if (v0->unk_C8[v2][v3].unk_00 == 0) {
            sub_020181B0(&v0->unk_C8[v2][v3].unk_04, v1);
            sub_020182A8(&v0->unk_C8[v2][v3].unk_04, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((-25) > 0) ? ((-25) * (1 << 12) + 0.5f) : ((-25) * (1 << 12) - 0.5f))), ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))));
            sub_020182C4(&v0->unk_C8[v2][v3].unk_04, ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))));
            sub_020182A0(&v0->unk_C8[v2][v3].unk_04, 1);
            sub_02018198(&v0->unk_C8[v2][v3].unk_7C, 0);
            sub_020181D4(&v0->unk_C8[v2][v3].unk_04, &v0->unk_C8[v2][v3].unk_7C);

            v0->unk_C8[v2][v3].unk_00 = 1;

            PlaySE(1425);
            return 1;
        }
    }

    return 0;
}

void ov93_0225E008(UnkStruct_ov117_02261280_t *param0, int param1, int param2) {
    UnkStruct_ov117_02261F08 *v0 = &param0->unk_D8;

    sub_020181E0(&v0->unk_C8[param1][param2].unk_04, &v0->unk_C8[param1][param2].unk_7C);
    v0->unk_C8[param1][param2].unk_00 = 0;
}

void ov93_0225E03C(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_02261F08 *v0 = &param0->unk_D8;
    int v1, v2;

    for (v1 = 0; v1 < 8; v1++) {
        for (v2 = 0; v2 < 4; v2++) {
            if (v0->unk_C8[v1][v2].unk_00 == 1) {
                if (sub_0201815C(&v0->unk_C8[v1][v2].unk_7C, FX32_ONE) == 1) {
                    ov93_0225E008(param0, v1, v2);
                }
            }
        }
    }
}

void ov93_0225E0A4(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_02261F08 *v0 = &param0->unk_D8;
    int v1, v2;

    for (v1 = 0; v1 < 8; v1++) {
        for (v2 = 0; v2 < 4; v2++) {
            if (v0->unk_C8[v1][v2].unk_00 == 1) {
                sub_020181EC(&v0->unk_C8[v1][v2].unk_04);
            }
        }
    }
}

BOOL ov93_0225E0F4(UnkStruct_ov117_02261280_t *param0) {
    if (param0->unk_04 == sub_0203769C()) {
        return 1;
    }

    return 0;
}

int ov93_0225E10C(UnkStruct_ov117_02261280_t *param0) {
    int v0, v1;
    u32 v2;

    v0 = param0->unk_00->unk_2C[0];
    v2 = param0->unk_0C[0];

    for (v1 = 1; v1 < param0->unk_00->unk_30; v1++) {
        if (v2 < param0->unk_0C[v1]) {
            v2 = param0->unk_0C[v1];
            v0 = param0->unk_00->unk_2C[v1];
        }
    }

    param0->unk_04 = v0;
    return param0->unk_04;
}

void ov93_0225E144(UnkStruct_ov117_02261280_t *param0, int param1, const UnkStruct_ov117_022621D4 *param2) {
    UnkStruct_ov117_022621D4 *v0;
    int v1;

    if (ov93_0225E1E0(param0, param1, param2) == 1) {
        return;
    }

    v1 = param0->unk_2F04[param1] % 60;
    v0 = &param0->unk_1C1C[param1].unk_00[v1];

    param0->unk_2F04[param1]++;

    GF_ASSERT(v0->unk_00 == 0);

    *v0 = *param2;
}

UnkStruct_ov117_022621D4 *ov93_0225E1A0(UnkStruct_ov117_02261280_t *param0, int param1) {
    UnkStruct_ov117_022621D4 *v0;
    int v1 = param0->unk_2F14[param1] % 60;
    v0 = &param0->unk_1C1C[param1].unk_00[v1];

    if (v0->unk_00 != 0) {
        param0->unk_2F14[param1]++;
        return v0;
    }

    return NULL;
}

BOOL ov93_0225E1E0(UnkStruct_ov117_02261280_t *param0, int param1, const UnkStruct_ov117_022621D4 *param2) {
    switch (param2->unk_00) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        GF_ASSERT(param0->unk_2EF0.unk_00 == 0);
        param0->unk_2EF0 = *param2;
        return 1;
    }

    return 0;
}

void ov93_0225E230(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    UnkStruct_ov117_022621D4 *v0;
    int v1;

    if (ov93_0225E27C(param0, param1) == 1) {
        return;
    }

    v1 = param0->unk_2F24 % 60;
    v0 = &param0->unk_176C.unk_00[v1];

    param0->unk_2F24++;

    GF_ASSERT(v0->unk_00 == 0);

    *v0 = *param1;
}

BOOL ov93_0225E27C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    switch (param1->unk_00) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (param0->unk_2EDC.unk_00 == 0) {
            param0->unk_2EDC = *param1;
            return 1;
        } else if (param1->unk_00 == param0->unk_2EDC.unk_00) {
            return 1;
        } else {
            GF_ASSERT(FALSE);
            return 0;
        }
        break;
    }

    return 0;
}

UnkStruct_ov117_022621D4 *ov93_0225E2D4(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_022621D4 *v0;
    int v1 = param0->unk_2F28 % 60;
    v0 = &param0->unk_176C.unk_00[v1];
    if (v0->unk_00 != 0) {
        return v0;
    }

    return NULL;
}

int ov93_0225E300(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_022621D4 *v0;

    if (param0->unk_2FC4 > 0) {
        param0->unk_2FC4--;
        return 0;
    }

    if (param0->unk_2EDC.unk_00 != 0) {
        if (ov93_02262724(param0, &param0->unk_2EDC) == 1) {
            param0->unk_2EDC.unk_00 = 0;
            return 1;
        } else {
            return 0;
        }
    }

    v0 = ov93_0225E2D4(param0);

    if (v0 == NULL) {
        return 0;
    }

    if (ov93_02262724(param0, v0) == 1) {
        v0->unk_00 = 0;
        param0->unk_2F28++;
        return 1;
    }

    return 0;
}

#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

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
#include "unk_02009D48.h"
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

extern const ManagedSpriteTemplate ov93_02262E34;

ManagedSprite *ov93_02260984(UnkStruct_ov117_02261280_t *param0, int param1, int param2);

BOOL ov93_02260A58(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264930 *param1, int param2, int param3);
void ov93_022609E0(UnkStruct_ov117_02261280_t *param0);
void ov93_02260A14(UnkStruct_ov117_02261280_t *param0);
void ov93_02260A8C(UnkStruct_ov117_02264930 *param0);
void ov93_02260AD8(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264930 *param1);
BOOL ov93_02260A30(UnkStruct_ov117_02261280_t *param0);
int ov93_02260B70(int param0);

ManagedSprite *ov93_02260984(UnkStruct_ov117_02261280_t *param0, int param1, int param2) {
    ManagedSprite *v0;
    ManagedSpriteTemplate v1;

    v1 = ov93_02262E34;
    v0 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &v1);

    ManagedSprite_SetPositionXYWithSubscreenOffset(v0, (9 * 8) - param2 * 16, (4 * 8), ((192 + 160) << FX32_SHIFT));
    ManagedSprite_SetAnim(v0, 0 + param1);
    Sprite_TickFrame(v0->sprite);
    ManagedSprite_SetAnimateFlag(v0, 1);

    return v0;
}

void ov93_022609E0(UnkStruct_ov117_02261280_t *param0) {
    int v0;
    UnkStruct_ov117_02264930 *v1;

    for (v0 = 0; v0 < 5; v0++) {
        v1 = &param0->unk_380C[v0];
        ov93_02260A58(param0, v1, v0, v0);
        ManagedSprite_TickNFrames(v1->unk_00, FX32_ONE * 4 * v0);
    }
}

void ov93_02260A14(UnkStruct_ov117_02261280_t *param0) {
    int v0;
    UnkStruct_ov117_02264930 *v1;

    for (v0 = 0; v0 < 5; v0++) {
        v1 = &param0->unk_380C[v0];
        Sprite_DeleteAndFreeResources(v1->unk_00);
    }
}

BOOL ov93_02260A30(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_02264930 *v0;
    int v1;

    for (v1 = 0; v1 < 5; v1++) {
        v0 = &param0->unk_380C[v1];
        ov93_02260A8C(v0);
        ov93_02260AD8(param0, v0);
    }

    return 1;
}

BOOL ov93_02260A58(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264930 *param1, int param2, int param3) {
    MI_CpuClear8(param1, sizeof(UnkStruct_ov117_02264930));

    param1->unk_04 = ov93_02260B70(param3);
    param1->unk_05 = 1;
    param1->unk_00 = ov93_02260984(param0, param1->unk_04, param2);
    param1->unk_06 = param2;
    param1->unk_08 = param3;

    return 1;
}

void ov93_02260A8C(UnkStruct_ov117_02264930 *param0) {
    s16 v0, v1, v2;

    v0 = (9 * 8) - param0->unk_06 * 16;

    ManagedSprite_GetPositionXYWithSubscreenOffset(param0->unk_00, &v1, &v2, ((192 + 160) << 12));

    if (v0 > v1) {
        v1 += 2;
        if (v1 > v0) {
            v1 = v0;
        }

        ManagedSprite_SetPositionXYWithSubscreenOffset(param0->unk_00, v1, v2, ((192 + 160) << 12));
    }
}

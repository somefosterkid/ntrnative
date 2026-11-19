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
#include "math_util.h"
#include "overlay_93_thumb_2.h"
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

extern const ManagedSpriteTemplate ov93_02262F38;

void ov93_02262444(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266344 *param1);

void ov93_02262484(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266344 *param1);
void ov93_0226249C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 *param1, UnkStruct_ov117_02266344 *param2);
void ov93_02262540(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 *param1, UnkStruct_ov117_02266344 *param2);

void ov93_02262444(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266344 *param1) {
    ManagedSprite *v0;
    int v1;

    for (v1 = 0; v1 < 2; v1++) {
        v0 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &ov93_02262F38);

        ManagedSprite_SetAnim(v0, 27);
        ManagedSprite_SetOamMode(v0, GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawFlag(v0, 0);
        Sprite_TickFrame(v0->sprite);

        param1->unk_00[v1] = v0;
    }
}

void ov93_02262484(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266344 *param1) {
    int v0;

    for (v0 = 0; v0 < 2; v0++) {
        Sprite_DeleteAndFreeResources(param1->unk_00[v0]);
    }
}

void ov93_0226249C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 *param1, UnkStruct_ov117_02266344 *param2) {
    s16 v0, v1;
    int v2;

    ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_00, &v0, &v1, ((192 + 160) << 12));

    for (v2 = 0; v2 < 2; v2++) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(param2->unk_00[v2], v0, v1 + 28, ((192 + 160) << 12));
        ManagedSprite_SetAnim(param2->unk_00[v2], 27);
        ManagedSprite_SetOamMode(param2->unk_00[v2], GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawFlag(param2->unk_00[v2], 1);

        param2->unk_08[v2] = LCRandom() % 0x1000 + 0x1000;
        param2->unk_10[v2] = LCRandom() % 0x400 + 0x400;

        if (v2 & 1) {
            param2->unk_08[v2] *= -1;
        }
    }

    param2->unk_19 = 0;
    param2->unk_18 = 1;
}

void ov93_02262540(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 *param1, UnkStruct_ov117_02266344 *param2) {
    int v0;

    switch (param2->unk_18) {
    case 0:
        break;
    case 1:
        for (v0 = 0; v0 < 2; v0++) {
            ManagedSprite_AddSpritePrecisePositionXY(param2->unk_00[v0], param2->unk_08[v0], -param2->unk_10[v0]);
            Sprite_TickFrame(param2->unk_00[v0]->sprite);
        }

        param2->unk_19++;

        if (param2->unk_19 > 8) {
            for (v0 = 0; v0 < 2; v0++) {
                ManagedSprite_SetDrawFlag(param2->unk_00[v0], 0);
            }

            param2->unk_18 = 0;
        }

        break;
    }
}

s32 ov93_02262598(OverlayManager *man) {
    void **dest;

    dest = OverlayManager_CreateAndGetData(man, 0xCU, HEAP_ID_117);
    MI_CpuFill8(dest, 0U, 0xCU);
    *dest = OverlayManager_GetArgs(man);
    return 1;
}

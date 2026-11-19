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

BOOL ov93_02261D3C(UnkStruct_ov117_02265C3C *param0, UnkStruct_ov117_02265C1C *param1);

void ov93_02261D1C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1);

extern const u16 ov93_02262C7A[4];

void ov93_02261D1C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1) {
    int v0;
    UnkStruct_ov117_02265C1C *v1;

    for (v0 = 0; v0 < 3; v0++) {
        if (param1->unk_00[v0].unk_15 == 1) {
            v1 = &param1->unk_00[v0];
            ov93_02261D3C(param1, v1);
        }
    }
}

BOOL ov93_02261D3C(UnkStruct_ov117_02265C3C *param0, UnkStruct_ov117_02265C1C *param1) {
    s16 v0;
    s16 v1;
    switch (param1->unk_13) {
    case 0:
        ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_00, &v0, &v1, (192 + 160) << 12);
        param1->unk_08 = v0 * ((fx32)0x0000000000001000L);
        param1->unk_0C = v1 * ((fx32)0x0000000000001000L);

        param1->unk_13++;
    case 1:
        if (param1->unk_16 > 0) {
            param1->unk_16--;
            break;
        }
        ManagedSprite_SetDrawFlag(param1->unk_00, 1);
        param1->unk_0C += 0x8000;
        if ((param1->unk_0C / ((fx32)0x0000000000001000L)) >= param1->unk_10) {
            param1->unk_0C = param1->unk_10 * ((fx32)0x0000000000001000L);
            param1->unk_13++;
        }
        break;

    case 2:
        ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_00, &v0, &v1, (192 + 160) << 12);
        ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_04, v0, v1 + (-32), (192 + 160) << 12);
        ManagedSprite_SetAnim(param1->unk_04, 33);
        ManagedSprite_SetDrawFlag(param1->unk_04, 1);
        PlaySE(1427);
        ManagedSprite_SetAnim(param1->unk_00, ov93_02262C7A[param1->unk_12] + 2);
        param1->unk_14 = 8;
        param1->unk_13++;
        break;

    case 3:
        param1->unk_14--;
        if (param1->unk_14 == 3) {
            ManagedSprite_TickNFrames(param1->unk_04, 4 * ((fx32)0x0000000000001000L));
        }
        if (param1->unk_14 == 0) {
            ManagedSprite_SetDrawFlag(param1->unk_04, 0);
            ManagedSprite_SetAnim(param1->unk_00, ov93_02262C7A[param1->unk_12] + 0);
            param1->unk_13++;
        }
        break;

    case 4:
        param1->unk_0C -= 0x8000;
        if ((param1->unk_0C / ((fx32)0x0000000000001000L)) <= (param1->unk_10 + (-96))) {
            ManagedSprite_SetDrawFlag(param1->unk_00, 0);
            param1->unk_13 = 0;
            param1->unk_15 = 0;
            return 0;
        }
        break;
    }

    ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00, param1->unk_08 / ((fx32)0x0000000000001000L), param1->unk_0C / ((fx32)0x0000000000001000L), (192 + 160) << 12);
    return 1;
}

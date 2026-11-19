#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
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

BOOL ov93_02262374(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266274 *param1);

BOOL ov93_02262374(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266274 *param1) {
    switch (param1->unk_04) {
    case 0:
    case 5:
        ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00, 128, 16, ((192 + 160) << FX32_SHIFT));
        ManagedSprite_SetDrawFlag(param1->unk_00, 1);
        param1->unk_04++;
        break;
    case 1:
    case 6:
        param1->unk_06++;

        if (param1->unk_06 > 15) {
            param1->unk_06 = 0;
            param1->unk_14 = 1;
            param1->unk_18 = 1;
            param1->unk_0C = 128;
            param1->unk_10 = 32;
            param1->unk_04++;
        }

        break;
    case 2:
    case 7:
        param1->unk_18 = 0;
        param1->unk_10 += 6;

        ManagedSprite_OffsetPositionXY(param1->unk_00, 0, 6);

        if (param1->unk_10 > 192 - 96) {
            param1->unk_04++;
        }

        break;
    case 3:
    case 8:
        param1->unk_14 = 0;
        ManagedSprite_SetDrawFlag(param1->unk_00, 0);
        param1->unk_04++;

    case 4:
    case 9:
        param1->unk_06++;

        if (param1->unk_06 > 30) {
            param1->unk_06 = 0;
            param1->unk_04++;
        }

        break;
    case 10:
        return 1;
    }

    return 0;
}

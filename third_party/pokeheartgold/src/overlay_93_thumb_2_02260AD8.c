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

int ov93_0225FEAC(UnkStruct_ov117_02261280_t *param0);

int ov93_02260B70(int param0);

void ov93_02260AD8(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264930 *param1);

void ov93_02260B84(UnkStruct_ov117_02261280_t *param0);

void ov93_02260AD8(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264930 *param1) {
    int v0, v1;
    UnkStruct_ov117_02264930 *v2;

    if (param1->unk_06 != 0) {
        return;
    }

    switch (param1->unk_05) {
    case 3:
        if (ManagedSprite_IsAnimated(param1->unk_00) == 0) {
            param1->unk_08 += 5;
            param1->unk_04 = ov93_02260B70(param1->unk_08);
            param1->unk_05 = 1;

            ManagedSprite_SetAnim(param1->unk_00, 0 + param1->unk_04);
            ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00, (9 * 8) - 5 * 16, (4 * 8), ((192 + 160) << 12));

            for (v1 = 0; v1 < 5; v1++) {
                v2 = &param0->unk_380C[v1];

                if (v2->unk_06 == 0) {
                    v2->unk_06 = 5 - 1;
                } else {
                    v2->unk_06--;
                }
            }
        }
        break;

    case 1:
        v0 = ov93_0225FEAC(param0);

        if ((param1->unk_05 == 1) && (v0 > 75)) {
            param1->unk_05 = 2;
            ManagedSprite_SetAnim(param1->unk_00, 3 + param1->unk_04);
            PlaySE(1430);
        }

        break;
    }
}

int ov93_02260B70(int param0) {
    if (param0 >= 10) {
        return 2;
    }

    if (param0 >= 5) {
        return 1;
    }

    return 0;
}

void ov93_02260B84(UnkStruct_ov117_02261280_t *param0) {
    int v0;
    UnkStruct_ov117_02264930 *v1;

    for (v0 = 0; v0 < 5; v0++) {
        v1 = &param0->unk_380C[v0];

        if (v1->unk_06 == 0) {
            v1->unk_05 = 3;
            ManagedSprite_SetAnim(v1->unk_00, 6);
            return;
        }
    }
}

void ov93_02260BB0(UnkStruct_ov117_02261280_t *arg0) {
    if (arg0->unk_00->unk_30 == 3) {
        BgTilemapRectChangePalette(arg0->unk_2C, 4U, 0U, 0xDU, 0xCU, 4U, 3U);
        BgTilemapRectChangePalette(arg0->unk_2C, 4U, 0x14U, 0xDU, 0xCU, 4U, 0U);
    }
}

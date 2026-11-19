#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/overlay_01_021FB878.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_texture.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02014DA0.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct FontOAM;

struct UnkStruct_02012744_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov117_02261280_t;

struct UnkStruct_ov117_022626B0_t;

struct UnkStruct_ov66_0222DFF8_t;

typedef u16 charcode_t;

int ov93_0225E3C4(UnkStruct_ov117_02261280_t *param0, int param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

extern const u16 ov93_02262D7C[5][4];

extern __attribute__((aligned(4))) const u16 _02262C6C[3];

extern const ManagedSpriteTemplate ov93_02262E00;

void ov93_02260314(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264808 *param1, UnkStruct_ov117_02263DAC *param2);

void ov93_02260314(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264808 *param1, UnkStruct_ov117_02263DAC *param2) {
    ManagedSpriteTemplate v0;
    ManagedSprite *v1;
    int v2;
    int v3[4];
    int v4, v6;
    int v7 = 0;
    int v8, v9;

    v0 = ov93_02262E00;
    v4 = param2->unk_08 + param2->unk_10;

    for (v8 = 0; v8 < param0->unk_00->unk_30; v8++) {
        v2 = ov93_0225E3C4(param0, param0->unk_00->unk_2C[v8]);
        v3[v8] = param2->unk_18[v2] * 36 / v4;
        v6 = ov93_02262D7C[param0->unk_00->unk_30][v2];

        for (v9 = 0; v9 < v3[v8]; v9++) {
            UnkStruct_ov117_02264508 *v10 = &param1->unk_04[v7];

            v1 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &v0);

            if (v1 == 0L) {
                break;
            }

            ManagedSprite_SetPositionXYWithSubscreenOffset(v1, 128, 96, ((192 + 160) << 12));
            ManagedSprite_SetAnim(v1, v6);
            Sprite_TickFrame(v1->sprite);

            v10->unk_04 = LCRandom() % 0x2000 + 0x2000;
            v10->unk_08 = LCRandom() % 0x4000 + 0x2000;
            v10->unk_0C = (LCRandom() % 360) << 12;
            v10->unk_10 = LCRandom() % 0x14000 + 0xa000;
            v10->unk_14 = LCRandom() % 15 + 20;

            param1->unk_04[v7].unk_00 = v1;

            v7++;
        }
    }

    v0.drawPriority = 14;
    v0.pal = 0;

    for (v8 = 0; v8 < 8; v8++) {
        UnkStruct_ov117_02264508 *v11 = &param1->unk_364[v8];

        v1 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &v0);

        if (v1 == 0L) {
            break;
        }

        ManagedSprite_SetPositionXYWithSubscreenOffset(v1, 128, 96, ((192 + 160) << 12));
        ManagedSprite_SetAnim(v1, 28 + LCRandom() % 3);
        Sprite_TickFrame(v1->sprite);

        v11->unk_04 = LCRandom() % 0x2000 + 0x2000;
        v11->unk_08 = LCRandom() % 0x4000 + 0x2000;
        v11->unk_0C = (LCRandom() % 360) << 12;
        v11->unk_10 = LCRandom() % 0x14000 + 0x10000;
        v11->unk_14 = LCRandom() % 15 + 20;

        param1->unk_364[v8].unk_00 = v1;
    }

    v0.drawPriority = 13;
    v0.pal = _02262C6C[param2->unk_04];

    for (v8 = 0; v8 < 3; v8++) {
        UnkStruct_ov117_02264508 *v12 = &param1->unk_424[v8];

        v1 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &v0);

        if (v1 == 0L) {
            break;
        }

        ManagedSprite_SetPositionXYWithSubscreenOffset(v1, 128, 96, ((192 + 160) << 12));
        ManagedSprite_SetAnim(v1, 11);
        Sprite_TickFrame(v1->sprite);

        v12->unk_04 = LCRandom() % 0x2000 + 0x2000;
        v12->unk_08 = LCRandom() % 0x4000 + 0x2000;
        v12->unk_0C = (LCRandom() % 360) << 12;
        v12->unk_10 = LCRandom() % 0x14000 + 0x10000;
        v12->unk_14 = LCRandom() % 15 + 20;

        param1->unk_424[v8].unk_00 = v1;
    }
}

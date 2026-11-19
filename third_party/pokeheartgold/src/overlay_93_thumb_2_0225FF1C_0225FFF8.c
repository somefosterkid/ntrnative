#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
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

int ov93_0225FE08(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02263DAC *param1, int param2, int param3, int param4);

BOOL ov93_02260080(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02263EF8 *param1);

void ov93_0225FFF8(UnkStruct_ov117_02261280_t *param0);

void ov93_0225FFF8(UnkStruct_ov117_02261280_t *param0) {
    int v0;
    UnkStruct_ov117_02263EF8 *v1 = param0->unk_2FDC;

    for (v0 = 0; v0 < 40; v0++) {
        if (v1->unk_00 == 1) {
            if (v1->unk_0C > 0) {
                v1->unk_0C--;

                if (v1->unk_0C == 0) {
                    ManagedSprite_SetDrawFlag(v1->unk_10, 1);
                }
            } else if (ov93_02260080(param0, v1) == 1) {
                if ((param0->unk_2FCC == 1) && (param0->unk_2FBC == 0)) {
                    ov93_0225FE08(param0, &param0->unk_1428, v1->unk_08, v1->unk_04, v1->unk_0E);
                }

                Sprite_DeleteAndFreeResources(v1->unk_10);
                v1->unk_00 = 0;
            }
        }

        v1++;
    }
}

#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
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
void ov93_02261C58(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1, int param2, int param3, int param4);
int ov93_0225FF1C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1, UnkStruct_ov117_02263EF8 *param2);

int ov93_0225FEAC(UnkStruct_ov117_02261280_t *param0);
BOOL ov93_0225FEC4(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1);

int ov93_0225FEAC(UnkStruct_ov117_02261280_t *param0) {
    UnkStruct_ov117_02263DAC *v0 = &param0->unk_1428;
    return 100 * v0->unk_08 / v0->unk_0C;
}

BOOL ov93_0225FEC4(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1) {
    int v0, v1;

    if (param0->unk_2FC8 != param1->unk_02) {
        (void)0;
    }

    for (v0 = 0; v0 < 40; v0++) {
        if (param0->unk_2FDC[v0].unk_00 == 0) {
            v1 = ov93_0225FF1C(param0, param1, &param0->unk_2FDC[v0]);
            ov93_02261C58(param0, &param0->unk_1560, param1->unk_01, param1->unk_00, v1);

            return 1;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
    return 0;
}

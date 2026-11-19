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

int ov93_022614F4(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 **param1);

int ov93_022614F4(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 **param1) {
    UnkStruct_ov117_022653F4 *v0 = &param0->unk_1468;
    int v1;

    for (v1 = 0; v1 < 3; v1++) {
        if (v0->unk_00[v1].unk_15 == 1) {
            *param1 = &v0->unk_00[v1];
            return v0->unk_00[v1].unk_14;
        }
    }

    *param1 = NULL;
    return 0;
}

void ov93_02261528(UnkStruct_ov117_02265428 *arg0, s8 arg1);

void ov93_02261528(UnkStruct_ov117_02265428 *arg0, s8 arg1) {
    *((s8 *)&arg0->unk_10) = arg1;
    *((s8 *)&arg0->unk_11) = 0;
    *((s8 *)&arg0->unk_12) = 0;
    arg0->unk_18 = 0;
    *((s8 *)&arg0->unk_15) = 0;
}

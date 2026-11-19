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

BOOL ov93_0225E0F4(UnkStruct_ov117_02261280_t *param0);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

extern int (*const ov93_02262A20[3])(UnkStruct_ov117_02261280_t *, UnkStruct_ov117_02262664 *);

BOOL ov93_0225E764(UnkStruct_ov117_02261280_t *param0);

void ov93_0225E7AC(void);

BOOL ov93_0225E764(UnkStruct_ov117_02261280_t *param0) {
    int v0;
    UnkStruct_ov117_02262664 *v1;

    if (ov93_0225E0F4(param0) == 0) {
        return 0;
    }

    v1 = &param0->unk_B8;

    if (v1->unk_00 >= NELEMS(ov93_02262A20)) {
        return 1;
    }

    v0 = ov93_02262A20[v1->unk_00](param0, v1);

    if (v0 == 1) {
        v1->unk_00++;
        v1->unk_0C = 0;
        v1->unk_14 = 0;
        v1->unk_10 = 0;
    }

    return 0;
}

void ov93_0225E7AC(void) {
}

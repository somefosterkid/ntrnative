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

void sub_02018068(Field3dModel *model);

void sub_020180F8(Field3DModelAnimation *anim, NNSFndAllocator *allocator);

void ov93_0225DED0(UnkStruct_ov117_02261280_t *param0);

void ov93_0225DED0(UnkStruct_ov117_02261280_t *param0) {
    int v0, v1;
    UnkStruct_ov117_02261F08 *v2 = &param0->unk_D8;

    sub_02018068(&v2->unk_00);

    for (v0 = 0; v0 < 4; v0++) {
        sub_02018068(&v2->unk_88[v0]);
    }

    for (v0 = 0; v0 < 8; v0++) {
        for (v1 = 0; v1 < 4; v1++) {
            sub_020180F8(&v2->unk_C8[v0][v1].unk_7C, &param0->unk_A8);
        }
    }

    sub_02018068(&param0->unk_13A0.unk_00);
}

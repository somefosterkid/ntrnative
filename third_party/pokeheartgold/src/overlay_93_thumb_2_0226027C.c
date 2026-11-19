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

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

extern const ManagedSpriteTemplate ov93_02262DCC;

ManagedSprite *ov93_0226027C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02266F10 *param1);

void ov93_022602E4(UnkStruct_ov117_02261280_t *param0);

ManagedSprite *ov93_0226027C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02266F10 *param1) {
    ManagedSprite *v0;
    ManagedSpriteTemplate v1;

    v1 = ov93_02262DCC;
    v1.x = param1->unk_00;
    v1.y = param1->unk_02;
    v1.pal = param1->unk_06;
    v0 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &v1);

    ManagedSprite_SetPositionXYWithSubscreenOffset(v0, param1->unk_00, param1->unk_02, ((192 + 160) << FX32_SHIFT));
    ManagedSprite_SetAnim(v0, param1->unk_04);
    ManagedSprite_SetAffineOverwriteMode(v0, AFFINE_OVERWRITE_MODE_DOUBLE);
    Sprite_TickFrame(v0->sprite);

    return v0;
}

void ov93_022602E4(UnkStruct_ov117_02261280_t *param0) {
    int v0;

    for (v0 = 0; v0 < 40; v0++) {
        if (param0->unk_2FDC[v0].unk_00 == 1) {
            Sprite_DeleteAndFreeResources(param0->unk_2FDC[v0].unk_10);
            param0->unk_2FDC[v0].unk_00 = 0;
        }
    }
}

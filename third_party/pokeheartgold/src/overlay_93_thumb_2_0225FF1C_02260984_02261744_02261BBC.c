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
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
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

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

extern const ManagedSpriteTemplate ov93_02262E9C;

extern const ManagedSpriteTemplate ov93_02262ED0;

void ov93_02261BBC(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1);

void ov93_02261BBC(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1) {
    ManagedSpriteTemplate v0;
    int v1;

    v0 = ov93_02262E9C;

    for (v1 = 0; v1 < 3; v1++) {
        param1->unk_00[v1].unk_00 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &v0);

        ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00[v1].unk_00, 0, 0, ((192 + 160) << FX32_SHIFT));
        ManagedSprite_SetDrawFlag(param1->unk_00[v1].unk_00, 0);

        param1->unk_00[v1].unk_04 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &ov93_02262ED0);

        ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00[v1].unk_04, 0, 0, ((192 + 160) << FX32_SHIFT));
        ManagedSprite_SetAnim(param1->unk_00[v1].unk_04, 33);
        Sprite_TickFrame(param1->unk_00[v1].unk_04->sprite);
        ManagedSprite_SetDrawFlag(param1->unk_00[v1].unk_04, 0);
    }
}

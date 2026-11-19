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

UnkStruct_ov117_022621D4 *ov93_0225E1A0(UnkStruct_ov117_02261280_t *param0, int param1);

typedef u16 charcode_t;

BOOL ov93_022627C0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022621D4 *param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void ov93_0225E370(UnkStruct_ov117_02261280_t *param0);

void ov93_0225E370(UnkStruct_ov117_02261280_t *param0) {
    int v1;
    UnkStruct_ov117_022621D4 *v2;
    int v3, v4;

    v4 = param0->unk_2EF0.unk_00;

    if (ov93_022627C0(param0, &param0->unk_2EF0) == 1) {
        return;
    }

    v2 = &param0->unk_2EF0;

    for (v1 = 0; v1 < param0->unk_00->unk_30; v1++) {
        while (1) {
            v2 = ov93_0225E1A0(param0, param0->unk_00->unk_2C[v1]);

            if (v2 == 0L) {
                break;
            }

            v4 = v2->unk_00;
            v3 = ov93_022627C0(param0, v2);

            if (v3 == 1) {
                (void)0;
            }
        }
    }
}

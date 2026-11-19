#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_89_unk_struct_ov113_0225_dbcc_t.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "yes_no_prompt.h"
#include "gf_gfx_planes.h"

struct UnkStruct_ov113_0225DBCC_t;

typedef struct UnkStruct_ov113_0225DBCC_t UnkStruct_ov113_0225DBCC;

extern __attribute__((aligned(4))) const s16 ov89_0225CA38[8][2];

extern const ManagedSpriteTemplate ov89_0225CAA0;

extern const ManagedSpriteTemplate ov89_0225CAD4;

void ov89_022597FC(UnkStruct_ov113_0225DBCC *param0);

void ov89_022597FC(UnkStruct_ov113_0225DBCC *param0) {
    int v0;
    ManagedSpriteTemplate v1;

    param0->unk_96C = SpriteSystem_NewSprite(param0->unk_1C, param0->unk_20, &ov89_0225CAA0);

    Sprite_TickFrame(param0->unk_96C->sprite);
    ManagedSprite_SetDrawFlag(param0->unk_96C, 0);

    v1 = ov89_0225CAD4;

    for (v0 = 0; v0 < 8; v0++) {
        v1.resIdList[0] = 10010 + v0;
        param0->unk_970[v0] = SpriteSystem_NewSprite(param0->unk_1C, param0->unk_20, &v1);

        ManagedSprite_SetPositionXYWithSubscreenOffset(param0->unk_970[v0], ov89_0225CA38[v0][0], ov89_0225CA38[v0][1], (192 + 80) << FX32_SHIFT);
        Sprite_TickFrame(param0->unk_970[v0]->sprite);
        ManagedSprite_SetDrawFlag(param0->unk_970[v0], 0);
    }
}

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
void ov89_0225A21C(UnkStruct_ov113_0225DBCC *param0);

void ov89_0225A21C(UnkStruct_ov113_0225DBCC *param0) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        if (param0->unk_990[v0] != 0L) {
            if (ManagedSprite_IsAnimated(param0->unk_990[v0]) == 0) {
                Sprite_DeleteAndFreeResources(param0->unk_990[v0]);
                param0->unk_990[v0] = 0L;
            } else {
                Sprite_TickFrame(param0->unk_990[v0]->sprite);
            }
        }
    }
}

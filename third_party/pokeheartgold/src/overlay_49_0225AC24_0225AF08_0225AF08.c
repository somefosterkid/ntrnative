#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "sprite.h"

BOOL ov49_0225AF08(const UnkStruct_ov70_0225EC20 *param0, u32 param1);

BOOL ov49_0225AF08(const UnkStruct_ov70_0225EC20 *param0, u32 param1) {
    int v0;

    for (v0 = 0; v0 < param0->unk_38; v0++) {
        if (param0->unk_34[v0].value == param1) {
            return 1;
        }
    }

    return 0;
}

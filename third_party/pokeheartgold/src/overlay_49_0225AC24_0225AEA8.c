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

void ov49_0225AEA8(UnkStruct_ov70_0225EC20 *param0, u32 param1, enum HeapID heapID, u32 param3);

void ov49_0225AEA8(UnkStruct_ov70_0225EC20 *param0, u32 param1, enum HeapID heapID, u32 param3) {
    int v0;
    GF_ASSERT(param0->unk_34 == NULL);
    param0->unk_34 = ListMenuItems_New(param1, heapID);
    param0->unk_38 = param1;

    for (v0 = 0; v0 < param1; v0++) {
        param0->unk_34[v0].value = param3;
    }
}

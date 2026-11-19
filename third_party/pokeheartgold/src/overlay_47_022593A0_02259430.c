#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"

struct Menu;

typedef struct Menu Menu;

u32 Handle2dMenuInput_DeleteOnFinish(Menu *menu, u32 heapID);

u32 ov47_02259430(UnkStruct_ov68_0225D128 *param0, u32 param1);

u32 ov47_02259430(UnkStruct_ov68_0225D128 *param0, u32 param1) {
    u32 v0 = Handle2dMenuInput_DeleteOnFinish(param0->unk_20, param1);

    if (v0 != 0xffffffff) {
        param0->unk_20 = NULL;
    }

    return v0;
}

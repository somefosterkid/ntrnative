#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6698(MsgData *param_1, s32 param_2, s32 param_3);

void ov14_021F6768(MsgData *param_1, u32 param_2, s32 param_3) {
    if (param_2 == 0) {
        BufferItemName(*(MessageFormat **)(*(u8 **)&param_1[4].narc_id + 0x24), 0, 0x70);
        ov14_021F6698(param_1, 0x37, param_3);
        return;
    }
    BufferItemName(*(MessageFormat **)(*(u8 **)&param_1[4].narc_id + 0x24), 0, param_2);
    ov14_021F6698(param_1, 0x10, param_3);
    return;
}

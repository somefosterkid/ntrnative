#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6698(MsgData *param_1, s32 param_2, s32 param_3);
void ov14_021F68C0(MsgData *param_1, u32 param_2, s32 param_3);

void ov14_021F68C0(MsgData *param_1, u32 param_2, s32 param_3) {
    s32 unaff_r4;

    switch (param_2) {
    case 2:
        unaff_r4 = 0x1a;
        BufferItemName(*(MessageFormat **)(*(u8 **)&param_1[4].narc_id + 0x24), 0, (u32) * (u16 *)(*(u8 **)&param_1[4].narc_id + 0x88c8));
        break;
    case 3:
        unaff_r4 = 0x1b;
        BufferItemName(*(MessageFormat **)(*(u8 **)&param_1[4].narc_id + 0x24), 0, (u32) * (u16 *)(*(u8 **)&param_1[4].narc_id + 0x88c8));
        break;
    case 4:
        unaff_r4 = 0x18;
        break;
    case 5:
        unaff_r4 = 0x22;
        break;
    case 6:
        unaff_r4 = 0xe;
        break;
    case 7:
        unaff_r4 = 0x3b;
    }
    ov14_021F6698(param_1, unaff_r4, param_3);
    return;
}

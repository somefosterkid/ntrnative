#include "global.h"

#include "msgdata.h"
#include "unk_02005D10.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E7588(s32 *param_1, u32 param_2);
u32 ov14_021F01D8(void *param_1, u32 param_2);
void ov14_021F0BF4(void *param_1);
void ov14_021F3488(void *param_1, u32 param_2, s32 param_3);
void ov14_021F43F4(s32 param_1, s32 param_2);
void ov14_021F685C(MsgData *param_1, u32 param_2, u32 param_3, s32 param_4);

void ov14_021EB2EC(s32 *param_1) {
    u32 unaff_r5;

    PlaySE(0x60c);
    switch (*(u32 *)(*param_1 + 8)) {
    case 0:
        ov14_021F0BF4((u8 *)param_1);
        ov14_021F685C((MsgData *)param_1, 0, 0, 0x27);
        ov14_021F43F4(param_1[0xd], 0);
        ov14_021F3488(param_1, 1, 1);
        ov14_021E7588(param_1, 0x1e);
        unaff_r5 = 0x5b;
        break;
    case 1:
        ov14_021F685C((MsgData *)param_1, 0, 0, 0x27);
        ov14_021E7588(param_1, 0);
        unaff_r5 = 0x51;
        break;
    case 2:
        ov14_021E7588(param_1, 0);
        unaff_r5 = 0xc;
        break;
    case 3:
        ov14_021F3488(param_1, 0x81, 1);
        ov14_021E7588(param_1, 0);
        unaff_r5 = 0x75;
    }
    ov14_021F01D8((u8 *)param_1, unaff_r5);
    return;
}

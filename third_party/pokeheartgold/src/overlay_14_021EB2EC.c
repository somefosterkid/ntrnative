#include "global.h"

#include "msgdata.h"
#include "unk_02005D10.h"
#include "overlay_14.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7588(void *arg0, s32 arg1);
s32 ov14_021F01D8(UnkStruct_ov14_boxapp *arg0, s32 arg1);
void ov14_021F0BF4(void *param_1);
void ov14_021F3488(void *arg0, s32 arg1, s32 arg2);
void ov14_021F43F4(void *arg0, s32 arg1);
void ov14_021F685C(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3, u32 param_4);

void ov14_021EB2EC(UnkStruct_ov14_boxapp *param_1) {
    u32 unaff_r5;

    PlaySE(0x60c);
    switch (param_1->unk_0->unk8) {
    case 0:
        ov14_021F0BF4(param_1);
        ov14_021F685C(param_1, 0, 0, 0x27);
        ov14_021F43F4(param_1->sys, 0);
        ov14_021F3488(param_1, 1, 1);
        ov14_021E7588(param_1, 0x1e);
        unaff_r5 = 0x5b;
        break;
    case 1:
        ov14_021F685C(param_1, 0, 0, 0x27);
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
    ov14_021F01D8(param_1, unaff_r5);
    return;
}

#include "global.h"

#include "constants/heap.h"

#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6B10(UnkStruct_ov14_boxapp *arg0);
void ov14_021F6B28(UnkStruct_ov14_boxapp *arg0, s32 target);
void ov14_021F6A44(UnkStruct_ov14_boxapp *param_1);

extern void *ov14_021F8B10[][3];

void ov14_021F6A44(UnkStruct_ov14_boxapp *param_1) {
    GridInputHandler *pGVar1;
    s32 unaff_r5;
    switch ((u32)param_1->unk_0->unk8) {
    case 0:
        unaff_r5 = 0;
        break;

    case 1:
        unaff_r5 = 2;
        break;

    case 2:
        unaff_r5 = 3;
        break;

    case 3:
        unaff_r5 = 6;
    }

    ov14_021F6B10(param_1);
    pGVar1 = GridInputHandler_Create(ov14_021F8B10[unaff_r5][0], ov14_021F8B10[unaff_r5][1], ov14_021F8B10[unaff_r5][2], param_1, 1, (u8)param_1->unk2C, HEAP_ID_10);
    param_1->sys->unk_2C = pGVar1;
    ov14_021F6B28(param_1, param_1->unk2C);
    return;
}

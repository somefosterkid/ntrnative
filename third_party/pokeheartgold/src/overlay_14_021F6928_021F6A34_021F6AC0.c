#include "global.h"

#include "constants/heap.h"

#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6B10(void *arg0, s32, u32);
void ov14_021F6B28(void *, u32);
void ov14_021F6AC0(void *param_1, s32 param_2, u32 param_3);

extern void *ov14_021F8B10[][3];

void ov14_021F6AC0(void *param_1, s32 param_2, u32 param_3) {
    GridInputHandler *pGVar1;
    ov14_021F6B10(param_1, param_2, param_3);
    pGVar1 = GridInputHandler_Create(ov14_021F8B10[param_2][0], ov14_021F8B10[param_2][1], ov14_021F8B10[param_2][2], param_1, 1, (u8)param_3, HEAP_ID_10);
    ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2C = pGVar1;
    ov14_021F6B28(param_1, param_3);
    return;
}

#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6B28(UnkStruct_ov14_boxapp *, u32);

void ov14_021F7854(void *param_1, u32 param_2) {
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2C, param_2 & 0xff);
    if ((param_2 != 6) && (param_2 != 7)) {
        ov14_021F6B28(param_1, param_2);
    }
    return;
}

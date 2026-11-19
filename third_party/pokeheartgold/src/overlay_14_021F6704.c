#include "global.h"

#include "message_format.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6698(void *, s32, s32);
void ov14_021F6704(void *arg0, u32 arg1, s32 arg2);

void ov14_021F6704(void *arg0, u32 itemId, s32 arg2) {
    BufferItemName(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_24, 0U, itemId);
    ov14_021F6698(arg0, 0x17, arg2);
}

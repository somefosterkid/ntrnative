#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A60(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021F2A18(UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F4B90(UnkStruct_ov14_boxsys *arg0, s32 arg1, s16 x, s16 y, s32 arg4) {
    ManagedSprite_SetPositionXY(arg0->sprites[arg1], x, y);
    ov14_021F2A18(arg0, arg1, arg4);
    ov14_021F2A60(arg0, arg1, 0);
}

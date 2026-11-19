#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(void *, s32, s32, s32);
s32 ov14_021F3190(void *, u32, s32);
void ov14_021EA1F0(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov14_021EA1F0(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 y[2];
    UnkStruct_ov14_boxsys *v0;

    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0xA, arg3);
    ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[9], &y[1], &y[0]);
    v0 = ((UnkStruct_ov14_boxapp *)arg0)->sys;
    ManagedSprite_SetPositionXY(v0->sprites[v0->unk_4094[((UnkStruct_ov14_boxapp *)arg0)->unk21]], y[1], (s16)(y[0] + 4));
    ov14_021F3190(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), ((UnkStruct_ov14_boxapp *)arg0)->unk21, 0);
}

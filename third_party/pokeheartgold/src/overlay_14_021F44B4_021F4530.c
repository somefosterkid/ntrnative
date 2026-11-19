#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E7930(void *, u32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F4530(struct UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);

void ov14_021F4530(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    s16 y[2];
    u32 v0;
    void *v1;
    v0 = ov14_021E7930(arg0, arg0->unk25, arg2);
    v1 = *((void * *)&arg0->sys);
    if ((((unsigned int)((u8)(*((u8 *)&((UnkStruct_ov14_boxsys *)v1)->unk_44D)))) >> 2) == (v0 >> 2)) {
        ov14_021F2A18(v1, 6, 1);
    } else {
        ov14_021F2A18(v1, 6, 0);
    }
    ManagedSprite_GetPositionXY(arg0->sys->sprites[6], &y[1], &y[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[6], (s16)((0x2E * (v0 & 3)) + 0x3B), y[0]);
}

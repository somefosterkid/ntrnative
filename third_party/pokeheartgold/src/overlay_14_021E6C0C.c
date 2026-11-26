#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E6C0C(UnkStruct_ov14_boxapp *arg0, MoveEntry *arg1) {
    s16 y[2];
    UnkStruct_ov14_boxsys *v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v5;
    u8 v4;

    v0 = arg0->sys;
    v4 = v0->unk_4094[arg1->arg];
    ManagedSprite_GetPositionXY(v0->sprites[v0->unk_4094[arg1->idx]], &arg1->x0, &arg1->y0);
    ManagedSprite_GetPositionXY(arg0->sys->sprites[v4], &y[1], &y[0]);
    if ((s32)arg1->x0 <= (s32)y[1]) {
        arg1->dx = 1;
        v5 = y[1] - arg1->x0;
    } else {
        arg1->dx = -1;
        v5 = arg1->x0 - y[1];
    }
    v1 = v5 << 0x10;
    arg1->mulX = (s32)((s32)(v1 + ((u32)(v1 >> 2) >> 0x1D)) >> 3);
    if ((s32)arg1->y0 <= (s32)y[0]) {
        arg1->dy = 1;
        v2 = (y[0] - arg1->y0) << 0x10;
        arg1->mulY = (s32)((s32)(v2 + ((u32)(v2 >> 2) >> 0x1D)) >> 3);
        return;
    }
    arg1->dy = -1;
    v3 = (arg1->y0 - y[0]) << 0x10;
    arg1->mulY = (s32)((s32)(v3 + ((u32)(v3 >> 2) >> 0x1D)) >> 3);
}

void ov14_021E6CC8(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    MoveSet *v0;

    v0 = arg0->sys->unk_C;
    v0->e[0].idx = arg1;
    v0->e[0].arg = arg2;
    v0->e[0].active = 1;
    ov14_021E6B48(arg0, &v0->e[0]);
    v0->e[1].idx = arg2;
    v0->e[1].arg = arg1;
    v0->e[1].active = 1;
    ov14_021E6B48(arg0, &v0->e[1]);
}

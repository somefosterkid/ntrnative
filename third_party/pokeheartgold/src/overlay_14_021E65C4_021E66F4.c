#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2F88(s32, void *, s32 *, u32);
s32 ov14_021F3190(void *, s32, s32);
s32 ov14_021E66F4(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021E66F4(UnkStruct_ov14_boxapp *arg0) {
    s16 pos[2];
    u32 i;
    u8 slot;
    MoveSet *set = arg0->sys->unk_C;
    MoveEntry *e;

    if (set->frame == 8) {
        e = set->e;
        for (i = 0; i < 7; i++, e++) {
            if (e->active != 0) {
                slot = arg0->sys->unk_4094[e->idx];
                ov14_021F2F88(e->arg, &pos[1], (s32 *)pos, arg0->unk22);
                if (arg0->unk21 != e->arg) {
                    pos[0] += 0x90;
                }
                ManagedSprite_SetPositionXY(arg0->sys->sprites[slot], pos[1], pos[0]);
            }
        }
        return 0;
    }
    set->frame++;
    i = 0;
    e = set->e;
    for (; i < 7; i++, e++) {
        if (e->active != 0 && arg0->unk21 == e->arg) {
            slot = arg0->sys->unk_4094[set->e[i].idx];
            pos[1] = set->e[i].x0 + set->e[i].dx * ((set->e[i].mulX * set->frame) >> 16);
            pos[0] = set->e[i].y0 + set->e[i].dy * ((set->e[i].mulY * set->frame) >> 16);
            ManagedSprite_SetPositionXY(arg0->sys->sprites[slot], pos[1], pos[0]);
            ov14_021F3190(arg0->sys, set->e[i].idx, 0);
            break;
        }
    }
    return 1;
}

s32 ov14_021E6814(UnkStruct_ov14_boxapp *arg0) {
    s16 pos[2];
    int i;
    u8 slot;
    MoveSet *set = ((MoveSet *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_C);
    MoveEntry *e;

    if (set->frame == 8) {
        e = set->e;
        for (i = 0; (u32)i < 7; i++, e++) {
            if (e->active != 0) {
                slot = arg0->sys->unk_4094[e->idx];
                ov14_021F2F88(e->arg, &pos[1], (s32 *)pos, arg0->unk22);
                ManagedSprite_SetPositionXY(arg0->sys->sprites[slot], pos[1], pos[0]);
            }
        }
        return 0;
    }
    e = set->e;
    set->frame++;
    for (i = 0; (u32)i < 7; i++, e++) {
        if (e->active != 0) {
            slot = arg0->sys->unk_4094[e->idx];
            pos[1] = e->x0 + e->dx * ((set->frame * e->mulX) >> 16);
            pos[0] = e->y0 + e->dy * ((set->frame * e->mulY) >> 16);
            ManagedSprite_SetPositionXY(arg0->sys->sprites[slot], pos[1], pos[0]);
            ov14_021F31E0(arg0->sys, e->idx, e->arg);
        }
    }
    return 1;
}

#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "party.h"
#include "pokemon.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2ED0(void *, u32, u32, s32);

void ov14_021F2F20(struct UnkStruct_ov14_boxapp *arg0);
void ov14_021F2F88(u32 pos, s16 *x, s16 *y, u32 mode);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F2F3C(struct UnkStruct_ov14_boxapp *arg0);

typedef struct {
    s16 x;
    s16 y;
} ClActorPos;
extern const ClActorPos ov14_021F80BC[6];

void ov14_021F2F20(UnkStruct_ov14_boxapp *arg0) {
    u32 v0;

    v0 = 0;
    do {
        ov14_021F2ED0(arg0, arg0->unk1F, v0, v0 + 0x19);
        v0 += 1;
    } while (v0 < 0x1EU);
}

void ov14_021F2F3C(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    UnkStruct_ov14_boxsys *v1;
    s32 slot;

    v0 = Party_GetCount(arg0->party);
    slot = 0;
    if (v0 != 0) {
        do {
            ov14_021F2EA0(arg0, Mon_GetBoxMon(Party_GetMonByIndex(arg0->party, slot)), arg0->sys->unk_4094[0x1E + slot]);
            v1 = arg0->sys;
            ov14_021F2A18(v1, v1->unk_4094[0x1E + slot], 0);
            slot += 1;
        } while ((u32)slot < (u32)v0);
    }
}

void ov14_021F2F88(u32 pos, s16 *x, s16 *y, u32 mode) {
    if (pos < 30) {
        *x = (pos % 6 + 1) * 24;
        *y = pos / 6 * 24 + 48;
    } else {
        pos -= 30;
        *x = ov14_021F80BC[pos].x;
        *y = ov14_021F80BC[pos].y - 144;
        if (mode == 2) {
            *x += 152;
        }
    }
}

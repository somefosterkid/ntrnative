#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094A90(UnkStruct_020932E0 *p, s32 mode) {
    u32 seq;
    VecFx32 v;
    u8 idx;

    if (mode != 0) {
        if (p->unk_469C == 2) {
            if (mode == 2) {
                v = p->unk_4688;
                seq = (u8)p->unk_4684;
            } else {
                idx = p->unk_46B0;
                v.x = 0xd4000;
                v.y = (idx + 1) * 0x28 << 12;
                v.z = 0;
                seq = (u8)p->unk_46B0;
            }
        } else {
            idx = p->unk_46B0;
            v.x = 0xd4000;
            v.y = (idx + 1) * 0x28 << 12;
            v.z = 0;
            seq = (u8)p->unk_46B0;
        }
        sub_02095D40(p->unk_46B8, 3, seq);
    } else {
        v = p->unk_4688;
        if (p->unk_469C == 2) {
            sub_02095D40(p->unk_46B8, 3, (u8)p->unk_4684);
        } else {
            sub_02095D40(p->unk_46B8, 2, (u8)p->unk_4684);
        }
    }
    Sprite_SetMatrix(p->unk_46A0[p->unk_4684].unk_0, &p->unk_4688);
    Sprite_SetDrawPriority(p->unk_46A0[p->unk_4684].unk_0, 6);
    Sprite_SetMatrix(p->unk_8C0, &v);
    Sprite_SetAnimCtrlSeq(p->unk_8C0, 0x2d);
    if (p->unk_46BC != 0) {
        p->unk_46BC = 0;
    }
    p->unk_4680 = 0;
    p->unk_4684 = -1;
    p->unk_4688.x = 0;
    p->unk_4688.y = 0;
    p->unk_4688.z = 0;
    p->unk_4694 = 0;
    p->unk_4698 = 0;
    p->unk_46A0 = NULL;
    p->unk_469C = 0;
    Sprite_SetDrawFlag(p->unk_7B0.unk_0, 0);
}

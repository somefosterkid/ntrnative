#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_rower_work.h"

void ov96_021FFE5C(Ov96RowerWork *work, s32 idx, s32 param_3);

void ov96_021FFE5C(Ov96RowerWork *work, s32 idx, s32 param_3) {
    u8 cVar1;
    u8 *states;
    u8 bVar4;

    states = work->unk644;
    bVar4 = 0;
    cVar1 = work->rowers[idx].subs[work->rowers[idx].subIdx].unk18;
    if (work->rowers[idx].unk9D != 0) {
        bVar4 = 3;
    } else if (work->rowers[idx].unkA9 != 0) {
        bVar4 = 3;
    } else if (cVar1 == 2) {
        bVar4 = 3;
    } else if (cVar1 == 1) {
        bVar4 = 1;
    }
    if (param_3 == 0) {
        states[idx] = bVar4;
        return;
    }
    if (work->counts[idx].unk0 != 0) {
        if (bVar4 <= 1) {
            states[idx] = 2;
            return;
        }
        if (bVar4 == 3) {
            states[idx] = 4;
            return;
        }
        GF_AssertFail();
    } else if (work->counts[idx].unk4 == 0) {
        states[idx] = bVar4;
    }
}

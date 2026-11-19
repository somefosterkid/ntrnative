#include "global.h"

#include "sprite_system.h"

void ov112_021F22B0(ManagedSprite *arg0, s32 arg1, s32 arg2);

void ov112_021F22B0(ManagedSprite *managedSprite, s32 arg1, s32 arg2) {
    s32 seqno;

    seqno = arg2 + 0xF;
    if (arg1 == 1) {
        seqno += 4;
    }
    ManagedSprite_SetDrawFlag(managedSprite, 1);
    ManagedSprite_SetAnim(managedSprite, seqno);
}

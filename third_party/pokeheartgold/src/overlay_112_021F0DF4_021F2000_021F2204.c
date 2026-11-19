#include "global.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F0F48(void *, s32);

void ov112_021F1F3C(Ov112SpriteRec *, s32);

void ov112_021F1F54(Ov112SpriteRec *, s16, s16, s32);

s32 ov112_021F2098(void *, u16, u32, u32, s32, s32);

void ov112_021F2204(Ov112SpriteApp *arg0, s32 arg1, s32 arg2);

void ov112_021F2204(Ov112SpriteApp *arg0, s32 arg1, s32 arg2) {
    Ov112Record *unk;

    unk = arg0->records[arg0->curIdx];
    if (arg1 == 0) {
        ov112_021F1F54(&arg0->spriteRecs[0], 0x68, 0x48, 0);
        ov112_021F1F3C(&arg0->spriteRecs[0], 3);
    } else {
        ov112_021F1F54(&arg0->spriteRecs[1], 0x98, 0x48, 0);
        ov112_021F1F3C(&arg0->spriteRecs[1], 1);
    }
    if (arg2 != 0) {
        ov112_021F2098(&arg0->spriteRecs[arg1], unk->speciesA, unk->bitsA.lo, unk->bitsA.mid, ov112_021F0F48(unk, 1), arg0->heapID);
    } else {
        ov112_021F2098(&arg0->spriteRecs[arg1], unk->speciesB, unk->bitsB.lo, unk->bitsB.mid, ov112_021F0F48(unk, 0), arg0->heapID);
    }
}

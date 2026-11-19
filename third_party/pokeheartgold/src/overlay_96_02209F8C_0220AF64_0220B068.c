#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"

s32 ov96_0220B148(void *, void *, void *);

void ov96_0220B068(UnkStruct_ov96_0220B148 *arg0);

void ov96_0220B068(UnkStruct_ov96_0220B148 *arg0) {
    s16 sp0[2];
    if (arg0 == 0L) {
        GF_AssertFail();
    }
    ov96_0220B148(arg0, &sp0[1], sp0);
    sp0[1] = (s16)(sp0[1] + arg0->unk_A);
    sp0[0] = (s16)(sp0[0] + arg0->unk_C);
    ManagedSprite_SetPositionXY(arg0->unk_4, sp0[1], sp0[0]);
}

#include "global.h"

#include "overlay_96_ov96_rower_work.h"

s32 ov96_021EAB38(void *, s32, s32, void *);

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

void *ov96_021EB594(void *);

s32 ov96_021FFE38(s32, s32, s32);

void ov96_021FEE60(Ov96Rower *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5);

void ov96_021FEE60(Ov96Rower *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5) {

    ov96_021EAB38(arg0->unk00[arg2], 1, arg2, arg3);
    arg0->unkC8 = 0;
    ov96_021EB52C(arg0->unk70, 1, 1);
    ov96_021EB564(arg0->unk70, 8);
    ov96_021EB52C(arg0->unk74, 1, 0);
    if (arg4 != 0) {
        *(s8 *)((u8 *)arg3 + 8) = 0;
    }
    VecFx32 sp0 = *(VecFx32 *)ov96_021EB594(arg0->unk70);
    ov96_021FFE38((sp0.x / 0x1000), 0x8B6, arg5);
}

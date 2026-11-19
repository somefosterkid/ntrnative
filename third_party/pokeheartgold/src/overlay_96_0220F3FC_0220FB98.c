#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "math_util.h"

void ov96_0220FB98(UnkStruct_ov96_0220E3F0_Record *arg0, s8 arg1, s8 arg2);

void ov96_0220FB98(UnkStruct_ov96_0220E3F0_Record *arg0, s8 arg1, s8 arg2) {
    arg0->unk_4 = (s32)(0x20 | arg0->unk_4);
    arg0->unk_0 = 0x80;
    arg0->unk_2 = 0;
    arg0->unk_0 = 0;
    arg0->unk_4 = (s32)(4 | (arg0->unk_4 & ~0x1C));
    arg0->unk_8 = arg2;
    arg0->unk_9 = (s8)(MTRandom() % 15U);
    arg0->unk_4 = (s32)(0xFFFFC03F & arg0->unk_4);
    arg0->unk_A = arg1;
}

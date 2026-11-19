#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "unk_struct_ov96_0220b148.h"

extern u8 ov96_0221CDC8;

extern u8 ov96_0221CDCA;

void ov96_0220AFF8(UnkStruct_ov96_0220B148 *arg0);

void ov96_0220AFF8(UnkStruct_ov96_0220B148 *arg0) {
    u8 var_r0;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    var_r0 = (MTRandom() % 3U);
    if ((u32)var_r0 >= ((u32)(arg0->unk_10 << 0x1C) >> 0x1C)) {
        var_r0 += 1;
    }
    arg0->unk_10 = (s32)((((s32 *)arg0)[4] & ~0xF) | (var_r0 & 0xF));
    arg0->unk_A = (*(s16 *)(&ov96_0221CDC8 + ((u32)(arg0->unk_10 << 0x1C) >> 0x1A)) + (MTRandom() & 0xF));
    arg0->unk_C = (*(s16 *)(&ov96_0221CDCA + ((u32)(arg0->unk_10 << 0x1C) >> 0x1A)) + (MTRandom() % 12U));
}

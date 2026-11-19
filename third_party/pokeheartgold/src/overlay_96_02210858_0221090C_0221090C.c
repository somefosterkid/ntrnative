#include "global.h"

#include "math_util.h"

extern u8 ov96_0221D1B0[][2];

extern s16 ov96_0221D1C4[][3][2];

typedef struct {
    u8 pad0[6];
    s16 unk6;
    s16 unk8;
    u8 padA[2];
    u32 unkC_0 : 1;
    u32 unkC_1 : 2;
    u32 unkC_3 : 2;
    u32 unkC_5 : 2;
    u32 unkC_7 : 4;
    u32 unkC_11 : 21;
} UnkStruct_ov96_0221090C;

void ov96_0221090C(UnkStruct_ov96_0221090C *arg0);

void ov96_0221090C(UnkStruct_ov96_0221090C *arg0) {
    u32 idx = arg0->unkC_5;
    arg0->unk6 = ov96_0221D1B0[idx][0] + MTRandom() % ov96_0221D1B0[idx][1];
    arg0->unk6 *= 8;
    arg0->unk8 = ov96_0221D1C4[arg0->unkC_3][arg0->unkC_5][0];
    arg0->unk8 += ((MTRandom() % 5) - 2) * 8;
    arg0->unkC_0 = 0;
    arg0->unkC_7 = 0;
}

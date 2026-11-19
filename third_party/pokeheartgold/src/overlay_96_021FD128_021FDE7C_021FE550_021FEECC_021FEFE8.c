#include "global.h"
#include "pokeathlon/pokeathlon.h"

typedef struct Ov96Elem {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s8 unk14;
    s8 unk15;
    s8 unk16;
    s8 unk17;
    u8 pad18[4];
} Ov96Elem;

typedef struct UnkStruct_ov96_021FEFE8 {
    u8 pad0[0x18];
    Ov96Elem elems[1];
} UnkStruct_ov96_021FEFE8;

void ov96_021FEFE8(PokeathlonCourseData *arg0, s32 (*arg1)[5], s32 arg2, u8 arg3, UnkStruct_ov96_021FEFE8 *arg4);

void ov96_021FEFE8(PokeathlonCourseData *arg0, s32 (*arg1)[5], s32 arg2, u8 arg3, UnkStruct_ov96_021FEFE8 *arg4) {
    PokeathlonMonKinds *idx = ov96_021E60D8(arg0, arg2, arg3);

    arg4->elems[arg3].unk8 = (f32)arg1[0][idx->statIdx[0]] / 10.0;
    arg4->elems[arg3].unk10 = arg1[1][idx->statIdx[3]];
    arg4->elems[arg3].unkC = arg1[1][idx->statIdx[3]];
    arg4->elems[arg3].unk16 = arg1[5][idx->statIdx[1]];
    arg4->elems[arg3].unk14 = arg1[7][idx->statIdx[1]];
    arg4->elems[arg3].unk15 = arg1[6][idx->statIdx[1]];
    arg4->elems[arg3].unk0 = (f32)arg1[2][idx->statIdx[4]] / 10.0;
    arg4->elems[arg3].unk4 = (f32)arg1[3][idx->statIdx[1]] / 10.0;
    arg4->elems[arg3].unk17 = arg1[4][idx->statIdx[3]];
}

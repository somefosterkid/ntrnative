#include "global.h"

#include "pokeathlon/pokeathlon.h"

typedef struct Ov96_0220CF50_Src {
    u8 filler0[4];
    s32 objs[4];
    u32 lo : 8;
    u32 pad0 : 8;
    u32 mid : 8;
    u32 x : 6;
    u32 idx : 2;
    u32 w18;
    u32 pad1 : 9;
    u32 bit : 1;
} Ov96_0220CF50_Src;

typedef struct Ov96_0220CF50_Dst {
    u32 a : 8;
    u32 b : 8;
    u32 c : 8;
    u32 d : 2;
    u32 e : 1;
    u32 f : 1;
    u32 g : 1;
} Ov96_0220CF50_Dst;

void ov96_0220CF50(Ov96_0220CF50_Src *src, s32 arg1, PokeathlonCourseData *arg2);

void ov96_0220CF50(Ov96_0220CF50_Src *src, s32 arg1, PokeathlonCourseData *arg2) {
    Ov96_0220CF50_Dst *dst;
    u32 idx;
    s32 *objp;

    dst = (Ov96_0220CF50_Dst *)ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(arg2) + 0x50 + 0x28 * (src->w18 >> 30));
    objp = &src->objs[src->idx];
    idx = src->idx;
    dst->a = src->lo;
    dst->b = (u32)(*objp << 6) >> 24;
    dst->c = src->mid;
    dst->e = src->bit;
    dst->f = arg1 & 2;
    dst->d = src->idx;
    dst->g = src->x == 1;
}

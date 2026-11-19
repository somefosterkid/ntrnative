/* PORTOK hk39 */
#include "error_handling.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
u8 ov96_021ED954(PokeathlonCourseData *, u8);
s32 ov96_021ED9CC(PokeathlonCourseData *);
s32 ov96_021EDCEC(PokeathlonCourseData *, s32, u8, u8);

/* invented: bitfield layout of the ov96_021E8A20 result, read from the asm masks */
typedef struct Ov96Unk021EDA58 {
    u8 b[4];
    u32 f0 : 3;
    u32 f3 : 12;
    u32 f15 : 12;
    u32 f27 : 5;
    u32 g0 : 16;
    u32 g16 : 12;
    u32 g28 : 4;
    u8 pad;
    u8 order[4];
} Ov96Unk021EDA58;

void ov96_021EDA58(PokeathlonCourseData *arg0) {
    Ov96Unk021EDA58 *d;
    PokeathlonFieldData *p2;
    PokeathlonFieldData *p1;
    s32 max1;
    u32 mode;
    s32 i;
    s32 j;
    u32 max2;
    s32 v;
    u32 t;
    s32 k;
    u8 idx;
    s32 i2;

    p1 = PokeathlonCourse_GetFieldData(arg0);
    d = (Ov96Unk021EDA58 *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0x28);
    d->f15 = 0;
    d->f3 = 0;
    d->g16 = 0;
    for (i = 0; i < 4; i++) {
        d->b[i] = ov96_021ED954(arg0, i);
    }
    d->f0 = ov96_021ED9CC(arg0);
    max1 = 0;
    max2 = 0;
    mode = d->f0;
    p2 = p1;
    for (i = 0; i < 4; i++) {
        PokeathlonFieldData *q = p2;
        for (j = 0; j < 3; j++) {
            if (mode != 4) {
                v = ov96_021EDCEC(arg0, (u8)mode, i, j);
                if (max1 < v) {
                    max1 = v;
                }
            }
            t = *(u32 *)((u8 *)q + 8);
            if (max2 < t) {
                max2 = t;
            }
            q = (PokeathlonFieldData *)((u8 *)q + 0x20);
        }
        p2 = (PokeathlonFieldData *)((u8 *)p2 + 0x60);
    }
    k = 0;
    for (i2 = 0; i2 < 4; i2++) {
        PokeathlonFieldData *q = p1;
        for (j = 0; j < 3; j++) {
            idx = j + k;
            if (*(s32 *)q == 0 && *(s32 *)((u8 *)q + 0x10) == 0 && *(s32 *)((u8 *)q + 0x1C) == 0) {
                d->f15 += 1 << idx;
            }
            if (max2 == *(u32 *)((u8 *)q + 8)) {
                d->g16 += 1 << idx;
            }
            if (mode != 4) {
                v = ov96_021EDCEC(arg0, (u8)mode, (u8)i2, j);
                if (v > max1) {
                    GF_AssertFail();
                }
                if (max1 == v) {
                    d->f3 += 1 << idx;
                }
            }
            q = (PokeathlonFieldData *)((u8 *)q + 0x20);
        }
        k += 3;
        p1 = (PokeathlonFieldData *)((u8 *)p1 + 0x60);
    }
    d->g0 = max1;
    j = MTRandom() & 3;
    for (i = 0; i < 4; i++) {
        d->order[j] = i;
        j = (j + 1) % 4;
    }
}

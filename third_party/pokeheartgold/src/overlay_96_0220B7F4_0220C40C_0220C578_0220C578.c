#include "global.h"

#include "pokeathlon/pokeathlon.h"

typedef struct {
    u8 pad0[0xc];
    PokeathlonCourseData *course;
    u8 pad1[0x34 - 0x10];
    u32 arr[3];
    u32 idx : 2;
    u32 val : 8;
    u32 x : 11;
    u32 r40 : 11;
    u32 v : 8;
    u32 flag : 1;
    u32 y : 1;
    u32 z : 6;
    u32 w : 8;
    u32 t : 8;
} UnkStruct_ov96_0220C578;

void ov96_021E8228(PokeathlonCourseData *param_1, u32 param_2, u8 param_3, s32 param_4, u32 param_5);

void ov96_0220C7FC(UnkStruct_ov96_0220C578 *param_1);

void ov96_0220C578(UnkStruct_ov96_0220C578 *p, u32 param_2);

void ov96_0220C578(UnkStruct_ov96_0220C578 *p, u32 param_2) {
    s32 v;
    switch (param_2) {
    case 2:
        ov96_021E8228(p->course, ov96_021E5F24(p->course) & 0xff, p->idx, 1, 1);
        p->arr[p->idx] &= 0xFC03FFFF;
        p->x = 0;
        break;
    case 3:
        ov96_021E8228(p->course, ov96_021E5F24(p->course) & 0xff, p->idx, 7, 1);
        p->t = 0;
        v = p->v - 10;
        if (v > 100) {
            v = 100;
        } else if (v < 0) {
            v = 0;
        }
        p->v = v;
        if (p->flag != 0) {
            ov96_0220C7FC(p);
        }
        p->idx = p->idx + 1;
        if (p->idx >= 3) {
            p->idx = 0;
        }
        break;
    }
    p->val = param_2;
    p->z = 0;
    p->w = 0;
}

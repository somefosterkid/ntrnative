#include "global.h"

#include "pokeathlon/pokeathlon.h"

typedef struct Ov96Pos16 {
    s16 x;
    s16 y;
} Ov96Pos16;

typedef struct Ov96A314Obj {
    PokeathlonCourseData *data;
    u8 state;
    u8 unk5;
    u8 unk6;
    s8 target;
    u32 idx : 2;
    u32 unk8_2 : 2;
    u32 unk8_4 : 1;
    u32 unk8_5 : 1;
    u32 unk8_6 : 26;
} Ov96A314Obj;

void ov96_02215DEC(PokeathlonCourseData *data, u8 idx, VecFx32 *out);

s32 ov96_02215F64(PokeathlonCourseData *data, u8 idx);

void ov96_02215F80(PokeathlonCourseData *data, u8 idx, const Ov96Pos16 *pos);

BOOL ov96_0221A034(VecFx32 *a, VecFx32 *b, fx32 dist);

extern const Ov96Pos16 ov96_0221D98C[];

void ov96_0221A314(Ov96A314Obj *obj);

void ov96_0221A314(Ov96A314Obj *obj) {
    const Ov96Pos16 *pos = &ov96_0221D98C[obj->idx];
    VecFx32 cur = { 0, 0, 0 };
    VecFx32 target = { 0, 0, 0 };

    ov96_02215DEC(obj->data, obj->idx, &cur);
    target.x = ov96_0221D98C[obj->idx].x << 12;
    target.y = ov96_0221D98C[obj->idx].y << 12;
    ov96_02215F80(obj->data, obj->idx, pos);
    if (ov96_0221A034(&cur, &target, 0x40000) != 0) {
        ov96_02215F64(obj->data, obj->idx);
        obj->unk8_4 = 0;
    }
}

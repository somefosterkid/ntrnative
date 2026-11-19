#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E8228(s32, u8, u8, s32, s32);

void ov96_02218578(void *, s32);

typedef struct UnkStruct_ov96_02219030 UnkStruct_ov96_02219030;

struct UnkStruct_ov96_02219030 {
    s32 w0;
    void *info;
    u8 pad8[8];
    UnkStruct_ov96_02219030 *link;
    s32 kind;
    s32 last;
    u8 pad1c[0x28];
    VecFx32 vec;
    u8 pad50[8];
    u8 b58;
    u8 pad59[2];
    u8 b5B;
    u8 pad5C;
    u8 b5D;
    u8 pad5E[2];
    u32 cnt : 12;
    u32 a : 2;
    u32 mode : 2;
    u32 pad16 : 4;
    u32 d : 2;
    u32 e : 2;
    u32 pad24 : 3;
    u32 f27 : 1;
    u32 pad28 : 3;
    u32 f31 : 1;
    s32 w64;
};

void ov96_02219030(UnkStruct_ov96_02219030 *p);

void ov96_02219030(UnkStruct_ov96_02219030 *p) {
    UnkStruct_ov96_02219030 *o;
    s32 n;

    p->b5B = 60;
    p->b58 = 0;
    o = p->link;
    if (o == NULL) {
        n = p->cnt - 10;
        if (n < 0) {
            n = 0;
        }
        p->cnt = n;
        ov96_021E8228(p->w0, p->d, p->e, 8, 1);
    } else {
        n = o->cnt + 10;
        if (n > 200) {
            n = 200;
        }
        o->cnt = n;
        ov96_021E8228(o->w0, o->d, o->e, 3, 1);
        p->link = NULL;
    }
    ov96_021E8228(p->w0, p->d, p->e, 5, 1);
    ov96_021E8228(p->w0, p->d, p->e, 1, 1);
    ov96_02218578(p, 3);
}

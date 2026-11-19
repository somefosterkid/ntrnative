#include "global.h"

#include "error_handling.h"
#include "math_util.h"

float _d2f(void);

u64 _dadd(s32, s32, s32, s32);

u64 _ddiv(s32, s32, u32, u32);

void _dgr(s32, s32, u32, u32);

void _dls(s32, s32, u32, u32);

u64 _f2d();

fx32 _ffix(void);

void _fflt(s32);

void _fmul(float, float);

s16 _s32_div_f(s32, u32);

void ov96_02204320(s32 param_1, VecFx32 *param_2);

s32 ov96_0220472C(s32 param_1, void *param_2, void *param_3);

typedef struct {
    u8 pad0[0x10];
    u32 f10;
    u16 f14;
    s16 f16;
    u8 pad18;
    u8 f19;
} UnkStruct_ov96_022044A0_sub1;

typedef struct {
    u8 pad0[0xc];
    s32 state;
    VecFx32 pos;
    VecFx32 vel;
    u8 pad28[0x44 - 0x28];
    u16 cnt;
    u16 lvl;
} UnkStruct_ov96_022044A0_sub2;

typedef struct {
    UnkStruct_ov96_022044A0_sub1 *a;
    u16 idx;
    u16 val;
} UnkStruct_ov96_022044A0_sub3;

typedef struct {
    UnkStruct_ov96_022044A0_sub1 *a[12];
    UnkStruct_ov96_022044A0_sub2 *b[12];
    UnkStruct_ov96_022044A0_sub3 c[12];
    u32 unk_C0;
    u8 unk_C4;
    u8 unk_C5;
    u8 unk_C6;
    u8 unk_C7;
    u8 unk_C8;
    u8 unk_C9;
    u8 unk_CA;
    u8 pad_CB[1];
} UnkStruct_ov96_022044A0;

extern VecFx32 ov96_0221CA30[];

void ov96_022044A0(UnkStruct_ov96_022044A0 *p, u8 param_2, u8 param_3, s32 param_4);

void ov96_022044A0(UnkStruct_ov96_022044A0 *p, u8 param_2, u8 param_3, s32 param_4) {
    s32 i;
    s32 n;
    u32 r;
    float f;
    UnkStruct_ov96_022044A0_sub3 *pc[12];
    UnkStruct_ov96_022044A0_sub3 *res[9];
    VecFx32 dir;
    s32 q;

    if (LCRandom() % 51 + p->a[param_3]->f16 <= 120 && p->b[param_3]->cnt < 4) {
        if (p->b[param_3]->state == 0) {
            p->b[param_3]->cnt = 1;
            p->b[param_3]->state = 2;
            p->b[param_3]->pos = ov96_0221CA30[param_3];
            return;
        }
        if (p->b[param_3]->state != 2) {
            GF_AssertFail();
        }
        p->b[param_3]->cnt++;
        if (p->b[param_3]->cnt <= 1) {
            p->b[param_3]->lvl = 0;
        } else if (p->b[param_3]->cnt <= 2) {
            p->b[param_3]->lvl = 1;
        } else if (p->b[param_3]->cnt <= 3) {
            p->b[param_3]->lvl = 2;
        } else {
            p->b[param_3]->lvl = 3;
        }
        return;
    }
    for (i = 0; i < 12; i++) {
        p->c[i].a = p->a[i];
        p->c[i].idx = i;
        r = LCRandom();
        q = p->a[i]->f14 / 20;
        p->c[i].val = q + (s32)r % 21;
        if (p->a[i]->f19 == 1) {
            p->c[i].val++;
        }
        p->c[i].val += (120 - p->a[i]->f16) / 20;
    }
    for (i = 0; i < 12; i++) {
        pc[i] = &p->c[i];
    }
    n = ov96_0220472C(param_2, pc, res);
    {
        u8 sel = LCRandom() % n;
        VecFx32 zero = { 0, 0, 0 };
        VecFx32 *src = &ov96_0221CA30[param_3];

        VEC_Subtract(&ov96_0221CA30[(u8)res[sel]->idx], src, &dir);
        VEC_Normalize(&dir, &dir);
        ov96_02204320(3, &dir);
        ov96_02204320((u8)p->a[param_3]->f10, &dir);
        f = (float)param_4 / 40.0 + ((float)p->a[param_3]->f16 / 50.0 + (float)(LCRandom() % 51));
        if ((double)f > 2.0) {
            f = 2.0f;
        } else if ((double)f < 1.0) {
            f = 1.0f;
        }
        VEC_MultAdd((fx32)(4096.0f * f), &dir, &zero, &p->b[param_3]->vel);
        p->b[param_3]->state = 1;
        p->b[param_3]->pos = *src;
    }
}

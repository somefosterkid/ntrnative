#include "global.h"

#include "math_util.h"

typedef struct {
    s32 f0;
    u8 pad[0x10];
    u32 a0 : 8;
    u32 cnt : 8;
    u32 a1 : 14;
    u32 ahi : 2;
    u32 q0 : 22;
    u32 lim : 8;
    u32 bhi : 2;
    u32 n : 8;
    u32 flag : 1;
} UnkStruct_ov96_0220CAC4;

s32 ov96_0220CCBC(void *);

s32 ov96_0220CD00(void *);

s32 ov96_021E8228(s32, u8, u8, s32, s32);

s32 ov96_0220CC38(void *, u32);

s32 ov96_0220CD84(void *, s32);

s32 ov96_0220CE04(void *, s32);

s32 ov96_0220D0F8(void *);

s32 ov96_0220CAC4(UnkStruct_ov96_0220CAC4 *arg0);

s32 ov96_0220CAC4(UnkStruct_ov96_0220CAC4 *arg0) {
    s32 var_r4;

    var_r4 = 0;
    arg0->cnt++;
    if (arg0->cnt >= arg0->lim) {
        if (ov96_0220CD00((u8 *)arg0) != 0) {
            ov96_0220D0F8(arg0);
            ov96_021E8228(arg0->f0, arg0->bhi, arg0->ahi, 7, 1);
            ov96_0220CD84(arg0, 1);
        } else {
            if (ov96_0220CCBC(arg0) != 0) {
                var_r4 += 2;
            }
            if (arg0->flag != 0) {
                if (arg0->n >= 2 && ov96_0220CC38(arg0, arg0->flag) != 0) {
                    ov96_0220CE04(arg0, 2 & var_r4);
                }
            } else {
                ov96_0220CE04(arg0, 2 & var_r4);
            }
        }
        arg0->cnt = 0;
        arg0->lim = MTRandom() % 3 + 4;
    }
    if (arg0->flag != 0) {
        arg0->n++;
        if (arg0->n >= 10) {
            arg0->n = 0;
            arg0->flag = 0;
        }
    }
    return var_r4;
}

typedef struct {
    u8 unk0[0x14];
    u32 unk14_0 : 8;
    u32 unk14_8 : 8;
    u32 unk14_16 : 16;
} UnkStruct_0220CBC0;

s32 ov96_0220CBC0(UnkStruct_0220CBC0 *arg0);

s32 ov96_0220CBC0(UnkStruct_0220CBC0 *arg0) {
    arg0->unk14_8++;
    if (arg0->unk14_8 >= 0x1A) {
        ov96_0220CD84(arg0, 0);
    }
    return 0;
}

typedef struct UnkStruct_ov96_0220CBEC {
    u32 a : 8;
    u32 cnt : 8;
    u32 b : 16;
} UnkStruct_ov96_0220CBEC;

s32 ov96_0220CBEC(void *arg0);

s32 ov96_0220CBEC(void *arg0) {
    UnkStruct_ov96_0220CBEC *p = (UnkStruct_ov96_0220CBEC *)((u8 *)arg0 + 0x14);
    p->cnt++;
    if (p->cnt >= 0x2A) {
        ov96_0220CD84(arg0, 0);
    }
    return 0;
}

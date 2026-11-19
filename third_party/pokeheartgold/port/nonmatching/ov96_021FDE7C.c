/* PORTOK hk42 */
#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon.h"
u16 ov96_021FF5A8(void *, s32, s32, VecFx32 *, VecFx32 *, VecFx32 *);
s32 ov96_021FF764(s32, s32, s32);
s32 _d2f(void);
void _dadd(s32, s32, u32, u32);
s32 _dfix(void);
u64 _dflt(s32);
void _dgeq(s32, s32, u32, u32);
void _dgr(void);
void _dleq(void);
u64 _dmul(u32, u32, s32, s32);
void _dsub(s32, s32, u32, u32);
u64 _f2d(s32);
void _fgr(s32, u32);
void _fleq(s32);
s64 _ll_mul(fx32, fx32, s32, s32);
void ov96_021E8228(PokeathlonCourseData *, u8, u8, u32, u32);
s32 ov96_021EAF8C(u32);
void ov96_021FE538(PokeathlonCourseData *, void *, s32);
/* invented: local layout of the course heap block */
typedef struct AthEntry {
    float f24;
    float f28;
    u8 f2c;
    u8 pad2d;
    u8 f2e;
    u8 f2f;
    u8 f30;
    u8 pad31[0x0f];
} AthEntry;

typedef struct AthPlayer {
    u32 f00[3];
    u32 f0c[3];
    u8 pad18[0x0c];
    AthEntry e[3];
    u8 pad78[4];
    VecFx32 pos;
    u8 pad88[3];
    u8 idx;
    VecFx32 vel;
    u8 pad98[4];
    u8 f9c;
    u8 f9d;
    u8 pad9e[4];
    u8 fa2;
    u8 fa3;
    u8 fa4;
    u8 fa5;
    u8 pada6[2];
    u8 fa8;
    u8 pada9[0x23];
    u32 fcc;
    u8 fd0;
    u8 fd1;
    u8 padd2[2];
} AthPlayer;

typedef struct AthHeap {
    u8 pad00[0x30];
    AthPlayer pl[4];
    u8 pad380[0x42];
    u8 f3c2;
    u8 pad3c3[0x279];
    u8 f63c;
    s8 f63d;
    s16 f63e;
} AthHeap;

void ov96_021FDE7C(PokeathlonCourseData *param_1) {
    AthHeap *heap;
    AthPlayer *p;
    u8 *fp;
    VecFx32 hit;
    VecFx32 oldPos;
    VecFx32 vel;
    VecFx32 zero;
    VecFx32 dir;
    u8 flags[4];
    s32 i;
    s32 k;
    s32 mag;
    s32 lim;
    s32 sVar3;
    u16 uVar1;
    float f;
    float sc;
    fx32 fVar6;
    fx32 fVar7;
    s64 l;

    heap = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    heap->f63c = 0;
    {
        u8 *fq = flags;
        u8 *hb = (u8 *)heap;
        AthPlayer *q = heap->pl;
        for (i = 0; i < 4; i++) {
            *fq = 0;
            if (hb[0xd9] != 0) {
                ov96_021FE538(param_1, heap, (s32)q);
                *fq = 1;
            }
            fq++;
            hb += 0xd4;
            q++;
        }
    }
    fp = flags;
    for (i = 0, p = heap->pl; i < 4; i++, p++, fp++) {
        if (*fp != 0) {
            continue;
        }
        oldPos = p->pos;
        VEC_Add(&p->pos, &p->vel, &p->pos);
        p->f0c[p->idx] += p->vel.x;
        if (p->pos.y < 0x20000) {
            p->pos.y = 0x20000;
        } else if (p->pos.y > 0xa8000) {
            p->pos.y = 0xa8000;
        }
        sVar3 = ov96_021EAF8C(p->f00[p->idx]);
        uVar1 = ov96_021FF5A8(heap, p->fcc, sVar3, &oldPos, &p->pos, &p->pos);
        p->fa5 = (u8)(uVar1 >> 8) == 1;
        p->fcc = (u8)uVar1;
        switch ((u8)uVar1) {
        case 2:
            if (p->vel.y > 0) {
                p->vel.y *= -1;
            }
            break;
        case 3:
            if (p->vel.x < 0) {
                p->vel.x *= -1;
            }
            break;
        case 4:
            if (p->vel.y < 0) {
                p->vel.y *= -1;
            }
            break;
        case 5:
            if (p->vel.x > 0) {
                p->vel.x *= -1;
            }
            break;
        case 6:
            if (p->vel.x > 0) {
                p->vel.x *= -1;
            }
            if (p->vel.y > 0) {
                p->vel.y *= -1;
            }
            break;
        case 7:
            if (p->vel.x < 0) {
                p->vel.x *= -1;
            }
            if (p->vel.y > 0) {
                p->vel.y *= -1;
            }
            break;
        case 8:
            if (p->vel.x < 0) {
                p->vel.x *= -1;
            }
            if (p->vel.y < 0) {
                p->vel.y *= -1;
            }
            break;
        case 9:
            if (p->vel.x > 0) {
                p->vel.x *= -1;
            }
            if (p->vel.y < 0) {
                p->vel.y *= -1;
            }
            break;
        }
        sVar3 = ov96_021FF764((s32)param_1, (s32)&heap->pl[0], (s32)&hit);
        if (heap->f63c == 0 && sVar3 != 0) {
            heap->f63c = 1;
            heap->f63e = hit.x / 4096;
            heap->f63d = hit.y / 4096;
        }
        mag = p->pos.x;
        if ((double)mag >= 4194304.0) {
            p->pos.x = (s32)((double)mag - 4194304.0);
            if (p->fd1 == 0 && p->f9c < 0x3c) {
                p->f9c++;
            }
            if (p->fa8 < p->f9c) {
                p->fa8 = p->f9c;
            }
            p->fd1 = 0;
        } else if (mag < 0) {
            p->pos.x = (s32)(4194304.0 + (double)mag);
            if (p->f9c != 0) {
                p->f9c--;
            } else if (p->f9c == 0) {
                p->fd1 = 1;
            }
        }
        vel = p->vel;
        mag = VEC_Mag(&vel);
        if (p->f9d == 0 && p->fa5 != 0 && (lim = p->e[p->idx].f2c * 0x1000, mag > lim)) {
            VEC_Set(&zero, 0, 0, 0);
            dir = p->vel;
            VEC_Normalize(&dir, &dir);
            VEC_MultAdd(lim, &dir, &zero, &dir);
            p->vel = dir;
            vel = p->vel;
            mag = VEC_Mag(&vel);
        }
        if (mag > 0) {
            sc = 1.0f;
            lim = (s32)(sc * 0.4 * 4096.0);
            if (heap->f3c2 != 0) {
                lim += 0x3000;
            }
            VEC_Normalize(&vel, &vel);
            l = (s64)vel.x * lim;
            vel.x = (l + 0x800) >> 12;
            l = (s64)vel.y * lim;
            vel.y = (l + 0x800) >> 12;
            fVar6 = VEC_Mag(&vel);
            fVar7 = VEC_Mag(&p->vel);
            if (fVar7 >= fVar6) {
                VEC_Subtract(&p->vel, &vel, &p->vel);
            } else {
                p->vel.x = 0;
                p->vel.y = 0;
            }
        }
        fVar6 = VEC_Mag(&p->vel);
        k = p->e[p->idx].f2e;
        if (fVar6 >= (k - 1) * 0x1000) {
            p->fa2 = 3;
        } else if (fVar6 >= (k - 3) * 0x1000) {
            p->fa2 = 2;
        } else if (fVar6 == 0) {
            p->fa2 = 0;
        } else {
            p->fa2 = 1;
        }
        if (p->f9d == 0) {
            for (k = 0; k < 3; k++) {
                f = p->e[k].f24;
                if (k == p->idx) {
                    if (p->e[k].f30 != 2) {
                        switch (p->fa2) {
                        case 0:
                            f += 0.5;
                            break;
                        case 1:
                        case 2:
                        case 3:
                            f = f - 0.5;
                            break;
                        }
                        if (p->fa3 != 0) {
                            if (f > 40.0) {
                                f = 40.0f;
                            }
                        } else if (f > p->e[k].f28) {
                            f = p->e[k].f28;
                        }
                    } else if (p->e[k].f30 == 2) {
                        p->fa4--;
                        if (p->fa4 == 0) {
                            p->e[k].f30 = 1;
                            f = 40.0f;
                        }
                    }
                } else {
                    f += 0.5;
                    if (f > p->e[k].f28) {
                        f = p->e[k].f28;
                    }
                }
                if (f <= 0.0f) {
                    if (k != p->idx) {
                        GF_AssertFail();
                    }
                    p->e[k].f24 = 0;
                    if (p->e[k].f30 != 2) {
                        p->e[k].f30 = 2;
                        p->fa3 = 1;
                        p->fa4 = p->e[k].f2f;
                        ov96_021E8228(param_1, p->fd0, p->idx, 1, 1);
                    }
                } else if (f <= 40.0) {
                    p->e[k].f24 = f;
                    p->e[k].f30 = 1;
                } else {
                    p->e[k].f24 = f;
                    p->e[k].f30 = 0;
                }
            }
        }
    }
}

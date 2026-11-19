#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_02203468work.h"

extern VecFx32 ov96_0221C98C[];

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov96_022031A8(void *p, u8 idx, s32 a, s32 b, VecFx32 *out);

void ov96_02202738(Ov96_02203468Work *p, void *q);

void ov96_022043C0(void *a, s32 b);

void ov96_022033FC(void *p);

void ov96_02203468(void *p);

void ov96_02203754(Ov96_02203468Work *p);

void ov96_02203544(PokeathlonCourseData *p, Ov96_02203468Work *q);

void ov96_02204320(s32 a, void *p);
#else
int ov96_022031A8(void *p, u8 idx, u8 a, u8 b, VecFx32 *out);

void ov96_02202738(void *p, void *q);

void ov96_022043C0(void *a, s32 b);

void ov96_022033FC(void *p);

void ov96_02203468(void *p);

void ov96_02203754(void *p);

void ov96_02203544(void *p, void *q);

void ov96_02204320(u8 a, void *p);
#endif

typedef struct {
    u32 w[9];
} UnkStruct_ov96_0220223C;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_0220223C(struct PokeathlonCourseData *param0);

void ov96_0220223C(struct PokeathlonCourseData *param0) {
    int i;
    u8 *data;
    f32 f20;
    u8 *cur;
    Ov96_02203468Work_Item *ps;
    int sidx;
    Ov96_02203468Work *p;
    u8 *dst;
    u8 *src;
    u8 *v7;
    Ov96_02203468Work_Ball *elem;
    int idx;
    int mag;
    int t;
    u32 cnt;
    f32 f7;
    f32 f38;
    VecFx32 v94;
    VecFx32 v88;
    VecFx32 v7c;

    data = PokeathlonCourse_GetDataCopyArea(param0);
    p = PokeathlonCourse_GetHeapAllocPtr4(param0);
    if (ov96_021E5F24(param0) != 0) {
        return;
    }
    if (p->unk_5D0) {
        data += 0x28;
        ov96_02202738(p, ov96_021E8A20(data));
        return;
    }
    dst = ov96_021E8A20(data + 0x50);
    src = ov96_021E8A20(data);
    *(UnkStruct_ov96_0220223C *)dst = *(UnkStruct_ov96_0220223C *)src;
    if (p->unk_5E8 != 0) {
        p->unk_5E8--;
    }
    ov96_021E8A20(data + 0x28);
    i = 0;
    cur = data + 0x50;
    for (; i < 4; cur += 0x28, i++) {
        v7 = ov96_021E8A20(cur);
        if (*(s32 *)v7 != 0) {
            if (p->touch[i].unk_0 != 0 && p->touch[i].unk_4 != 0) {
                p->touch[i].unk_0 = 0;
            } else if (p->touch[i].unk_0 == 0 && p->touch[i].unk_4 == 0) {
                p->touch[i].unk_0 = 1;
                p->touch[i].unk_4 = 1;
            }
        } else {
            p->touch[i].unk_0 = 0;
            p->touch[i].unk_4 = 0;
        }

        if (p->touch[i].unk_0 != 0) {
            idx = ov96_022031A8(p, (u8)i, v7[4], v7[5], &v94);
            if (idx != 12) {
                if (p->items[idx].unk_18 != 0) {
                    continue;
                }
                t = p->balls[idx].state;
                if (t == 0 || t == 2) {
                    p->unk_5CC[i] = idx;
                    elem = &p->balls[idx];
                    elem->accel = v94;
                    v88.x = v7[4] << 12;
                    v88.z = v7[5] << 12;
                    v88.y = 0;
                    elem->unk_34 = v88;
                    if (elem->state == 0) {
                        elem->pos = ov96_0221C98C[idx];
                        elem->state = 2;
                        elem->unk_44 = 0;
                        elem->unk_46 = 0;
                    }
                }
            }
            p->unk_5EA = 0;
        } else if (p->touch[i].unk_4 != 0) {
            if (p->unk_5CC[i] != 12) {
                if ((double)p->unk_5EA < 30.0) {
                    p->unk_5EA++;
                }
            }
        } else {
            if (p->unk_5CC[i] != 12) {
                idx = ov96_022031A8(p, (u8)i, v7[4], v7[5], &v7c);
                if (idx != 12 && idx == p->unk_5CC[i]) {
                    elem = &p->balls[idx];
                    if (elem->unk_44 < 4) {
                        elem->unk_44 += 1;
                        cnt = elem->unk_44;
                        if (cnt <= 1) {
                            elem->unk_46 = 0;
                        } else if (cnt <= 2) {
                            elem->unk_46 = 1;
                        } else if (cnt <= 3) {
                            elem->unk_46 = 2;
                        } else {
                            elem->unk_46 = 3;
                        }
                    }
                } else {
                    VecFx32 v70 = { 0, 0, 0 };
                    VecFx32 v64;
                    VecFx32 v58;
                    VecFx32 v4c;

                    sidx = p->unk_5CC[i];
                    VEC_Subtract(&v7c, &p->balls[sidx].accel, &v64);
                    v4c.x = v7[4] << 12;
                    v4c.z = v7[5] << 12;
                    v4c.y = 0;
                    VEC_Subtract(&v4c, &p->balls[sidx].unk_34, &v58);
                    if (v58.z <= 0) {
                        p->balls[sidx].vel = v64;
                        ps = &p->items[sidx];
                        t = ps->unk_16;
                        if (t <= 60) {
                            f20 = 0.5f;
                        } else if (t <= 70) {
                            f20 = 0.6f;
                        } else if (t <= 80) {
                            f20 = 0.7f;
                        } else if (t <= 90) {
                            f20 = 0.8f;
                        } else {
                            f20 = 1.0f;
                        }
                        mag = VEC_Mag(&p->balls[sidx].vel);
                        f7 = (f32)(mag / 4096);
                        if ((double)f7 > 2.0) {
                            f7 = 2.0f;
                        } else if ((double)f7 < 1.0) {
                            f7 = 1.0f;
                        }
                        f38 = 1.0 + (f32)p->unk_5EA / 30.0;
                        f7 = f20 * (f7 * (2.0 - ps->unk_4)) / f38;
                        if ((double)f7 > 3.0) {
                            f7 = 3.0f;
                        } else if ((double)f7 < 1.0) {
                            f7 = 1.0f;
                        }
                        VEC_Normalize(&p->balls[sidx].vel, &p->balls[sidx].vel);
                        VEC_MultAdd((s32)(f7 * 4096.0f), &p->balls[sidx].vel, &v70, &p->balls[sidx].vel);
                        ov96_02204320((u8)ps->unk_10, &p->balls[sidx].vel);
                        p->balls[sidx].state = 1;
                    }
                }
                p->unk_5CC[i] = 12;
            }
            p->unk_5EA = 0;
        }
    }
    ov96_022043C0(p->unk_5DC, 0x708 - p->unk_5E8);
    ov96_022033FC(param0);
    ov96_02203468(param0);
    ov96_02203754(p);
    ov96_02203544(param0, p);
    if (p->unk_5E8 == 0) {
        p->unk_5D0 = 1;
    } else {
        p->unk_5D0 = 0;
    }
    ov96_02202738(p, ov96_021E8A20(data + 0x28));
}
#else
typedef struct Ov96Ball {
    u8 filler_00[8];
    s32 state;
    u8 filler_0C[0x3C];
} Ov96Ball;

void ov96_0220223C(struct PokeathlonCourseData *param0);

void ov96_0220223C(struct PokeathlonCourseData *param0) {
    u8 *b34;
    u8 *b30;
    u8 *b2c;
    int i;
    u8 *data;
    f32 f20;
    u8 *cur;
    u8 *pi;
    u8 *ps;
    int sidx;
    u8 *q;
    u8 *p;
    u8 *dst;
    u8 *src;
    u8 *v7;
    u8 *elem;
    int idx;
    int r6;
    int mag;
    int t;
    u32 cnt;
    f32 f7;
    f32 f38;
    VecFx32 v94;
    VecFx32 v88;
    VecFx32 v7c;

    data = PokeathlonCourse_GetDataCopyArea(param0);
    p = PokeathlonCourse_GetHeapAllocPtr4(param0);
    if (ov96_021E5F24(param0) != 0) {
        return;
    }
    if (p[0x5D0]) {
        data += 0x28;
        ov96_02202738(p, ov96_021E8A20(data));
        return;
    }
    dst = ov96_021E8A20(data + 0x50);
    src = ov96_021E8A20(data);
    *(UnkStruct_ov96_0220223C *)dst = *(UnkStruct_ov96_0220223C *)src;
    if (*(u16 *)(p + 0x5E8) != 0) {
        (*(u16 *)(p + 0x5E8))--;
    }
    ov96_021E8A20(data + 0x28);
    i = 0;
    cur = data + 0x50;
    q = p;
    b34 = p + 0xD4;
    b2c = p + 0xE0;
    b30 = p + 0xEC;
    for (; i < 4; cur += 0x28, q += 12, i++) {
        v7 = ov96_021E8A20(cur);
        if (*(s32 *)v7 != 0) {
            if (*(s32 *)(q + 0x59C) != 0 && *(s32 *)(q + 0x5A0) != 0) {
                *(s32 *)(q + 0x59C) = 0;
            } else if (*(s32 *)(q + 0x59C) == 0 && *(s32 *)(q + 0x5A0) == 0) {
                *(s32 *)(q + 0x59C) = 1;
                *(s32 *)(q + 0x5A0) = 1;
            }
        } else {
            *(s32 *)(q + 0x59C) = 0;
            *(s32 *)(q + 0x5A0) = 0;
        }

        if (*(s32 *)(q + 0x59C) != 0) {
            idx = ov96_022031A8(p, (u8)i, v7[4], v7[5], &v94);
            if (idx != 12) {
                if (p[idx * 32 + 0x430] != 0) {
                    continue;
                }
                t = ((Ov96Ball *)(p + 0xBC))[idx].state;
                if (t == 0 || t == 2) {
                    p[i + 0x5CC] = idx;
                    elem = p + idx * 72;
                    *(VecFx32 *)(elem + 0xE0) = v94;
                    v88.x = v7[4] << 12;
                    v88.z = v7[5] << 12;
                    v88.y = 0;
                    *(VecFx32 *)(elem + 0xEC) = v88;
                    if (*(s32 *)(elem + 0xC4) == 0) {
                        *(VecFx32 *)(elem + 0xC8) = ov96_0221C98C[idx];
                        *(s32 *)(elem + 0xC4) = 2;
                        *(u16 *)(elem + 0xFC) = 0;
                        *(u16 *)(elem + 0xFE) = 0;
                    }
                }
            }
            p[0x5EA] = 0;
        } else if (*(s32 *)(q + 0x5A0) != 0) {
            if (p[i + 0x5CC] != 12) {
                if ((double)p[0x5EA] < 30.0) {
                    p[0x5EA]++;
                }
            }
        } else {
            pi = p + i;
            if (pi[0x5CC] != 12) {
                idx = ov96_022031A8(p, (u8)i, v7[4], v7[5], &v7c);
                if (idx != 12 && idx == pi[0x5CC]) {
                    elem = p + idx * 72;
                    if (*(u16 *)(elem + 0xFC) < 4) {
                        *(u16 *)(elem + 0xFC) += 1;
                        cnt = *(u16 *)(elem + 0xFC);
                        if (cnt <= 1) {
                            *(u16 *)(elem + 0xFE) = 0;
                        } else if (cnt <= 2) {
                            *(u16 *)(elem + 0xFE) = 1;
                        } else if (cnt <= 3) {
                            *(u16 *)(elem + 0xFE) = 2;
                        } else {
                            *(u16 *)(elem + 0xFE) = 3;
                        }
                    }
                } else {
                    VecFx32 v70 = { 0, 0, 0 };
                    VecFx32 v64;
                    VecFx32 v58;
                    VecFx32 v4c;

                    sidx = pi[0x5CC];
                    r6 = sidx * 72;
                    VEC_Subtract(&v7c, (VecFx32 *)(b2c + r6), &v64);
                    v4c.x = v7[4] << 12;
                    v4c.z = v7[5] << 12;
                    v4c.y = 0;
                    VEC_Subtract(&v4c, (VecFx32 *)(b30 + r6), &v58);
                    if (v58.z <= 0) {
                        *(VecFx32 *)(p + r6 + 0xD4) = v64;
                        ps = p + sidx * 32;
                        t = *(s16 *)(ps + 0x42E);
                        if (t <= 60) {
                            f20 = 0.5f;
                        } else if (t <= 70) {
                            f20 = 0.6f;
                        } else if (t <= 80) {
                            f20 = 0.7f;
                        } else if (t <= 90) {
                            f20 = 0.8f;
                        } else {
                            f20 = 1.0f;
                        }
                        mag = VEC_Mag((VecFx32 *)(b34 + r6));
                        f7 = (f32)(mag / 4096);
                        if ((double)f7 > 2.0) {
                            f7 = 2.0f;
                        } else if ((double)f7 < 1.0) {
                            f7 = 1.0f;
                        }
                        f38 = 1.0 + (f32)p[0x5EA] / 30.0;
                        f7 = f20 * (f7 * (2.0 - *(f32 *)(ps + 0x41C))) / f38;
                        if ((double)f7 > 3.0) {
                            f7 = 3.0f;
                        } else if ((double)f7 < 1.0) {
                            f7 = 1.0f;
                        }
                        VEC_Normalize((VecFx32 *)(b34 + r6), (VecFx32 *)(b34 + r6));
                        VEC_MultAdd((s32)(f7 * 4096.0f), (VecFx32 *)(b34 + r6), &v70, (VecFx32 *)(b34 + r6));
                        ov96_02204320((u8) * (u32 *)(ps + 0x428), (VecFx32 *)(b34 + r6));
                        *(s32 *)(p + r6 + 0xC4) = 1;
                    }
                }
                pi[0x5CC] = 12;
            }
            p[0x5EA] = 0;
        }
    }
    ov96_022043C0(*(void **)(p + 0x5DC), 0x708 - *(u16 *)(p + 0x5E8));
    ov96_022033FC(param0);
    ov96_02203468(param0);
    ov96_02203754(p);
    ov96_02203544(param0, p);
    if (*(u16 *)(p + 0x5E8) == 0) {
        p[0x5D0] = 1;
    } else {
        p[0x5D0] = 0;
    }
    ov96_02202738(p, ov96_021E8A20(data + 0x28));
}
#endif

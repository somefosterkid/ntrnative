#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "math_util.h"
#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"

BOOL ov96_02218744(Ov96RdObj *obj);

BOOL ov96_02218598(Ov96RdObj *a, Ov96RdObj *b);

BOOL ov96_022178A0(Ov96RdObj *a, Ov96RdObj *b, VecFx32 *dir);

u32 ov96_02217E08(Ov96RdObj *obj);

void ov96_02217DC8(Ov96RdObj *obj, u32 a);

void ov96_02217E7C(PokeathlonCourseData *data);

void ov96_02217E7C(PokeathlonCourseData *data) {
    Ov96RdObj *obj;
    Ov96RdObj *other;
    s32 i, j;
    Ov96RdWork *work;
    BOOL r1, r2;
    u32 a, b;
    u8(*hit)[4];

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    hit = work->course.hit;
    for (i = 0; i < 4; i++) {
        obj = &work->course.players[i].obj;
        if (!ov96_02218744(obj) || obj->b5D > 0) {
            continue;
        }
        for (j = i + 1; j < 4; j++) {
            other = &work->course.players[j].obj;
            if (!ov96_02218744(other) || !ov96_02218598(obj, other)) {
                continue;
            }
            VecFx32 dir = { 0, 0, 0 };
            if (hit[i][j] && !obj->f27) {
                hit[i][j] = hit[j][i] = 0;
            }
            if (hit[i][j]) {
                continue;
            }
            VEC_Subtract(&obj->pos2C, &other->pos2C, &dir);
            if (dir.x == 0 && dir.y == 0) {
                dir.x = (MTRandom() & 0x3F) - 32;
                dir.y = (MTRandom() & 0x3F) - 32;
            }
            VEC_Normalize(&dir, &dir);
            r1 = ov96_022178A0(obj, other, &dir);
            dir.x *= -1;
            dir.y *= -1;
            r2 = ov96_022178A0(other, obj, &dir);
            a = ov96_02217E08(obj);
            b = ov96_02217E08(other);
            ov96_02217DC8(obj, a);
            ov96_02217DC8(other, b);
            if (r1 || r2) {
                hit[i][j] = hit[j][i] = 1;
            }
        }
    }
}

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021E8228.c (tools/pokehg_abi.py)
void ov96_021E8228(PokeathlonCourseData *data, u32 a, u32 b, s32 c, s32 d);
#else
void ov96_021E8228(PokeathlonCourseData *data, u8 a, u8 b, s32 c, s32 d);
#endif

void ov96_02217FD0(Ov96RdPlayer *player, PokeathlonCourseData *data);

void ov96_02217FD0(Ov96RdPlayer *player, PokeathlonCourseData *data) {
    Ov96RdObj *obj;
    Ov96RdEntry *entry;
    s32 i, j;
    s32 max;
    f32 add;

    for (i = 0; i < 4; player++, i++) {
        obj = &player->obj;
        for (j = 0, entry = player->obj.entries; j < 3; j++, entry++) {
            add = 0.0f;
            max = entry->stats->s2;
            if (obj->e == j) {
                if (obj->f28) {
                    obj->timer5C--;
                    if (obj->timer5C <= 0) {
                        entry->value = 0x14000;
                        obj->pos20 = obj->pos2C;
                        obj->f28 = 0;
                    }
                } else if (entry->value <= 0) {
                    obj->timer5C = 60;
                    obj->f28 = 1;
                    obj->f29 = 0;
                    obj->f30 = 0;
                    ov96_021E8228(obj->data, obj->d, obj->e, 1, 1);
                }
            } else {
                add = 819.0f;
            }
            entry->value = (f32)entry->value + add;
            if (entry->value > max) {
                entry->value = max;
            }
        }
    }
}

s32 ov96_021E6104(void);

s32 ov96_022158D4(s32, s32);

s32 ov96_022158EC(s32 *, s32 *, s32);

s32 ov96_0221862C(Ov96RdObj *obj);

s32 ov96_022186B8(Ov96RdObj *obj, Ov96RdPlayer *player);

s32 ov96_02218B1C(Ov96RdObj *obj, PokeathlonCourseData *data);

s32 ov96_0221910C(Ov96RdObj *obj);

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define OV96_FX(v) ((s32)((v) > 0 ? 0.5f + (f32)((v) << 12) : (f32)((v) << 12) - 0.5f))

void ov96_022180CC(Ov96RdPlayer *players, PokeathlonCourseData *data);

void ov96_022180CC(Ov96RdPlayer *players, PokeathlonCourseData *data) {
    s32 i;
    Ov96RdObj *obj;
    Ov96RdFlags *flags;
    Ov96RdPlayer *p;
    s16 px;
    s16 py;
    s32 r;

    i = 0;
    p = players;
    do {
        s32 a[3] = { 0, 0, 0 };
        s32 b[3] = { 0, 0, 0 };
        flags = &p->flags;
        obj = &p->obj;
        a[0] = OV96_FX(p->pos.x);
        a[1] = OV96_FX(p->pos.y);
        b[0] = OV96_FX(p->tx);
        b[1] = OV96_FX(p->ty);
        if (flags->b14) {
            if (ov96_022158D4(p->pos.x, p->pos.y)) {
                if (ov96_0221862C(obj)) {
                    ov96_0221910C(obj);
                }
            } else {
                r = OV96_FX(ov96_021E6104());
                if (obj->f26 == 0 && obj->state != 3 && ov96_022158EC((s32 *)&obj->pos2C, a, r)) {
                    flags->b29 = 1;
                }
            }
        } else if (flags->b15) {
            if (flags->b29 == 1) {
                px = obj->pos2C.x >> 12;
                py = obj->pos2C.y >> 12;
                if (obj->state == 3) {
                    flags->b29 = 0;
                } else if (ABS(p->pos.x - px) > 1 || ABS(p->pos.y - py) > 1) {
                    ov96_022186B8(obj, p);
                }
            }
        } else if (flags->cnt16) {
            if (flags->b29 == 1 && (obj->state == 1 || obj->state == 4)) {
                if (obj->f28 == 0 && obj->f26 == 0) {
                    if (ov96_022158EC(b, a, 0x4000)) {
                        if (flags->cnt16 <= 7) {
                            obj->f30 = 0;
                            obj->f29 = 1;
                        }
                    } else if (!ov96_022158EC(b, a, 0xC000) && flags->cnt16 <= 7) {
                        obj->f30 = 1;
                        obj->f29 = 0;
                        obj->target = p->pos;
                    }
                }
            }
            flags->b29 = 0;
        }
        ov96_02218B1C(&p->obj, data);
        p++;
        i++;
    } while (i < 4);
    ov96_02217E7C(data);
    ov96_02217FD0(players, data);
}

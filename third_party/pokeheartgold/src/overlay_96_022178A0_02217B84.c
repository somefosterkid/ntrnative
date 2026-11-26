#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "sprite_system.h"
#include "unk_02009D48.h"

#include "overlay_96_ov96_rd_work.h"

PokeathlonMonPerf *ov96_021E60C0(PokeathlonCourseData *data, s32 a, s32 b);

u32 ov96_021E6104(void);

s32 ov96_021E6138(PokeathlonMonPerf *a);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021EAA00.c (tools/pokehg_abi.py)
struct Ov96SpriteSetElem *ov96_021EAA04(struct Ov96SpriteSet *set, s32 idx);
#else
struct Ov96SpriteSetElem *ov96_021EAA04(struct Ov96SpriteSet *set, u8 idx);
#endif

void ov96_021EAA20(struct Ov96SpriteSetElem *elem);

void ov96_021EAB38(struct Ov96SpriteSetElem *elem, s32 flag);

void ov96_021EABA8(struct Ov96SpriteSetElem *elem, u32 priority);

void ov96_021EABE0(struct Ov96SpriteSetElem *elem, u8 affine);

void ov96_021EAC0C(struct Ov96SpriteSetElem *elem, s32 state);

void ov96_021E62AC(PokeathlonCourseData *data, s32 a1, struct UnkStruct_ov96_021E9A78 *resSrc, SpriteList *list, s32 a4, u16 a5, void *a6);







void ov96_021EAF6C(void *sprite, u32 a);

void ov96_021EAF70(void *sprite, u32 a, u32 b);

void ov96_021EAF94(void *sprite, s32 x, s32 y);

void ov96_021EB0A4(void *sprite, s32 x, s32 y, s32 *outX, s32 *outY);

void ov96_02218510(void *sprite, s32 a);

void ov96_02218578(Ov96RdObj *obj, s32 a);

void ov96_0221918C(PokeathlonCourseData *data);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_0221935C.c (tools/pokehg_abi.py)
Ov96RdStats *ov96_0221935C(PokeathlonCourseData *data, u32 a, u32 b);
#else
Ov96RdStats *ov96_0221935C(PokeathlonCourseData *data, u8 a, u8 b);
#endif

typedef struct Ov96NarcPair {
    u32 a;
    u32 b;
} Ov96NarcPair;

extern const Ov96RdPos16 ov96_0221D6E4[4];

void ov96_02217B84(Ov96RdWork *work, PokeathlonCourseData *data);

void ov96_02217B84(Ov96RdWork *work, PokeathlonCourseData *data) {
    Ov96NarcPair narc[3] = { 0 };
    Ov96RdPos16 pos;
    s32 x1, y1;
    s32 x2, y2;
    u8 q, r;
    s32 x, y;
    struct Ov96SpriteSetElem *sprite;
    Ov96RdEntry *entry;
    Ov96RdObj *obj;
    s32 kind;
    s32 i;

    ReadWholeNarcMemberByIdPair(narc, NARC_a_1_7_0, 19);
    GF_ASSERT(work != NULL);
    ov96_0221918C(data);
    for (i = 0; i < 12; i++) {
        q = i / 3;
        r = i % 3;
        entry = &work->course.players[q].obj.entries[r];
        sprite = ov96_021EAA04(work->spriteSet, i);
        ov96_021EAB38(sprite, (u8)(r == 0));
        kind = ov96_021E6138(ov96_021E60C0(data, q, r));
        ov96_021EAF70(sprite, narc[kind - 1].a, narc[kind - 1].b);
        sprite = ov96_021EAA04(work->spriteSet, i);
        ov96_021EAA20(sprite);
        ov96_021EABE0(sprite, 2);
        ov96_02218510(sprite, 1);
        x = ov96_0221D6E4[q].x;
        y = ov96_0221D6E4[q].y;
        if (q == ov96_021E5F24(data)) {
            pos.x = x;
            pos.y = y;
        }
        ov96_021EAC0C(sprite, 2);
        ov96_021EABA8(sprite, 7);
        ov96_021EAF94(sprite, x, y);
        ov96_021EAF6C(sprite, ov96_021E6104());
        ov96_021EB0A4(sprite, x, y, &x1, &y1);
        entry->idx = i;
        entry->sprite = sprite;
        entry->stats = ov96_0221935C(data, q, r);
        entry->value = entry->stats->s2;
    }
    ov96_021E62AC(data, 0, work->resSrc, SpriteManager_GetSpriteList(work->spriteManager), 1, 1, &pos);
    for (i = 0; i < 4; i++) {
        obj = &work->course.players[i].obj;
        obj->owner = &work->course.players[i];
        obj->info = work->course.players[i].obj.entries;
        obj->unkC = work->unk_180;
        obj->data = data;
        ov96_021EB0A4(obj->info->sprite, ov96_0221D6E4[i].x, ov96_0221D6E4[i].y, &x2, &y2);
        ov96_02218578(obj, 1);
        obj->f16 = 2;
        obj->pos2C.x = x2 << 12;
        obj->pos2C.y = y2 << 12;
        obj->pos20.x = x2 << 12;
        obj->pos20.y = y2 << 12;
        obj->d = i;
        obj->w54 = 0;
        obj->b5F = 3;
        work->course.players[i].index = i;
    }
}

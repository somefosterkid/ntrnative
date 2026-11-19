#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

typedef struct { /* invented */
    ManagedSprite *sprite;
    u8 unk4[0x10];
    fx32 scale;
    u32 counter : 8;
    u32 timer : 8;
    u32 flag16 : 1;
    u32 flag17 : 1;
    u32 flag18 : 1;
    u32 flag19 : 1;
    u32 pad : 12;
} UnkPokeathlonElem;

typedef struct { /* invented */
    u8 unk0[4];
    u8 unk4[12];
    u8 unk10[12];
    u32 unk1C : 24;
    u32 pad1C : 8;
    u32 unk20_0 : 7;
    u32 unk20_7 : 12;
    u32 unk20_19 : 12;
    u32 pad20 : 1;
} UnkPokeathlonArg1;

typedef struct { /* invented */
    u8 unk0;
    void *unk4;
} UnkSortEntry;

u8 ov96_0220F3B4(u8 *);
void *ov96_021EAA04(void *, u8);
void ov96_021EAB38(void *, u8);
void ov96_021EABA8(void *, int);
void ov96_021EAC0C(void *, int);
void ov96_021EB01C(void *, s32, s32, s32);
void ov96_021EB06C(void *, u8, u8, s32 *, s32 *);
void ov96_021EB0A4(void *, u8, u8, s32 *, s32 *);
void ov96_021EB10C(void *, f32, f32);
void ov96_0220D52C(void *, void *);
void ov96_0220D554(void *, s32);
s32 ov96_0220DE90(void *elem1, void *elem2);
s32 ov96_0220E8C0(u8 *, u32, f32 *);
u8 *ov96_0220F378(PokeathlonCourseData *, u8, u8);

void ov96_0220DEAC(u8 *arg0, UnkPokeathlonArg1 *arg1, PokeathlonCourseData *arg2) {
    UnkSortEntry entries[12] = { 0 };
    s32 x;
    s32 y;
    s32 sp34;
    s32 sp30;
    f32 scale;
    f32 sp2C;
    void *heap;
    u8 visible;
    UnkPokeathlonElem *elem;
    void *obj;
    u8 *course;
    u8 val;
    int i;
    s32 div;

    heap = PokeathlonCourse_GetHeapAllocPtr4(arg2);
    elem = (UnkPokeathlonElem *)(arg0 + 0x3C);
    for (i = 0; i < 12; i++) {
        obj = ov96_021EAA04(*(void **)((u8 *)heap + 0x20), i);
        GF_ASSERT(elem != NULL);
        GF_ASSERT(obj != NULL);
        val = arg1->unk10[i];
        ov96_021EB0A4(obj, arg1->unk4[i], val, &x, &y);
        entries[i].unk0 = val;
        entries[i].unk4 = obj;
        visible = (arg1->unk20_7 >> i) & 1;
        if (visible == 1 && elem->flag19 == 0) {
            ov96_0220D554(obj, 1);
            elem->flag18 = 1;
            elem->timer = 6;
            elem->scale = FX32_ONE;
            PlaySE(0x89E);
        } else if (visible == 0 && elem->flag19 == 1) {
            ManagedSprite_SetPositionXYWithSubscreenOffset(elem->sprite, x, y, FX32_CONST(480));
            ManagedSprite_SetDrawFlag(elem->sprite, 1);
            ManagedSprite_ResetSpriteAnimCtrlState(elem->sprite);
            ov96_0220D554(obj, 0);
            ov96_021EB10C(obj, 1.0f, 1.0f);
            ov96_021EAB38(obj, 1);
            elem->flag18 = 0;
            PlaySE(0x8B6);
        }
        elem->flag19 = visible;
        if (elem->flag18) {
            elem->timer--;
            if (elem->timer == 0) {
                ov96_021EAB38(obj, 0);
            } else {
                elem->scale -= 0x19A;
                scale = elem->scale / 4096.0f;
                ov96_021EB10C(obj, scale, scale);
                ov96_021EB06C(obj, arg1->unk4[i], arg1->unk10[i], &sp34, &sp30);
                if (sp30 >= 0xB4) {
                    y = val + 4 * (6 - elem->timer);
                    ov96_021EB01C(obj, x, y, 1);
                }
            }
        } else {
            val = (arg1->unk1C >> (i * 2)) & 3;
            ov96_021EB01C(obj, x, y, 1);
            ov96_021EAC0C(obj, val + 1);
            if ((arg1->unk20_19 >> i) & 1) {
                div = i / 3;
                course = ov96_0220F378(arg2, div, i % 3);
                if (div == ov96_021E5F24(arg2) && elem->flag16 == 0 && elem->flag17 == 0) {
                    if (course[2] == 0) {
                        PlaySE(0x5F3);
                    } else {
                        PlaySE(0x8C5);
                    }
                }
                if (course[2] == 0) {
                    ov96_0220D52C(elem->unk4, obj);
                    elem->flag17 = 1;
                } else {
                    elem->flag16 = 1;
                }
            } else {
                elem->flag17 = 0;
            }
            if (elem->flag16) {
                course = ov96_0220F378(arg2, i / 3, i % 3);
                ov96_021EB01C(obj, x, y + ov96_0220E8C0(course, elem->counter, &sp2C), 0);
                ov96_021EB10C(obj, sp2C, sp2C);
                if (++elem->counter > ov96_0220F3B4(course)) {
                    elem->flag16 = 0;
                    elem->counter = 0;
                    ov96_021EB01C(obj, x, y, 0);
                    ov96_021EB10C(obj, 1.0f, 1.0f);
                }
            }
        }
        elem++;
    }
    MATH_QSort(entries, 12, 8, ov96_0220DE90, NULL);
    for (i = 0; i < 12; i++) {
        GF_ASSERT(entries[i].unk4 != NULL);
        ov96_021EABA8(entries[i].unk4, i + 7);
    }
}

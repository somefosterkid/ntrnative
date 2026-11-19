#include "error_handling.h"
#include "filesystem.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
s32 ov96_021E6104(void);
s32 ov96_021E6138(void *arg0);
u32 _s32_div_f(u32, u32);
u32 ov96_021E60C0(PokeathlonCourseData *, u32, u32);
void ov96_021E62AC(PokeathlonCourseData *, u32, u32, SpriteList *, u32, u32, s16 *);
u32 ov96_021E64F8(PokeathlonCourseData *, u32, u32, SpriteList *, u32);
u8 *ov96_021E8BB0(u32);
u32 ov96_021EAA04(u32, u8);
u32 ov96_021EAA20(u32);
void ov96_021EAB38(u32, u32);
void ov96_021EABA8(u32, u32);
void ov96_021EAC0C(u32, u32);
void ov96_021EAF6C(u32, s32);
void ov96_021EAF70(u32, u32, u32);
void ov96_021EAF94(u32, s32, s32);
void ov96_021EB0A4(u32, s32, s32, s32 *, s32 *);
void ov96_021EB10C(u32, u32, u32);
void ov96_0220D554(u32, u32);
void ov96_0220F280(PokeathlonCourseData *);
u32 ov96_0220F378(PokeathlonCourseData *, u8, u8);
extern s16 ov96_0221CFBC[][3][2];

void ov96_0220F4A0(u8 *work) {
    s32 pos[3][2] = { 0 };
    s16 coords[3][2];
    s32 outX;
    s32 outY;
    u8 *anim;
    u32 obj;
    u8 *base;
    s32 quot;
    s32 y;
    s32 x;
    s32 rem;
    PokeathlonCourseData *data;
    u32 sprObj;
    s32 i;
    u32 *flags;
    u8 *entry;
    u8 *copy;
    s32 r;
    s16 *tbl;

    ReadWholeNarcMemberByIdPair(pos, NARC_a_1_7_0, 0x11);
    GF_ASSERT(work != NULL);
    GF_ASSERT(*(PokeathlonCourseData **)(work + 4) != NULL);
    data = *(PokeathlonCourseData **)(work + 4);
    ov96_0220F280(data);
    for (i = 0, flags = (u32 *)(work + 0x23C); i < 4; i++) {
        *flags = (*flags & ~3) | (i & 3);
        flags = (u32 *)((u8 *)flags + 0xE4);
    }
    for (i = 0, base = work + 0x164; i < 12; i++) {
        rem = i % 3;
        quot = i / 3;
        entry = base + quot * 0xE4 + rem * 0x48;
        sprObj = ov96_021EAA04(*(u32 *)(work + 0x20), i);
        ov96_021EAB38(sprObj, 1);
        r = ov96_021E6138((void *)ov96_021E60C0(data, i / 3, i % 3));
        ov96_021EAF70(sprObj, pos[r - 1][0], pos[r - 1][1]);
        obj = ov96_021EAA04(*(u32 *)(work + 0x20), i);
        anim = ov96_021E8BB0(ov96_021EAA20(obj));
        ov96_0220D554(obj, 0);
        ov96_021EB10C(obj, 0x3F800000, 0x3F800000);
        tbl = ov96_0221CFBC[quot][rem];
        x = tbl[0];
        y = tbl[1];
        ov96_021EAC0C(obj, 2);
        ov96_021EAF94(obj, x, y);
        ov96_021EAF6C(obj, ov96_021E6104());
        ov96_021EB0A4(obj, x, y, &outX, &outY);
        *(PokeathlonCourseData **)entry = data;
        *(u32 *)(entry + 0x40) = (*(u32 *)(entry + 0x40) & 0xFFF0FFFF) | ((i & 0xF) << 16);
        entry[0x45] = quot;
        entry[0x46] = rem;
        *(u32 *)(entry + 4) = obj;
        *(u32 *)(entry + 0x40) = (*(u32 *)(entry + 0x40) & 0xFCFFFFFF) | ((*(u16 *)(anim + 4) & 3) << 24);
        *(u32 *)(entry + 8) = ov96_0220F378(data, quot, rem);
        *(u32 *)(entry + 0xC) = 2;
        *(u32 *)(entry + 0x40) = (*(u32 *)(entry + 0x40) & 0xFF0FFFFF) | (2 << 20);
        *(s32 *)(entry + 0x1C) = outX << 12;
        *(s32 *)(entry + 0x20) = outY << 12;
        *(s32 *)(entry + 0x10) = outX << 12;
        *(s32 *)(entry + 0x14) = outY << 12;
        if (quot == ov96_021E5F24(data)) {
            coords[rem][0] = x;
            coords[rem][1] = y;
            Sprite_SetDrawPriority((Sprite *)ov96_021E64F8(data, obj, *(u32 *)(work + 0x1C), SpriteManager_GetSpriteList(*(SpriteManager **)(work + 0x10)), 1), 4);
            ov96_021EABA8(obj, 6);
        } else {
            ov96_021EABA8(obj, 7);
        }
        if (ov96_021E5F24(data) == FALSE) {
            copy = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0x28);
            copy[i + 4] = outX;
            copy[i + 0x10] = outY;
        }
    }
    ov96_021E62AC(data, 0, *(u32 *)(work + 0x1C), SpriteManager_GetSpriteList(*(SpriteManager **)(work + 0x10)), 1, 3, (s16 *)coords);
}

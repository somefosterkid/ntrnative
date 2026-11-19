#include "global.h"

#include "sprite_system.h"

typedef struct UnkStruct_ov96_0220BE28 {
    void *course;
    s32 w4[3];
    u32 w10[3];
    ManagedSprite *spr1C;
    ManagedSprite *spr20;
    ManagedSprite *spr24;
    ManagedSprite *spr28;
    ManagedSprite *spr2C;
    ManagedSprite *spr30;
    u32 w34;
    u32 pad38 : 15;
    u32 c : 8;
    u32 d : 2;
    u32 a : 2;
    u32 b : 2;
    u32 pad38b : 3;
} UnkStruct_ov96_0220BE28;

typedef struct {
    u8 filler0[4];
    u16 unk4;
} Unk8BB0;

u32 ov96_021EAA20(s32 param0);

Unk8BB0 *ov96_021E8BB0(u32 param0);

void ov96_0220BFB4(UnkStruct_ov96_0220BE28 *p, s32 anim);

void ov96_0220BFB4(UnkStruct_ov96_0220BE28 *p, s32 anim) {
    s16 x = p->b * 0x40 + 0x48;
    s16 y = 0x28;
    ManagedSprite *spr = p->spr24;

    if (ov96_021E8BB0(ov96_021EAA20(p->w4[p->d]))->unk4 != 0) {
        y -= 0x10;
    }
    ManagedSprite_SetPositionXY(spr, x, y);
    ManagedSprite_SetAnimNoRestart(spr, anim);
    ManagedSprite_SetDrawFlag(spr, 1);
}

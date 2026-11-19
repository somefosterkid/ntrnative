#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220_be28.h"

s32 ov96_021EAA04(s32 param_1, s32 param_2);

ManagedSprite *ov96_0220D13C(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u8 param_4, u16 param_5, u16 param_6);

void ov96_0220BE28(UnkStruct_ov96_0220BE28 *p, SpriteSystem *spriteSys, SpriteManager *spriteMan, s32 arg3, u8 idx, PokeathlonCourseData *course, u32 *arg6);

void ov96_0220BE28(UnkStruct_ov96_0220BE28 *p, SpriteSystem *spriteSys, SpriteManager *spriteMan, s32 arg3, u8 idx, PokeathlonCourseData *course, u32 *arg6) {
    s32 i;
    u32 *dst;
    s32 y;
    u32 n;

    MI_CpuFill8(p, 0, sizeof(UnkStruct_ov96_0220BE28));
    n = idx;
    p->a = n;
    p->course = course;
    p->b = n;
    if (p->a > ov96_021E5F24(course)) {
        p->b--;
    }
    for (i = 0; i < 3; i++) {
        p->w4[i] = ov96_021EAA04(arg3, (u8)(i + p->b * 3));
        p->w10[i] = arg6[i];
    }
    dst = (u32 *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x50 + n * 0x28);
    *dst = (*dst & 0xffff00ff) | ((arg6[0] & 0xff) << 8);
    y = p->b * 0x40 + 0x48;
    p->spr1C = ov96_0220D13C(spriteSys, spriteMan, y, 0x38, 0xc, 0x1d);
    p->spr20 = ov96_0220D13C(spriteSys, spriteMan, y, 0x38, 5, 0x1c);
    ManagedSprite_SetDrawFlag(p->spr20, 0);
    p->spr28 = ov96_0220D13C(spriteSys, spriteMan, y, 0x28, 0x10, 0x1f);
    ManagedSprite_SetAffineOverwriteMode(p->spr28, 1);
    ManagedSprite_SetAffineScale(p->spr28, 0.7, 0.7);
    ManagedSprite_SetDrawFlag(p->spr28, 0);
    p->spr24 = ov96_0220D13C(spriteSys, spriteMan, y, 0x28, 0x18, 0x1f);
    ManagedSprite_SetDrawFlag(p->spr24, 0);
    p->spr2C = ov96_0220D13C(spriteSys, spriteMan, y, 0x28, 0xe, 0x1e);
    ManagedSprite_SetDrawFlag(p->spr2C, 0);
    p->spr30 = ov96_0220D13C(spriteSys, spriteMan, y, 0x28, 0x1b, 0x1e);
    ManagedSprite_SetDrawFlag(p->spr30, 0);
}

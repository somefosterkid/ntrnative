#include "global.h"

#include "heap.h"
#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"
#include "overlay_96_app_0220a87c.h"

struct PokeathlonCourseData;

UnkStruct_ov96_0220B148 *ov96_0220AE40(enum HeapID, SpriteSystem *, SpriteManager *, s32);

void ov96_0220B178(ManagedSprite **, SpriteSystem *, SpriteManager *);

ManagedSprite *ov96_0220D13C(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);

Ov96App_0220A87C *ov96_0220A744(enum HeapID arg0, SpriteSystem *arg1, SpriteManager *arg2, struct PokeathlonCourseData *arg3);

Ov96App_0220A87C *ov96_0220A744(enum HeapID arg0, SpriteSystem *arg1, SpriteManager *arg2, struct PokeathlonCourseData *arg3) {
    Ov96App_0220A87C *spC;
    enum HeapID sp8;
    ManagedSprite *temp_r0_2;
    ManagedSprite *temp_r0_3;
    u32 var_r4;
    Ov96App_0220A87C *temp_r0;

    sp8 = arg0;
    temp_r0 = Heap_Alloc(arg0, sizeof(Ov96App_0220A87C));
    spC = temp_r0;
    MI_CpuFill8(temp_r0, 0U, sizeof(Ov96App_0220A87C));
    spC->unk_180 = (spC->unk_180 & ~0xF) | 1;
    spC->unk_180 = spC->unk_180 & ~0xF0;
    spC->spriteSystem = arg1;
    spC->spriteManager = arg2;
    spC->course = arg3;
    temp_r0_2 = ov96_0220D13C(arg1, arg2, 0, 0, 7, 1);
    spC->unk_C = temp_r0_2;
    ManagedSprite_SetDrawFlag(temp_r0_2, 0);
    temp_r0_3 = ov96_0220D13C(arg1, arg2, 0, 0, 0x12, 1);
    spC->unk_10 = temp_r0_3;
    ManagedSprite_SetDrawFlag(temp_r0_3, 0);
    var_r4 = 0;
    do {
        spC->unk_154[var_r4] = ov96_0220AE40(sp8, arg1, arg2, (u16)var_r4);
        var_r4 += 1;
    } while ((s32)var_r4 < 0xA);
    ov96_0220B178((ManagedSprite **)spC->unk_14, arg1, arg2);
    return spC;
}

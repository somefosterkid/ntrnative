#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite_system.h"

s32 ov96_021E62AC(PokeathlonCourseData *, s32, s32, SpriteList *, s32, u16, s32 *);

s32 ov96_021E637C(PokeathlonCourseData *);

s32 ov96_021EB144(void *, s32);

s32 ov96_0220A840(void *);

s32 ov96_0220AD34(void *);

BOOL ov96_02209130(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_02209130(PokeathlonCourseData *arg0, u8 *arg1) {
    s32 temp_r0;
    void *temp_r4;

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = *arg1;
    switch (temp_r0) {
    case 0:
        ov96_0220A840(*(void **)((u8 *)temp_r4 + 0x40));
        if (ov96_0220AD34(*(void **)((u8 *)temp_r4 + 0x40)) == 2) {
            u16 spC[2] = { 120, 132 };
            ov96_021E62AC(arg0, 0, *(s32 *)((u8 *)temp_r4 + 0x14), SpriteManager_GetSpriteList(*(SpriteManager **)((u8 *)temp_r4 + 0xC)), 1, 1, (s32 *)spC);
            *arg1 += 1;
        }
        break;
    case 1:
        if (ov96_021E637C(arg0) != 0) {
            ov96_021EB144(*(void **)((u8 *)temp_r4 + 0x10), 0);
            PokeathlonCourse_SetStateField07(arg0, 1U);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

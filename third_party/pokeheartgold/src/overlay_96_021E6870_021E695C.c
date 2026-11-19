#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E87EC(s32, void *, s32, void *);

s32 ov96_021E99F8(void);

s32 ov96_021E695C(PokeathlonCourseData *arg0);

s32 ov96_021E695C(PokeathlonCourseData *arg0) {
    arg0->unk_200_PokeathlonCourseData = 0;
    if (ov96_021E87EC(0x17, ((u8 *)arg0) + 0x200, ov96_021E99F8(), arg0->system) != 0) {
        PokeathlonCourse_SetStateField07(arg0, 0x26U);
    }
    return 0;
}

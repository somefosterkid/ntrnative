#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"

void ov96_021EFB20(PokeathlonCourseData *arg0, s32 arg1);

void ov96_021EFB20(PokeathlonCourseData *data, s32 arg1) {
    void *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, 4, 1, *(enum HeapID *)((u8 *)v0 + 0xC));
    *(s32 *)((u8 *)v0 + 0x18) = arg1;
}

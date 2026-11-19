#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "error_handling.h"
#include "screen_fade.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

void ov96_021EFC10(void *arg0);

void ov96_021EFC8C(void *arg0);

s32 ov96_021EF98C(void *);

s32 ov96_021EFB58(void *);

void ov96_021EFD08(PokeathlonCourseData *arg0);

void ov96_021EFD08(PokeathlonCourseData *arg0) {
    s32 temp_r0_2;
    void *temp_r0;

    temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    BgClearTilemapBufferAndCommit(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_4), 0U);
    BgClearTilemapBufferAndCommit(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_4), 2U);
    BgClearTilemapBufferAndCommit(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_4), 4U);
    BgClearTilemapBufferAndCommit(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_4), 5U);
    BgClearTilemapBufferAndCommit(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_4), 6U);
    BgClearTilemapBufferAndCommit(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_4), 7U);
    ov96_021EF98C(temp_r0);
    temp_r0_2 = *((s32 *)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_18);
    switch (temp_r0_2) {
    case 1:
        ov96_021EFB58(temp_r0);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 4, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_C));
        break;
    case 2:
        ov96_021EFC10(temp_r0);
        break;
    case 3:
        ov96_021EFC8C(temp_r0);
        break;
    default:
        GF_AssertFail();
        break;
    }
    PokeathlonCourse_SetStateField07(arg0, *((s32 *)&((UnkStruct_ov96_021EF610 *)temp_r0)->unk_18));
}

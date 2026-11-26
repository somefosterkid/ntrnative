#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "overlay_96_app_0221040c.h"

#include "pokeathlon/pokeathlon.h"
#include "error_handling.h"
#include "screen_fade.h"

s32 ov96_021E8318(PokeathlonCourseData *, s16);

s32 ov96_0220E9A0(PokeathlonCourseData *, s32);

void ov96_0220E2A8(PokeathlonCourseData *arg0);

s32 ov96_021E667C(PokeathlonCourseData *);
s32 ov96_0220D408(UnkStruct_ov96_0220E3F0_6A8 *);
s32 ov96_0220EFD0(u8 *, BgConfig *, enum HeapID);
s32 ov96_0220F03C(PokeathlonCourseData *);
s32 ov96_0220F1CC(PokeathlonCourseData *);
BOOL ov96_0220E2DC(PokeathlonCourseData *arg0, u8 *arg1);

void ov96_0220E2A8(PokeathlonCourseData *data) {
    u8 v0;

    ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
    v0 = (u8)ov96_022104C4(((UnkStruct_ov96_0220E3F0 *)PokeathlonCourse_GetHeapAllocPtr4(data))->unk_C4);
    ov96_021E8318(data, v0);
    ov96_0220E9A0(data, v0);
}

BOOL ov96_0220E2DC(PokeathlonCourseData *data, u8 *arg1) {
    PokeathlonTally *v2;
    u8 v1;
    UnkStruct_ov96_0220E3F0 *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_0220EFD0(v0->hud.unk_194, v0->bgConfig, v0->heapId);
    v1 = *arg1;
    switch (v1) {
    case 0:
        ov96_0220F03C(data);
        if (ov96_0220F1CC(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        ov96_0220F03C(data);
        v2 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
        ov96_02210324(v0->unk_C4, (u8)((u32)v2->pairs[3].word1 >> 0x1E), (u16)((u32)(v2->pairs[4].word0 << 0x19) >> 0x19));
        if ((ov96_021E667C(data) != 0) && (ov96_0220D408(&v0->unk_6A8) != 0)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v0->heapId);
            PokeathlonCourse_SetStateField07(data, 2U);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "overlay_96_app_0221040c.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E6454(PokeathlonCourseData *, s32);

s32 ov96_0220DC7C(UnkStruct_ov96_0220E3F0_Hud *, u8 *, PokeathlonCourseData *);

s32 ov96_0220DEAC(UnkStruct_ov96_0220E3F0_Hud *, u8 *, PokeathlonCourseData *);

void ov96_0221013C(UnkStruct_ov96_0220E3F0_Hud *arg0, PokeathlonCourseData *arg1);

void ov96_0221013C(UnkStruct_ov96_0220E3F0_Hud *arg0, PokeathlonCourseData *arg1) {
    u8 *data;
    UnkStruct_ov96_0220E3F0 *heap;
    u8 idx;
    s32 val;
    u16 *arr;
    data = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg1) + 0xF0);
    heap = PokeathlonCourse_GetHeapAllocPtr4(arg1);
    ov96_0220DC7C(arg0, data, arg1);
    ov96_0220DEAC(arg0, data, arg1);
    idx = (*(u32 *)(data + 0x1C)) >> 30;
    arr = arg0->unk_18C;
    if (((u32)(*(s32 *)(data + 0x20) << 25) >> 25) != arr[idx]) {
        ov96_02210390(arg0->unk_0, idx, (u32)(*(s32 *)(data + 0x20) << 25) >> 25);
    }
    arr[idx] = (u32)(*(s32 *)(data + 0x20) << 25) >> 25;
    val = ((u32)(*(u32 *)(data + 0x1C) << 2) >> 26) - 1;
    if (val <= 0) {
        val = 0;
    }
    if (val <= 0) {
        val = 0;
    }
    ov96_0221040C(heap->unk_C4, val);
    ov96_021E6454(arg1, 0x1E * ((u32)(*(u32 *)(data + 0x1C) << 2) >> 26));
}

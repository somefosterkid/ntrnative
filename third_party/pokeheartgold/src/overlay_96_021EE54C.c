#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"

u32 ov96_021EE54C(PokeathlonCourseData *arg0);

u32 ov96_021EE54C(PokeathlonCourseData *data) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return (u32)(v1->u8s[0x21] << 0x1E) >> 0x1F;
}

u32 ov96_021EE580(PokeathlonCourseData *arg0);

u32 ov96_021EE580(PokeathlonCourseData *data) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return (u32)(v1->pairs[1].word0 * 4) >> 0x1F;
}

void *ov96_021EE5B4(s32 arg0, enum HeapID arg1);

void *ov96_021EE5B4(s32 arg0, enum HeapID heapID) {
    void *dest;

    dest = Heap_Alloc(heapID, 0x30U);
    MI_CpuFill8(dest, 0U, 0x30U);
    *(enum HeapID *)dest = heapID;
    *(s32 *)((u8 *)dest + 8) = arg0;
    *(NARC **)((u8 *)dest + 0xC) = NARC_New(NARC_a_2_1_9, heapID);
    return dest;
}

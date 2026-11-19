#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_ov96_0220_data.h"

s32 ov96_0220A424(PokeathlonCourseData *);

typedef struct UnkStruct_ov96_0220A4DC_Bits {
    u32 a : 8;
    u32 b : 8;
    u32 c : 8;
    u32 d : 2;
    u32 e : 1;
    u32 f : 1;
    u32 g : 1;
} UnkStruct_ov96_0220A4DC_Bits;

typedef struct UnkStruct_ov96_0220A4DC_Blob36 {
    u32 w[9];
} UnkStruct_ov96_0220A4DC_Blob36;

void ov96_0220A4DC(PokeathlonCourseData *arg0);

void ov96_0220A4DC(PokeathlonCourseData *arg0) {
    s32 r6;
    int i;
    UnkStruct_ov96_0220A4DC_Blob36 *copy;
    Ov96_0220Data *heap;
    u8 *area;
    UnkStruct_ov96_0220A4DC_Bits *dst;

    r6 = ov96_021E5F24(arg0);
    heap = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    area = PokeathlonCourse_GetDataCopyArea(arg0);
    if (heap == NULL) {
        GF_AssertFail();
    }
    if (heap->unk_4C == NULL) {
        GF_AssertFail();
    }
    if (heap->unk_40 == NULL) {
        GF_AssertFail();
    }
    if (r6 == 0) {
        dst = (UnkStruct_ov96_0220A4DC_Bits *)ov96_021E8A20(area + 0x28);
        copy = (UnkStruct_ov96_0220A4DC_Blob36 *)ov96_021E8A20(area + 0x50);
        *copy = *(UnkStruct_ov96_0220A4DC_Blob36 *)ov96_021E8A20(area);
        area += 0x50;
        for (i = 0; i < 4; i++) {
            UnkStruct_ov96_0220A4DC_Bits *src = (UnkStruct_ov96_0220A4DC_Bits *)ov96_021E8A20(area);
            dst->a = src->a;
            dst->b = src->b;
            dst->c = src->c;
            dst->d = src->d;
            dst->e = src->e;
            dst->f = src->g;
            dst++;
            area += 0x28;
        }
    }
    ov96_0220A424(arg0);
}

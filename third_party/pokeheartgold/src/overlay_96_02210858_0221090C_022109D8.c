#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "error_handling.h"

u8 ov96_0220E7BC(PokeathlonCourseData *, u8);

s16 ov96_0220E6DC(int, int);

UnkStruct_ov96_0220E3F0_Record *ov96_0220E74C(UnkStruct_ov96_0220E3F0 *, u8);

s32 ov96_0220E818(UnkStruct_ov96_0220E3F0 *, s16 *, u8, u8);

typedef struct {
    PokeathlonCourseData *course;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u32 flag : 1;
    u32 idx : 2;
    u32 unkC_3 : 2;
    u32 unkC_5 : 2;
    u32 state : 4;
    u32 unkC_11 : 21;
} UnkStruct_ov96_022109D8;

s32 ov96_02210BD0(UnkStruct_ov96_022109D8 *, int, int);

void ov96_022109D8(UnkStruct_ov96_022109D8 *work);

void ov96_022109D8(UnkStruct_ov96_022109D8 *work) {
    UnkStruct_ov96_0220E3F0 *data;
    UnkStruct_ov96_0220E3F0_Record *entry;
    int i;

    data = PokeathlonCourse_GetHeapAllocPtr4(work->course);
    ov96_0220E7BC(work->course, work->unkC_3);
    switch (work->state) {
    case 0:
        if (work->flag != 0) {
            GF_AssertFail();
        }
        work->unk4 = work->unk8;
        for (i = 0; i < 2; i++) {
            if ((((u32)ov96_0220E74C(data, i)->unk_4 << 0x1A) >> 0x1F) != 0 && ov96_02210BD0(work, i, 0x38) != 0) {
                work->idx = i;
                work->flag = 1;
                work->state = 1;
            }
        }
        break;
    case 1:
        if (work->flag != 1) {
            GF_AssertFail();
        }
        if (work->flag) {
            entry = ov96_0220E74C(data, work->idx);
            if ((((u32)entry->unk_4 << 0x1A) >> 0x1F) != 0) {
                work->unk4 = ov96_0220E6DC((s16)(work->unk6 + 0x120), entry->unk_9);
            } else {
                work->flag = 0;
                work->state = 0;
            }
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov96_0220E818(data, &work->unk4, work->unkC_3, work->unkC_5);
}

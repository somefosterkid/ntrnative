#include "global.h"
struct PokeathlonCourseData;

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

s32 ov96_021E8340(s32, s32, void *);

void ov96_021E8228(PokeathlonCourseData *arg0, u32 arg1, u32 arg2, s32 arg3, s32 arg4);

void ov96_021E8228(PokeathlonCourseData *arg0, u32 arg1, u32 arg2, s32 arg3, s32 arg4) {
    s32 v0;
    u32 v1;

    v0 = arg3;
    if (arg3 == 0) {
        if (arg1 != ov96_021E5F24(arg0)) {
            GF_AssertFail();
        }
        if (arg4 != 1) {
            GF_AssertFail();
        }
        v1 = *(u32 *)((u8 *)arg0 + 0x8B0);
        if (v1 < 0x270FU) {
            *(u32 *)((u8 *)arg0 + 0x8B0) = (u32)(v1 + 1);
        }
    } else {
        if (arg2 >= 3U) {
            GF_AssertFail();
        }
        if (*(s32 *)((u8 *)arg0->state + 0x10) == 0) {
            if (ov96_021E5F24(arg0) != 0) {
                GF_AssertFail();
            }
            ov96_021E8340(v0, (u8)arg4, &arg0->fieldData[0].data[0x60 * arg1] + (arg2 << 5));
            return;
        }
        if (ov96_021E5F24(arg0) == 0) {
            if (arg1 >= (u32)PokeathlonCourse_GetParticipantCount(arg0)) {
                ov96_021E8340(v0, (u8)arg4, &arg0->fieldData[0].data[0x60 * arg1] + (arg2 << 5));
                return;
            }
            if (arg1 != ov96_021E5F24(arg0)) {
                GF_AssertFail();
            }
            ov96_021E8340(v0, (u8)arg4, arg0->filler_B44 + (arg2 << 5));
            return;
        }
        if (arg1 != ov96_021E5F24(arg0)) {
            GF_AssertFail();
        }
        ov96_021E8340(v0, (u8)arg4, arg0->filler_B44 + (arg2 << 5));
    }
}

void ov96_021E8318(struct PokeathlonCourseData *arg0, s16 arg1);

void ov96_021E8318(PokeathlonCourseData *arg0, s16 arg1) {
    *((s16 *)&arg0->filler_D2A) = arg1;
}

void ov96_021E8324(struct PokeathlonCourseData *arg0, void *arg1);

void ov96_021E8324(PokeathlonCourseData *arg0, void *arg1) {
    if (*((s32 *)&arg0->unk_D6C) != 0) {
        GF_AssertFail();
    }
    *((void **)&arg0->unk_D6C) = arg1;
}

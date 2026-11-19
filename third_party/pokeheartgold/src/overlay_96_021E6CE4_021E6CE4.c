#include "global.h"

#include "pokeathlon/pokeathlon.h"

extern const void *_0221DA00[];

extern u32 ov96_0221DA28[];

s32 ov96_021E6CE4(PokeathlonCourseData *arg0);

s32 ov96_021E6CE4(PokeathlonCourseData *arg0) {
    u8 idx = arg0->field_3D8[arg0->currentParticipantIndex];
    arg0->unk_728 = 0;
    arg0->filler_D2A = 0xFFFF;
    arg0->filler_72B[0] = 0;
    arg0->unk_D6C = 0;
    PokeathlonCourse_ResetDataCopyArea(arg0);
    arg0->state = ov96_0221DA28[idx];
    arg0->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(_0221DA00[idx], (PokeathlonStateInfo *)arg0->filler_3CC);
    PokeathlonCourse_SetStateField07(arg0, 0x10);
    return 0;
}
